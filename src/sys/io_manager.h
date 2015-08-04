// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_IO_MANAGER_H_
#define INCLUDE_IO_MANAGER_H_

#include <eznetpp.h>

namespace eznetpp {
namespace sys {
class socket;
class io_manager {
 public:
  io_manager(void);
  virtual ~io_manager(void);

  void add_fd(int fd);
  void del_fd(int fd);

  int send_data(int fd, const char* data, int len);

  void loop(void);

 protected:
  void* read_loop(void);
 
 private:
  int _epoll_fd = -1;
  struct epoll_event* _events = nullptr;
  int _received_event_fd = -1;

  std::thread _read_th;

  std::vector<int> _polling_list;

  DISALLOW_COPY_AND_ASSIGN(io_manager);
};
}  // namespace sys
}  // namespace eznetpp

#endif INCLUDE_IO_MANAGER_H_
