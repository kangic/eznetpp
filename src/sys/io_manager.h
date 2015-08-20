// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_IO_MANAGER_H_
#define INCLUDE_IO_MANAGER_H_

#include "eznetpp.h"
#include "event/event_dispatcher.h"
#include "net/socket.h"

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
  int init(int max_descs_cnt = 1024); // TODO(kangic) : define the value

  /*
   * Add(delete) the socket to observe by epoll descriptor.
   */
  int add_socket(eznetpp::net::socket* sock);
  int del_socket(eznetpp::net::socket* sock);

  /*
   * Create the read_loop thread in this function.
   */
  int loop(void);

 protected:
  /*
   * This function works to poll descriptors in _epoll_fd.
   * If exists a changed descriptor, read data and pass to event_dispatcher for
   * queueing.
   */
  void read_loop(void);
 
 private:
  // variables
  int _epoll_fd = -1;
  struct epoll_event* _events = nullptr;
  int _received_event_fd = -1;
  int _max_descs_cnt = 1024;

  // socket_id<key>, socket_class<value>
  std::map<int, eznetpp::net::socket*> _sock_maps;
  std::mutex _sock_maps_mutex;

  // event_dispatcher _dispatcher;
  std::thread _read_th;

  DISALLOW_COPY_AND_ASSIGN(io_manager);
};
}  // namespace sys
}  // namespace eznetpp

#endif  // INCLUDE_IO_MANAGER_H_
