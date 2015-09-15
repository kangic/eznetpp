//  Copyright [2015] <kangic21@gmail.com>

#include "event_dispatcher.h"
#include "net/tcp/tcp_socket.h"

namespace eznetpp {
namespace event {

event_dispatcher* event_dispatcher::_evt_dispatcher = nullptr;

event_dispatcher::event_dispatcher(void) {

}

event_dispatcher::~event_dispatcher(void) {
  _disp_th.join();
}

int event_dispatcher::init(void) {
  _disp_th = std::thread(&event_dispatcher::dispatch_loop, this);

  if (!_disp_th.joinable()) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
        , __FILE__, __FUNCTION__, __LINE__
        , "failed to create dispatch thread(%d)"
        , errno);

    return -1;
  }

  return 0;
}

bool event_dispatcher::register_socket_event_handler(eznetpp::net::if_socket* sock, event_handler* handler) {
  std::lock_guard<std::mutex> guard(_handlers_map_mutex);
  _handlers_map[sock] = handler;
  return true;
}

bool event_dispatcher::deregister_socket_event_handler(eznetpp::net::if_socket* sock) {
  // erase the handler
  {
    std::lock_guard<std::mutex> guard(_handlers_map_mutex);
    if (_handlers_map.find(sock) == _handlers_map.end())
      return false;

    _handlers_map.erase(sock);
  }

  return true;
}

void event_dispatcher::push_event(io_event* evt) {
  {
    std::lock_guard<std::mutex> lock(_ioevents_vec_mutex);
    // TODO : re-think the below case(urgent case?? is it right?)
    if (evt->type() == event::event_type::close)
      _ioevents_vec.emplace(_ioevents_vec.begin(), evt);
    else
      _ioevents_vec.emplace_back(evt);
  }

  std::unique_lock<std::mutex> lk(_disp_th_cv_mutex);
  _disp_th_cv.notify_one();
}

void event_dispatcher::dispatch_loop(void) {
  eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "start dispatch_loop");
                         

  while (1) {
    io_event* evt = nullptr;

    {
      // step 1. wait for signal
      std::unique_lock<std::mutex> lk(_disp_th_cv_mutex);
      while (_ioevents_vec.empty()) {
        _disp_th_cv.wait(lk);
      }

      // step 2. pop from the vector and then erase the iterator from the vector
      {
        std::lock_guard<std::mutex> lock(_ioevents_vec_mutex);
        evt = _ioevents_vec.front();
        _ioevents_vec.erase(_ioevents_vec.begin());
      }
    }
       
    // step 3. process the ioevent
    process_event(evt);

    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
        , __FILE__, __FUNCTION__, __LINE__
        , "process a event from this");

  } // while-loop
}

void event_dispatcher::process_event(io_event* evt) {
  eznetpp::net::if_socket* sock = evt->publisher();
  event_handler* handler = _handlers_map[sock];

  eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
      , __FILE__, __FUNCTION__, __LINE__
      , "io_event : %p(type : %d), socket : %p, handler : %p"
      , evt, evt->type(), &sock, handler);

  switch (evt->type()) {
    case event::event_type::close:
      {
        // Delete the socket descriptor from epoll descriptor automatically
        // when the socket is closed.
        clear_resources(sock, handler);
        evt->done();

        break;
      }
    case event::event_type::tcp_accept:
      {
        int sock_fd = evt->result();

        eznetpp::net::tcp::tcp_socket* tcp_sock = new eznetpp::net::tcp::tcp_socket(sock_fd);
        tcp_sock->set_peer_info(std::move(evt->data()), evt->opt_data());
        tcp_sock->set_nonblocking();
        tcp_sock->set_tcpnodelay();
        handler->on_accept(tcp_sock, 0);
        evt->done();

        // TODO : push the socket to new list
        {
          std::lock_guard<std::mutex> lock(_sockets_vec_mutex);
          _sockets_vec.push_back(std::move(tcp_sock));
        }

        break;
      }
    case event::event_type::tcp_connect:
      {
        sock->set_peer_info(evt->data().c_str(), evt->opt_data());
        handler->on_connect(sock->last_error());
        evt->done();

        break;
      }
    case event::event_type::tcp_recv:
      {
        // TODO : implement recv for epoll et mode
        handler->on_recv(evt->data(), evt->result());
        evt->done();

        break;
      }
    case event::event_type::tcp_send:
      {
        handler->on_send(evt->result());
        evt->done();

        break;
      }
    case event::event_type::udp_recvfrom:
      {
        handler->on_recvfrom(evt->data(), evt->result()
            , sock->peer().ip, sock->peer().port);
        evt->done();

        break;
      }
    case event::event_type::udp_sendto:
      {
        handler->on_sendto(evt->result());
        evt->done();

        break;
      }
  }

  // step 4. delete ioevent
  if (evt != nullptr && evt->is_done()) {
    delete evt;
    evt = nullptr;
  }
}

void event_dispatcher::clear_resources(
    eznetpp::net::if_socket* sock, event_handler* handler) {
  // step 1. delete events
  {
    std::lock_guard<std::mutex> guard(_ioevents_vec_mutex);
    auto iter = _ioevents_vec.begin();
    while (iter != _ioevents_vec.end()) {
      if ((*iter)->publisher() == sock) {
        //process_event(*iter);

        iter = _ioevents_vec.erase(iter);
      } else {
        ++iter;
      }
    }
  }

  // step 2. call to on_close
  if (handler != nullptr)
    handler->on_close(sock->last_error());

  // step 3. erase the socket and then delete it
  {
    std::lock_guard<std::mutex> guard(_sockets_vec_mutex);
    auto iter = _sockets_vec.begin();
    while (iter != _sockets_vec.end()) {
      if ((*iter) == sock) {
        if ((*iter) != nullptr) {
          delete *iter;
          *iter = nullptr;
        }

        iter = _sockets_vec.erase(iter);

        break;
      } else {
        ++iter;
      }
    }
  }

  // step 4. deregister and then delete handler class
  deregister_socket_event_handler(sock);

  if (handler != nullptr) {
    delete handler;
    handler = nullptr;
  }
}

}  // namespace event
}  // namespace eznetpp
