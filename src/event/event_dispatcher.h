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

#ifndef INCLUDE_EVENT_DISPATCHER_H_
#define INCLUDE_EVENT_DISPATCHER_H_

#include "eznetpp.h"
#include "event/event.h"
#include "event/event_handler.h"
#include "net/if_socket.h"

namespace eznetpp {
namespace event {
class event_dispatcher
{
 public:
  static event_dispatcher& instance()
  {
    if (_evt_dispatcher == nullptr)
      _evt_dispatcher = new event_dispatcher();

    return *_evt_dispatcher;
  }

  static void release()
  {
    if (_evt_dispatcher != nullptr) {
      delete _evt_dispatcher;
      _evt_dispatcher = nullptr;
    }
  }

  int init(int num_of_disp_threads);

  bool register_socket_event_handler(eznetpp::net::if_socket* sock
      , event_handler* handler);
  bool deregister_socket_event_handler(eznetpp::net::if_socket* sock);
  
  void push_event(io_event* evt);

 protected:
  void dispatch_loop(int id);

 private:
  event_dispatcher(void);
  virtual ~event_dispatcher(void);

  void process_event(io_event* evt);
  void clear_resources(eznetpp::net::if_socket* sock
      , event_handler* handler);

  static event_dispatcher* _evt_dispatcher;

  // work threads
  int _num_of_disp_threads;
  std::vector<std::thread> _disp_threads_vec;

  // socket<key>, socket_class<value>
  std::map<eznetpp::net::if_socket*, event_handler*> _handlers_map;
  std::mutex _handlers_map_mutex;

  std::vector<eznetpp::net::if_socket*> _sockets_vec;
  std::mutex _sockets_vec_mutex;

  std::vector<io_event*> _ioevents_vec;
  std::mutex _ioevents_vec_mutex;

  // condition variable for dispatching the event
  std::condition_variable _disp_th_cv;  
  std::mutex _disp_th_cv_mutex;

  // condition variable for exiting the thread
  bool bClosed = false;
  std::condition_variable _exit_cv;
  std::mutex _exit_mutex;

  DISALLOW_COPY_AND_ASSIGN(event_dispatcher);
};
}  // namespace event
}  // namespace eznetpp

#endif  // INCLUDE_EVENT_DISPATCHER_H_
