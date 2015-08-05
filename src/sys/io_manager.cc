//  Copyright [2015] <kangic21@gmail.com>

#include "io_manager.h"

namespace eznetpp {
namespace sys {

io_manager::io_manager(void) {

}

io_manager::~io_manager(void) {

}

int io_manager::init(int max_fds_cnt) {
  return create_epoll_fd_and_events(max_fds_cnt);
}

int io_manager::add_fd(int fd) {
  struct epoll_event ev;

  ev.events = EPOLLIN;
  ev.data.fd = fd;

  return epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &ev);
}

int io_manager::del_fd(int fd) {
  return epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
}

int io_manager::send_data(int fd, const std::string& data, int len) {
  int send_bytes = send(fd, data.c_str(), len, MSG_NOSIGNAL);
}

int io_manager::loop(void) {
  _read_th = std::thread(&io_manager::read_loop, this);

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
int io_manager::create_epoll_fd_and_events(int max_fds_cnt) {
  // create epoll fd and event structures
  _epoll_fd = epoll_create(max_fds_cnt);
  if (_epoll_fd == -1) {
    logger::instance().log(logger::log_level::error
                          , __FILE__, __FUNCTION__, __LINE__
                          , "epoll_create error(%d)"
                          , errno);
  
    return errno;
  }

  _events = new epoll_event[max_fds_cnt];
  if (_events == nullptr) {
    logger::instance().log(logger::log_level::error
                          , __FILE__, __FUNCTION__, __LINE__
                          , "failed to create events(%d)"
                          , errno);

    return errno;
  }

  logger::instance().log(logger::log_level::debug
                        , __FILE__, __FUNCTION__, __LINE__
                        , "created epoll_fd : %d"
                        , _epoll_fd);

  return 0;
}
 
}  // namespace sys
}  // namespace eznetpp
