// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_IF_SERVER_H_
#define INCLUDE_IF_SERVER_H_

#include <string>

#include "./macros.h"
#include "./connection.h"

namespace eznetpp {
class if_server {
 public:
  virtual ~if_server() {}

 public:
  // override
  virtual void on_accept(connection* conn, int err_no) = 0;

  virtual int start_async_io() = 0;
};
}  // namespace eznetpp

#endif  // INCLUDE_IF_SERVER_H_
