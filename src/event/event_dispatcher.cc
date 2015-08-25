//  Copyright [2015] <kangic21@gmail.com>

#include "event_dispatcher.h"
#include "net/tcp/tcp_socket.h"

namespace eznetpp {
namespace event {

event_dispatcher* event_dispatcher::_evt_dispatcher = nullptr;

event_dispatcher::event_dispatcher(void) {

}

event_dispatcher::~event_dispatcher(void) {
  _dispatch_th.join();
}

int event_dispatcher::init(void) {
  _dispatch_th = std::thread(&event_dispatcher::dispatch_loop, this);

  if (!_dispatch_th.joinable()) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "failed to create dispatch thread(%d)"
                           , errno);
    return -1;
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
  _ioevents_vec.emplace_back(evt);
  // notify to wake the loop
}

void event_dispatcher::dispatch_loop() {
  eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "start dispatch_loop");

  while (1) {
    // step 1. wait for signal
    
    // step 2. check the mutex
    // pop from the vector and then erase the iterator from the vector
    
    // step 3. process the ioevent to the its handler

    // step 4. delete ioevent
    if (!_ioevents_vec.empty()) {
      io_event* evt = _ioevents_vec.front();
      eznetpp::net::socket* sock = reinterpret_cast<eznetpp::net::socket*>(evt->publisher());
      event_handler* handler = _handlers_map[sock];

      eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "io_event : %p(type : %d), socket : %p, handler : %p"
                         , evt, evt->type(), sock, handler);

      switch (evt->type()) {
        case event::event_type::on_close:
        {
          handler->on_close(evt->err_code());
          deregister_socket_event_handler(sock);

          if (handler != nullptr) {
            delete handler;
            handler = nullptr;
          }

          if (sock != nullptr) {
            delete sock;
            sock = nullptr;
          }

          break;
        }
        case event::event_type::on_accept:
        {
          // TODO(kangic) : Convert data to connector class.
          int sock_fd = evt->result();
          eznetpp::net::tcp::tcp_socket* sock = new eznetpp::net::tcp::tcp_socket(sock_fd);
          register_socket_event_handler(sock, nullptr);
          handler->on_accept(*sock, evt->err_code());
          break;
        }
        case event::event_type::on_connect:
        {
          //handler->
          break;
        }
        case event::event_type::on_recv:
        {
          handler->on_recv(evt->data(), evt->result(), evt->err_code());
          break;
        }
        case event::event_type::on_send:
        {
          handler->on_send(evt->result(), evt->err_code());
          break;
        }
      }

      if (evt != nullptr) {
        delete evt;
        evt = nullptr;
      }

      _ioevents_vec.erase(_ioevents_vec.begin());

      eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "process a event from this");
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

}  // namespace event
}  // namespace eznetpp
