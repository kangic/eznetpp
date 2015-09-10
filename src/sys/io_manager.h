// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_IO_MANAGER_H_
#define INCLUDE_IO_MANAGER_H_

#include "eznetpp.h"
#include "event/event_handler.h"
#include "net/if_socket.h"

namespace eznetpp {
namespace sys {
class io_manager {
 public:
  io_manager(bool log_enable = false);
  virtual ~io_manager(void);

  /*
   * After the class declaration, must be called this function for preparing 
   * to work epoll* functions.
   */
  int init(int max_descs_cnt = 1024); // TODO(kangic) : define the value

  /*
   * Register/Unregister the socket class and the event handler to recieve the
   * event.
   */
  int register_socket_event_handler(eznetpp::net::if_socket* sock
      , eznetpp::event::event_handler* handler);
  int deregister_socket_event_handler(eznetpp::net::if_socket* sock);
  
  /*
   * Create the read_loop thread in this function.
   */
  int loop(void);
  void stop(void);

  static int _epoll_fd;
 protected:
  /*
   * This function works to poll descriptors in _epoll_fd.
   * If exists a changed descriptor, read data and pass to event_dispatcher for
   * queueing.
   */
  void epoll_loop(void);
 
 private:
  struct epoll_event* _events = nullptr;
  int _max_descs_cnt = 1024;

  std::thread _loop_th;

  // for exiting thread
  bool bClosed = false;
  std::mutex _exit_mutex;
};

}  // namespace sys
}  // namespace eznetpp

#endif  // INCLUDE_IO_MANAGER_H_
