// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_EVENT_DISPATCHER_H_
#define INCLUDE_EVENT_DISPATCHER_H_

#include <eznetpp.h>

namespace eznetpp {
namespace event {
class event_handler;
class event_dispatcher {
 public:
  static event_dispatcher& instance() {
    if (_evt_dispatcher == nullptr)
      _evt_dispatcher = new event_dispatcher();

    return *_evt_dispatcher;
  }

 private:
  event_dispatcher(void);
  virtual ~event_dispatcher(void);

 public:
  void init(void);

  bool register_handler(event_handler* handler);
  bool deregister_handler(event_handler* handler);

 protected:
  void* loop();

 private:
  static event_dispatcher* _evt_dispatcher = null;

  std::thread _dispatch_th;

  DISALLOW_COPY_AND_ASSIGN(event_dispatcher);
};
}  // namespace event
}  // namespace eznetpp

#endif INCLUDE_EVENT_DISPATCHER_H_
