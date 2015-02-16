#ifndef _EZNETPP_SRC_IF_EVENT_LISTENER_H_
#define _EZNETPP_SRC_IF_EVENT_LISTENER_H_

#include "macros.h"

namespace eznetpp {
class if_event_listener {
 public:
  virtual ~if_event_listener() {}
  
 public:
  virtual void handle_accept() = 0;
  virtual void handle_read() = 0;
  virtual void handle_write() = 0;
};
  
}  // namespace eznetpp

#endif  // _EZNETPP_SRC_IF_EVENT_LISTENER_H_
