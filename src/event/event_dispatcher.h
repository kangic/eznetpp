// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_EVENT_DISPATCHER_H_
#define INCLUDE_EVENT_DISPATCHER_H_

#include "eznetpp.h"
#include "event/event.h"
#include "event/event_handler.h"
#include "net/socket.h"

namespace eznetpp {
namespace event {
class event_dispatcher {
 public:
  static event_dispatcher& instance() {
    if (_evt_dispatcher == nullptr)
      _evt_dispatcher = new event_dispatcher();

    return *_evt_dispatcher;
  }

  int init(int num_of_disp_threads);

  bool register_socket_event_handler(eznetpp::net::socket* sock
      , event_handler* handler);
  bool deregister_socket_event_handler(eznetpp::net::socket* sock);

  void push_event(io_event* evt);

 protected:
  void dispatch_loop(int id);

 private:
  event_dispatcher(void);
  virtual ~event_dispatcher(void);

  static event_dispatcher* _evt_dispatcher;

  // work threads
  std::vector<std::thread> _disp_ths_vec;

  // socket<key>, socket_class<value>
  std::map<eznetpp::net::socket*, event_handler*> _handlers_map;
  std::mutex _handlers_map_mutex;

  std::vector<io_event*> _ioevents_vec;

  // condition variables
  std::condition_variable _disp_th_cv;  
  std::mutex _disp_th_cv_mutex;

  DISALLOW_COPY_AND_ASSIGN(event_dispatcher);
};
}  // namespace event
}  // namespace eznetpp

#endif  // INCLUDE_EVENT_DISPATCHER_H_
