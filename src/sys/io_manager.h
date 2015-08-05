// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_IO_MANAGER_H_
#define INCLUDE_IO_MANAGER_H_

#include "eznetpp.h"

namespace eznetpp {
namespace sys {
class io_manager {
 public:
  io_manager(void);
  virtual ~io_manager(void);

  /*
   * After the class declaration, must be called this function for preparing 
   * to work epoll* functions.
   */
  int init(int max_fds_cnt = 1000); // TODO(kangic) : define a value

  /*
   * Add(delete) the descriptor to observe by epoll descriptor.
   */
  int add_fd(int fd);
  int del_fd(int fd);

  /*
   * Send the data through the fd.
   */
  int send_data(int fd, const std::string& data, int len);

  /*
   * Create the read_loop thread in this function.
   */
  int loop(void);

 protected:
  /*
   * 
   */
  void* read_loop(void);
 
 private:
  int create_epoll_fd_and_events(int _max_fds_cnt);

  // variables
  int _epoll_fd = -1;
  struct epoll_event* _events = nullptr;
  int _received_event_fd = -1;

  std::thread _read_th;

  DISALLOW_COPY_AND_ASSIGN(io_manager);
};
}  // namespace sys
}  // namespace eznetpp

#endif  // INCLUDE_IO_MANAGER_H_
