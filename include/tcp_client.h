// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_TCP_CLIENT_H_
#define INCLUDE_TCP_CLIENT_H_

#include <eznetpp.h>

namespace eznetpp {
class tcp_client {
 public:
  tcp_client();
  virtual ~tcp_client();

  // override
  virtual void on_connect(int client_id) = 0;

  int start_async_io();

 private:
  DISALLOW_COPY_AND_ASSIGN(tcp_client);
};
}  // namespace eznetpp

#endif  // INCLUDE_TCP_CLIENT_H_
