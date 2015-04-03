// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_IF_SERVER_H_
#define INCLUDE_IF_SERVER_H_

#include <string>

#include "./macros.h"

namespace eznetpp {
class connection;
class event_dispatcher;
class if_server {
 public:
  virtual ~if_server() {}

 public:
  virtual void on_accept(const connection& conn, int err_no) = 0;
  virtual void on_read(const connection& conn, const std::string& msg
      , int len, int err_no) = 0;
  virtual void on_write(const connection& conn, unsigned int len
      , int err_no) = 0;
  virtual void on_close(const connection& conn, int err_no) = 0;

  virtual int start_async_io() = 0;
};
}  // namespace eznetpp

#endif  // INCLUDE_IF_SERVER_H_
