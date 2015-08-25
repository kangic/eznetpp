// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_IO_EVENT_H_
#define INCLUDE_IO_EVENT_H_

#include "eznetpp.h"

namespace eznetpp {
namespace event {

  enum event_type { 
    on_close = 0,
    on_accept,
    on_connect,
    on_recv,
    on_send,
  };

class io_event {
 public:
  io_event(void) = delete;
  io_event(event_type type, int result, int err_code, const char* data, void* publisher) {
   _type = type;
   _result = result;
   _err_code = err_code;
   _data = (char*)data;
   _publisher = publisher;
  };
  virtual ~io_event(void) = default; 

  event_type type(void) { return _type; };
  int result(void) { return _result; };
  int err_code(void) { return _err_code; };
  const char* data(void) { return _data; };
  void* publisher(void) { return _publisher; };

 private:
  event_type _type;
  int _result;
  int _err_code;
  char* _data;
  void* _publisher;
};

}  // namespace event
}  // namespace eznetpp

#endif  // INCLUDE_IO_EVENT_H_
