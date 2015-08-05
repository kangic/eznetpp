//  Copyright [2015] <kangic21@gmail.com>

#include "io_manager.h"

namespace eznetpp {
namespace sys {

io_manager::io_manager(void) {

}

io_manager::~io_manager(void) {

}

int io_manager::add_fd(int fd) {
  struct epoll_event ev;

  ev.events = EPOLLIN;
  ev.data.fd = cfd;

  return epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &ev);
}

int io_manager::del_fd(int fd) {
  return epoll_ctl(efd, EPOLL_CTL_DEL, cfd, NULL);
}

int io_manager::send_data(int fd, const std::string& data, int len) {
  int send_bytes = send(fd, data.c_str(), len, MSG_NOSIGNAL);
}

int io_manager::loop(void) {
  ret = create_epoll_fd_and_events();
  if (ret != 0) {
    return ret;
  }

  _read_th = std::thread(&io_manager::read_thread, this);

  if (!_read_th.joinable()) {
    logger::instance().log(logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "failed to create read thread(%d)"
                           , errno);
    return -1;
  }

  _read_th.join();

  return 0;
}

void* io_manager::read_loop(void) {

}

////////////////////////////////////////////////////////////////////////////////
// create epoll file descriptor and events
int io_manager::create_epoll_fd_and_events() {
  // create epoll fd and event structures
  _epoll_fd = epoll_create(_max_connections_cnt);
  if (_epoll_fd == -1) {
    logger::instance().log(logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "epoll_create error(%d)"
                           , errno);
  
    return errno;
  }

  _events = new epoll_event[_max_connections_cnt];
  if (_events == nullptr) {
    logger::instance().log(logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "failed to create events(%d)"
                           , errno);

    return errno;
  }

  return 0;
}

 
}  // namespace sys
}  // namespace eznetpp
