// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_EVENT_DISPATCHER_H_
#define INCLUDE_EVENT_DISPATCHER_H_

#include <eznetpp.h>

namespace eznetpp {
class event_handler;
class event_dispatcher {
 public:
  event_dispatcher(void);
  virtual ~event_dispatcher(void);

 public:
  void init(void);

  bool register_handler(event_handler* handler);
  bool deregister_handler(event_handler* handler);

 private:
  DISALLOW_COPY_AND_ASSIGN(event_dispatcher);
};
}  // namespace eznetpp

#endif INCLUDE_EVENT_DISPATCHER_H_
