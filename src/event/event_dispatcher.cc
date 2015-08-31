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
}

int event_dispatcher::init(int num_of_disp_threads) {
  for (int i = 0; i < num_of_disp_threads; ++i) {
    std::thread disp_th = std::thread(&event_dispatcher::dispatch_loop, this, i);

    if (!disp_th.joinable()) {
      eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "failed to create dispatch thread(%d)"
                           , errno);

      return -1;
    }

    _disp_ths_vec.push_back(std::move(disp_th));
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
    _ioevents_vec.push_back(std::move(evt));
  }

  std::unique_lock<std::mutex> lk(_disp_th_cv_mutex);
  _disp_th_cv.notify_one();
}

void event_dispatcher::dispatch_loop(int id) {
  eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "start dispatch_loop(id : %d)"
                         , id);

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
       
    // step 3. process the ioevent to the its handler
    eznetpp::net::if_socket* sock = evt->publisher();
    event_handler* handler = _handlers_map[sock];

    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
        , __FILE__, __FUNCTION__, __LINE__
        , "id(%d), io_event : %p(type : %d), socket : %p, handler : %p"
        , id, evt, evt->type(), &sock, handler);

    switch (evt->type()) {
      case event::event_type::close:
        {
          // Delete the socket descriptor from epoll descriptor automatically
          // when the socket is closed.
          close_socket_and_clear_resources(sock, handler);
          evt->done();
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
              evt->done();
              eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                  , __FILE__, __FUNCTION__, __LINE__
                  , "::accept() error(%d)", errno);
            }
            break;
          } 
          eznetpp::net::tcp::tcp_socket* tcp_sock = new eznetpp::net::tcp::tcp_socket(sock_fd);
          tcp_sock->set_peer_info(inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
          tcp_sock->set_nonblocking();
          handler->on_accept(tcp_sock, 0);

          // TODO : push the socket to new list
          {
            std::lock_guard<std::mutex> lock(_sockets_vec_mutex);
            _sockets_vec.push_back(std::move(tcp_sock));
          }

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

          sock->set_peer_info(evt->data().c_str(), evt->opt_data());
          sock->set_nonblocking();
          handler->on_connect(0);

          evt->done();

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
              printf("errno : %d\n", errno);
              close_socket_and_clear_resources(sock, handler);
            }
          } else if (len == 0) {
            // socket is closed gracefully.
            close_socket_and_clear_resources(sock, handler);
          } else {
            // problem
            if (errno) {
              printf("id(%d) - errno : %d, socket : %p, len : %d\n"
                  , id, errno, sock, len);
              close_socket_and_clear_resources(sock, handler);
            } else {
              handler->on_recv(data, len, errno);
            }
            evt->done();
          }
          break;
        }
      case event::event_type::tcp_send:
        {
          int len = ::send(sock->descriptor(), evt->data().c_str(), evt->opt_data(), MSG_NOSIGNAL);
          if (len == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
              // descriptor is empty.
            } else {
              // TODO : implement the error case
              printf("errno : %d\n", errno);
              close_socket_and_clear_resources(sock, handler);
            }
          } else if (len > 0) {
            handler->on_send(len, errno);
            evt->done();
          } else {
            eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                , __FILE__, __FUNCTION__, __LINE__
                , "send error(%d)"
                , errno);
          }
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

      // step 4. delete ioevent
      if (evt != nullptr && evt->is_done()) {
        delete evt;
        evt = nullptr;
      }

      eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "process a event from this");

    } // while-loop
}

void event_dispatcher::close_socket_and_clear_resources(
    eznetpp::net::if_socket* sock, event_handler* handler) {
  // step 1. delete events
  {
    std::lock_guard<std::mutex> guard(_ioevents_vec_mutex);
    auto iter = _ioevents_vec.begin();
    while (iter != _ioevents_vec.end()) {
      if ((*iter)->publisher() == sock) {
        delete *iter;
        *iter = nullptr;

        iter = _ioevents_vec.erase(iter);
      } else {
        ++iter;
      }
    }
  }

  // step 2. close socket and call to on_close
  ::close(sock->descriptor());
  sock->descriptor(-1);
 
  handler->on_close(0);

  // step 3. erase the socket and then delete it
  {
    std::lock_guard<std::mutex> guard(_sockets_vec_mutex);
    auto iter = _sockets_vec.begin();
    while (iter != _sockets_vec.end()) {
      if ((*iter) == sock) {
        delete *iter;
        *iter = nullptr;

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
