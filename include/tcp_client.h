// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_TCP_CLIENT_H_
#define INCLUDE_TCP_CLIENT_H_

#include <sys/epoll.h>

#include <thread>
#include <mutex>

namespace eznetpp {
class tcp_client {
 public:
  tcp_client();
  virtual ~tcp_client();

 private:
  DISALLOW_COPY_AND_ASSIGN(tcp_server);
};
}  // namespace eznetpp

#endif  // INCLUDE_TCP_CLIENT_H_
