// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_IF_SERVER_H_
#define INCLUDE_IF_SERVER_H_

#include <string>

#include "./macros.h"

namespace eznetpp {
class dummy_connection;
class if_server {
 public:
  virtual ~if_server() {}

 public:
  virtual void on_accept(const dummy_connection& conn) = 0;
  virtual void on_read(const dummy_connection& conn, const std::string& msg
                     , int len) = 0;
  virtual void on_write(const dummy_connection& conn, unsigned int len) = 0;

  virtual int start_async_io() = 0;
};
}  // namespace eznetpp

#endif  // INCLUDE_IF_SERVER_H_
