#ifndef _EZNETPP_SRC_EVENT_DISPATCHER_H_
#define _EZNETPP_SRC_EVENT_DISPATCHER_H_

#include <vector>

#include "./macros.h"

namespace eznetpp {
class if_event_listener;
  
class event_dispatcher {
 public:
  event_dispatcher(void);
  virtual ~event_dispatcher(void);
  
 public:
  bool reg_event_listener(eznetpp::if_event_listener* listener);
  bool unreg_event_listener(eznetpp::if_event_listener* listener);

  void dispatch();

 private:
  std::vector<eznetpp::if_event_listener*> listener_container_;
  
 private:
  DISALLOW_COPY_AND_ASSIGN(event_dispatcher);
};
  
}  // namespace eznetpp

#endif  // _EZNETPP_SRC_EVENT_DISPATCHER_H_
