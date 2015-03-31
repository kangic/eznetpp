// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_IF_SERVER_H_
#define INCLUDE_IF_SERVER_H_

#include <string>

#include "./macros.h"

namespace eznetpp {
class dummy_connection;
class event_dispatcher;
class if_server {
 public:
  virtual ~if_server() {}

 public:
  virtual void on_accept(const dummy_connection& conn, int err_no) = 0;
  virtual void on_read(const dummy_connection& conn, const std::string& msg
      , int len, int err_no) = 0;
  virtual void on_write(const dummy_connection& conn, unsigned int len
      , int err_no) = 0;
  virtual void on_close(const dummy_connection& conn, int err_no) = 0;

  virtual int start_async_io() = 0;

  void set_evt_dispatcher(event_dispatcher* evt_disp) {
    evt_dispatcher = evt_disp;
  }

 protected:
  event_dispatcher* evt_dispatcher;
};
}  // namespace eznetpp

#endif  // INCLUDE_IF_SERVER_H_
