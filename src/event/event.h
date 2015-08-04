// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_EVENT_H_
#define INCLUDE_EVENT_H_

#include <eznetpp.h>

namespace eznetpp {
namespace event {
class event {
 public:
  event(void);
  virtual ~event(void);

  enum event_type : { on_accept = 0x01, on_recv = 0x02, on_send = 0x04, on_close = 0x08};

  void set_event(event_type type, int result, void* data);

  event_type type();
  int result();
  void* data();

 private:
  DISALLOW_COPY_AND_ASSIGN(event);
};
}  // namespace event
}  // namespace eznetpp

#endif INCLUDE_EVENT_H_
