// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_IO_EVENT_H_
#define INCLUDE_IO_EVENT_H_

#include "eznetpp.h"
#include "net/if_socket.h"

namespace eznetpp {
namespace event {

  enum event_type { 
    close = 0,
    tcp_accept,
    tcp_connect,
    tcp_recv,
    tcp_send,
    udp_recv,
    udp_send,
  };

class io_event {
 public:
  io_event(void) = delete;

  io_event(event_type type, int err_no, eznetpp::net::if_socket* publisher) {
   _type = type;
   _err_no = err_no;
   _publisher = publisher;
  }

  io_event(event_type type, int result, int err_no
      ,eznetpp::net::if_socket* publisher) {
   _type = type;
   _result = result;
   _err_no = err_no;
   _publisher = publisher;
  }

  io_event(event_type type, int result, int err_no, const std::string& data
      ,int opt_data, eznetpp::net::if_socket* publisher) {
   _type = type;
   _result = result;
   _err_no = err_no;
   _data = data;
   _opt_data = opt_data;
   _publisher = publisher;
  }

  virtual ~io_event(void) = default;

  event_type type(void) { return _type; }
  int result(void) { return _result; }
  int err_no(void) { return _err_no; }
  const std::string& data(void) { return _data; }
  int opt_data(void) { return _opt_data; }
  eznetpp::net::if_socket* publisher(void) { return _publisher; }
  
  // is work done?
  bool is_done() { return _done; }
  void done() { _done = true; }

 private:
  event_type _type = event_type::close;
  int _result = 0;
  int _err_no = 0;
  std::string _data = "";
  int _opt_data = 0;
  eznetpp::net::if_socket* _publisher = nullptr;
  bool _done = false;
};

}  // namespace event
}  // namespace eznetpp

#endif  // INCLUDE_IO_EVENT_H_
