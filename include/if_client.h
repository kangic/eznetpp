// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_IF_CLIENT_H_
#define INCLUDE_IF_CLIENT_H_

#include <string>

#include "./eznetpp.h"

namespace eznetpp {
class if_client {
 public:
  virtual ~if_client() {}

 public:
  // override
  virtual void on_connect(int client_id) = 0;

  virtual int start_async_io() = 0;
};
}  // namespace eznetpp

#endif  // INCLUDE_IF_CLIENT_H_
