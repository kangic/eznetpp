// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_EVENT_DISPATCHER_H_
#define INCLUDE_EVENT_DISPATCHER_H_

#include "eznetpp.h"
#include "event/event.h"
#include "event/event_handler.h"
#include "net/if_socket.h"

namespace eznetpp {
namespace event {
class event_dispatcher {
 public:
  static event_dispatcher& instance() {
    if (_evt_dispatcher == nullptr)
      _evt_dispatcher = new event_dispatcher();

    return *_evt_dispatcher;
  }

  int init(void);

  bool register_socket_event_handler(eznetpp::net::if_socket* sock
      , event_handler* handler);
  bool deregister_socket_event_handler(eznetpp::net::if_socket* sock);
  
  void push_event(io_event* evt);

 protected:
  void dispatch_loop(void);

 private:
  event_dispatcher(void);
  virtual ~event_dispatcher(void);

  void process_event(io_event* evt);
  void clear_resources(eznetpp::net::if_socket* sock
      , event_handler* handler);

  static event_dispatcher* _evt_dispatcher;

  // work threads
  std::thread _disp_th;

  // socket<key>, socket_class<value>
  std::map<eznetpp::net::if_socket*, event_handler*> _handlers_map;
  std::mutex _handlers_map_mutex;

  std::vector<eznetpp::net::if_socket*> _sockets_vec;
  std::mutex _sockets_vec_mutex;

  std::vector<io_event*> _ioevents_vec;
  std::mutex _ioevents_vec_mutex;

  // condition variables
  std::condition_variable _disp_th_cv;  
  std::mutex _disp_th_cv_mutex;

  DISALLOW_COPY_AND_ASSIGN(event_dispatcher);
};
}  // namespace event
}  // namespace eznetpp

#endif  // INCLUDE_EVENT_DISPATCHER_H_
