/* Copyright (c) 2015 In Cheol, Kang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef INCLUDE_IO_MANAGER_H_
#define INCLUDE_IO_MANAGER_H_

#include "eznetpp.h"
#include "event/event_handler.h"
#include "net/if_socket.h"

namespace eznetpp {
namespace sys {
class io_manager
{
 public:
  io_manager(int num_of_work_threads = 1, bool log_enable = false);
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
      , eznetpp::event::if_event_handler* handler);
  int deregister_socket_event_handler(eznetpp::net::if_socket* sock);

  /*
   * Create the read_loop thread in this function.
   */
  int loop(void);
  void stop(void);

  static int update_epoll_event(eznetpp::net::if_socket* sock);

 protected:
  /*
   * This function works to poll descriptors in _epoll_fd.
   * If exists a changed descriptor, read data and pass to event_dispatcher for
   * queueing.
   */
  void epoll_loop(int idx);

 private:
  static int _epoll_fd;
  struct epoll_event* _events = nullptr;
  int _max_descs_cnt = 1024;
  int _num_of_loop_threads = 1;

  //std::thread _loop_th;
  std::vector<std::thread> _loop_threads_vec;

  // for exiting thread
  bool bClosed = false;
  std::mutex _exit_mutex;
  std::condition_variable _exit_cv;

  // for terminating
  std::mutex _term_mutex;
  std::condition_variable _term_cv;

  DISALLOW_COPY_AND_ASSIGN(io_manager);
};

}  // namespace sys
}  // namespace eznetpp

#endif  // INCLUDE_IO_MANAGER_H_
