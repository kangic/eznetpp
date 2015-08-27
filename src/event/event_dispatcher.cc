//  Copyright [2015] <kangic21@gmail.com>

#include "event_dispatcher.h"
#include "net/tcp/tcp_socket.h"

namespace eznetpp {
namespace event {

event_dispatcher* event_dispatcher::_evt_dispatcher = nullptr;

event_dispatcher::event_dispatcher(void) {

}

event_dispatcher::~event_dispatcher(void) {
  for (auto& th : _disp_ths_vec) {
    th.join();
  }
  //_dispatch_th.join();
}

int event_dispatcher::init(int num_of_disp_threads) {
  for (int i = 0; i < num_of_disp_threads; ++i) {
    std::thread disp_th = std::thread(&event_dispatcher::dispatch_loop, this, i);

    if (!disp_th.joinable()) {
      eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "failed to create dispatch thread(%d)"
                           , errno);
    }

    _disp_ths_vec.emplace_back(std::move(disp_th));
  }
}

bool event_dispatcher::register_socket_event_handler(eznetpp::net::socket* sock
      , event_handler* handler) {
  std::lock_guard<std::mutex> guard(_handlers_map_mutex);
  _handlers_map[sock] = handler;
  return true;
}

bool event_dispatcher::deregister_socket_event_handler(eznetpp::net::socket* sock) {
  std::lock_guard<std::mutex> guard(_handlers_map_mutex);
  if (_handlers_map.find(sock) == _handlers_map.end())
    return false;

  _handlers_map.erase(sock);

  return true;
}

void event_dispatcher::push_event(io_event* evt) {
  std::unique_lock<std::mutex> lk(_disp_th_cv_mutex);
  _ioevents_vec.emplace_back(evt);
  _disp_th_cv.notify_one();
}

void event_dispatcher::dispatch_loop(int id) {
  eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "start dispatch_loop(id : %d)"
                         , id);
  bool socket_closed = false;

  while (1) {
    io_event* evt = nullptr;

    {
      // step 1. wait for signal
      std::unique_lock<std::mutex> lk(_disp_th_cv_mutex);
      while (_ioevents_vec.empty()) {
        _disp_th_cv.wait(lk);
      }

      // step 2. pop from the vector and then erase the iterator from the vector
      evt = _ioevents_vec.front();
      _ioevents_vec.erase(_ioevents_vec.begin());
    }
       
    // step 3. process the ioevent to the its handler
    eznetpp::net::socket* sock = evt->publisher();
    event_handler* handler = _handlers_map[sock];

    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
        , __FILE__, __FUNCTION__, __LINE__
        , "id(%d), io_event : %p(type : %d), socket : %p, handler : %p"
        , id, evt, evt->type(), sock, handler);

    socket_closed = false;
    switch (evt->type()) {
      case event::event_type::close:
        {
          // Delete the socket descriptor from epoll descriptor automatically
          // when the socket is closed.
          int ret = ::close(sock->descriptor());
          sock->descriptor(-1);
          socket_closed = true;
          break;
        }
      case event::event_type::accept:
        {
          struct sockaddr_in client_addr;
          socklen_t client_addr_len = sizeof(client_addr);

          int sock_fd = ::accept(sock->descriptor(), (struct sockaddr *)&client_addr
              , &client_addr_len);

          if (sock_fd == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
              // descriptor is empty.
            } else {
              // TODO : implement the error case
              handler->on_accept(nullptr, errno);
              eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                  , __FILE__, __FUNCTION__, __LINE__
                  , "::accept() error(%d)", errno);
            }
            break;
          } 
          eznetpp::net::tcp::tcp_socket* sock = new eznetpp::net::tcp::tcp_socket(sock_fd);
          sock->set_peerinfo(inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
          sock->set_nonblocking();

          handler->on_accept(sock, 0);

          break;
        }
      case event::event_type::connect:
        {
          struct sockaddr_in server_addr;
          bzero(&server_addr, sizeof(server_addr));
          server_addr.sin_family = AF_INET;
          server_addr.sin_addr.s_addr = inet_addr(evt->data().c_str());
          server_addr.sin_port = htons(evt->opt_data());

          // connect to server(on_connect event)
          int ret = ::connect(sock->descriptor(), (struct sockaddr *)&server_addr
              , sizeof(server_addr));
          if (ret == -1) {
            handler->on_connect(errno);
            eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                , __FILE__, __FUNCTION__, __LINE__
                , "::connect() error(%d)", errno);
            break;
          }

          sock->set_peerinfo(evt->data().c_str(), evt->opt_data());
          sock->set_nonblocking();
          handler->on_connect(0);

          break;
        }
      case event::event_type::tcp_recv:
        {
          // TODO : implement recv for epoll et mode
          std::string data(eznetpp::opt::max_transfer_bytes, '\0');
          int len = ::recv(sock->descriptor(), &data[0], eznetpp::opt::max_transfer_bytes, MSG_NOSIGNAL);
          if (len == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
              // descriptor is empty.
            } else {
              // TODO : implement the error case
            }
          } else if (len == 0) {
            // socket is closed gracefully.
            int ret = ::close(sock->descriptor());
            sock->descriptor(-1);
            socket_closed = true;
          } else {
            handler->on_recv(data, len, errno);
          }
          break;
        }
      case event::event_type::tcp_send:
        {
          int len = ::send(sock->descriptor(), evt->data().c_str(), evt->opt_data(), MSG_NOSIGNAL);
          handler->on_send(len, errno);
          break;
        }
      case event::event_type::udp_recv:
        {
          break;
        }
      case event::event_type::udp_send:
        {
          break;
        }
      }

      if (socket_closed) {
        handler->on_close(0);
        deregister_socket_event_handler(sock);

        if (handler != nullptr) {
          delete handler;
          handler = nullptr;
        }

        if (sock != nullptr) {
          delete sock;
          sock = nullptr;
        }
      }

      // step 4. delete ioevent
      if (evt != nullptr) {
        delete evt;
        evt = nullptr;
      }

      eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "process a event from this");

    } // while-loop

}

}  // namespace event
}  // namespace eznetpp
