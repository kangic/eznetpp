//  Copyright [2015] <kangic21@gmail.com>

#include "io_manager.h"

namespace eznetpp {
namespace sys {

io_manager::io_manager(void) {

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

int io_manager::add_socket(const eznetpp::net::socket& sock) {
  struct epoll_event ev;

  ev.events = EPOLLIN;
  ev.data.fd = sock.descriptor();

  return epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, sock.descriptor(), &ev);
}

int io_manager::del_socket(const eznetpp::net::socket& sock) {
  return epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, sock.descriptor(), NULL);
}

int io_manager::send_data(const eznetpp::net::socket& sock
                         , const std::string& data, int len) {
  int send_bytes = send(sock.descriptor(), data.c_str(), len, MSG_NOSIGNAL);
}

int io_manager::loop(void) {
  _read_th = std::thread(&io_manager::read_loop, this);

  if (!_read_th.joinable()) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "failed to create read thread(%d)"
                           , errno);
    return -1;
  }

  _read_th.join();

  return 0;
}

void io_manager::read_loop(void) {
  eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "start read_loop");

  while (1) {
    int changed_events = epoll_wait(_epoll_fd, _events, _max_descs_cnt, -1);
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                          , __FILE__, __FUNCTION__, __LINE__
                          , "changed_events : %d"
                          , changed_events);

    for (int i = 0; i < changed_events; i++) {
      /*
      // 
      if (_events[i].data.fd == _server_socket) {
        //do_accept();
        {
          std::unique_lock<std::mutex> lk(_worker_mutex);
        }
        _acceptor_cv.notify_one();
      } else {
        //do_read(_events[i]);
        {
          std::unique_lock<std::mutex> lk(_worker_mutex);
          _received_event_fd = _events[i].data.fd;
        }
        _worker_cv.notify_one();
      }
      */
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

}  // namespace sys
}  // namespace eznetpp
