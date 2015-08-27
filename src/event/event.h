// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_IO_EVENT_H_
#define INCLUDE_IO_EVENT_H_

#include "eznetpp.h"
#include "net/socket.h"

namespace eznetpp {
namespace event {

  enum event_type { 
    close = 0,
    accept,
    connect,
    recv,
    send,
  };

class io_event {
 public:
  io_event(void) = delete;

  io_event(event_type type, eznetpp::net::socket* publisher) {
   _type = type;
   _publisher = publisher;
  };

  io_event(event_type type, const std::string& data, int opt_data, eznetpp::net::socket* publisher) {
   _type = type;
   _data = data;
   _opt_data = opt_data;
   _publisher = publisher;
  };

  virtual ~io_event(void) {
  }

  event_type type(void) { return _type; };
  const std::string& data(void) { return _data; };
  int opt_data(void) { return _opt_data; };
  eznetpp::net::socket* publisher(void) { return _publisher; };

 private:
  event_type _type = event_type::close;
  std::string _data = "";
  int _opt_data = 0;
  eznetpp::net::socket* _publisher;
};

}  // namespace event
}  // namespace eznetpp

#endif  // INCLUDE_IO_EVENT_H_
