// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_IO_MANAGER_H_
#define INCLUDE_IO_MANAGER_H_

#include "eznetpp.h"

namespace eznetpp {
namespace sys {
class socket;
class io_manager {
 public:
  static io_manager& instance() {
    if (_inst == nullptr)
      _inst = new io_manager();

    return *_inst;
  }

  int add_fd(int fd);
  int del_fd(int fd);

  int send_data(int fd, const std::string& data, int len);

  // create read_loop thread in the function.
  int loop(void);

 protected:
  void* read_loop(void);
 
 private:
   io_manager(void);
  virtual ~io_manager(void);

  static io_manager* _inst = null;

  int create_epoll_fd_and_events();

  int _epoll_fd = -1;
  struct epoll_event* _events = nullptr;
  int _received_event_fd = -1;
  int _max_connections_cnt = 1000;

  std::thread _read_th;

  std::vector<int> _polling_list;

  DISALLOW_COPY_AND_ASSIGN(io_manager);
};
}  // namespace sys
}  // namespace eznetpp

#endif INCLUDE_IO_MANAGER_H_
