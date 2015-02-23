#ifndef _EZNETPP_SRC_IF_EVENT_LISTENER_H_
#define _EZNETPP_SRC_IF_EVENT_LISTENER_H_

#include <string>

#include "macros.h"

namespace eznetpp {
class dummy_connection;
class if_event_listener {
public:
  virtual ~if_event_listener() {}
  
public:
  virtual void on_accept(const dummy_connection& conn) {}
  virtual void on_read(const dummy_connection& conn, const std::string& msg
                     , int len) {}
  virtual void on_write(const dummy_connection& conn, unsigned int len) {}
};
  
}  // namespace eznetpp

#endif  // _EZNETPP_SRC_IF_EVENT_LISTENER_H_
