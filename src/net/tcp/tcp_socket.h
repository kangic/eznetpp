// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_TCP_SOCKET_H_
#define INCLUDE_TCP_SOCKET_H_

#include "eznetpp.h"
#include "net/socket.h"

namespace eznetpp {
namespace net {
namespace tcp {
class tcp_socket : public eznetpp::net::socket {
 public:
  tcp_socket(void);
  tcp_socket(int sd);
  virtual ~tcp_socket(void);

  int connect(const std::string& ip, int port);
  void send(const std::string& msg, int len);
  void recv(void); 
  void close(void);
};

}  // namespace tcp
}  // namespace net
}  // namespace eznetpp

#endif  // INCLUDE_TCP_SOCKET_H_
