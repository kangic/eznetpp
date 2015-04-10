// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_EVENT_H_
#define INCLUDE_EVENT_H_

#include <string>

#include "./macros.h"

namespace eznetpp {
class event {
 public:
  // event type
  typedef enum _event_type_ {
    on_accept = 0,
    on_read,
    on_write,
    on_close,
  } event_type;

  event();
  virtual ~event();

  void set(event_type type, const std::string& data, int data_len, int err_no);

  event_type type();
  const std::string& data();
  int data_length();
  int err_no();

 private:
  event_type _type;
  std::string _data;
  int _data_len;
  int _err_no;

  DISALLOW_COPY_AND_ASSIGN(event);
};
}  // namespace eznetpp

#endif  // INCLUDE_EVENT_H_
