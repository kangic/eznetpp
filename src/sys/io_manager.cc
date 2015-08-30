//  Copyright [2015] <kangic21@gmail.com>

#include "io_manager.h"
#include "event/event_dispatcher.h"


namespace eznetpp {
namespace sys {

io_manager::io_manager(int num_of_disp_threads, bool log_enable) {
  _num_of_disp_threads = num_of_disp_threads;
  eznetpp::util::logger::instance().set_enable_option(log_enable);
}

io_manager::~io_manager(void) {
}

/*
 * create epoll file descriptor and events
 */
int io_manager::init(int max_descs_cnt) {
  // create epoll fd and event structures
  _epoll_fd = epoll_create(max_descs_cnt);
  if (_epoll_fd == -1) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                          , __FILE__, __FUNCTION__, __LINE__
                          , "epoll_create error(%d)"
                          , errno);
  
    return errno;
  }

  _events = new epoll_event[max_descs_cnt];
  if (_events == nullptr) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                          , __FILE__, __FUNCTION__, __LINE__
                          , "failed to create events(%d)"
                          , errno);

    return errno;
  }

  eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                        , __FILE__, __FUNCTION__, __LINE__
                        , "created epoll_fd : %d"
                        , _epoll_fd);

  _max_descs_cnt = max_descs_cnt;

  return 0;

}

int io_manager::register_socket_event_handler(eznetpp::net::if_socket* sock
      , eznetpp::event::event_handler* handler) {
  // Find the socket class in the event_dispatcher's handlers map to check
  // to register already.
  if (!eznetpp::event::event_dispatcher::instance().register_socket_event_handler(sock, handler)) {
    return -1;
  }

  struct epoll_event ev;

  ev.events = EPOLLIN | EPOLLET;

  ev.data.ptr = sock;

  return epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, sock->descriptor(), &ev);
}

int io_manager::deregister_socket_event_handler(eznetpp::net::if_socket* sock) {
  eznetpp::event::event_dispatcher::instance().deregister_socket_event_handler(sock);
  return epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, sock->descriptor(), NULL);
}

int io_manager::loop(void) {
  if (eznetpp::event::event_dispatcher::instance().init(_num_of_disp_threads) == -1) {
    return -1;
  }

  _loop_th = std::thread(&io_manager::epoll_loop, this);

  if (!_loop_th.joinable()) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "failed to create read thread(%d)"
                           , errno);
    return -1;
  }


  _loop_th.join();

  return 0;
}

void io_manager::epoll_loop(void) {
  eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "start read_loop");

  while (1) {
    int changed_events = epoll_wait(_epoll_fd, _events, _max_descs_cnt, -1);
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                          , __FILE__, __FUNCTION__, __LINE__
                          , "changed_events : %d"
                          , changed_events);

    if (changed_events < 0) {
      eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                          , __FILE__, __FUNCTION__, __LINE__
                          , "epoll failed");

      // TODO : decide to the action
      break;
    }

    for (int i = 0; i < changed_events; ++i) {
      eznetpp::net::if_socket* sock = static_cast<eznetpp::net::if_socket*>(_events[i].data.ptr);

      eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                          , __FILE__, __FUNCTION__, __LINE__
                          , "changed descriptor : %d"
                          , sock->descriptor());

      if (_events[i].events & EPOLLIN) {
        sock->recv();
      } else if (_events[i].events & EPOLLHUP || _events[i].events & EPOLLERR) {
        // TODO : add log
        printf("epollhup or epollerr\n");
        sock->close();
      }
    }
  }
}

}  // namespace sys
}  // namespace eznetpp
