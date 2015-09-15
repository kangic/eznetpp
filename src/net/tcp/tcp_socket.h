// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_TCP_SOCKET_H_
#define INCLUDE_TCP_SOCKET_H_

#include "eznetpp.h"
#include "net/if_socket.h"

namespace eznetpp {
namespace net {
namespace tcp {
class tcp_socket : public eznetpp::net::if_socket {
 public:
  tcp_socket(void);
  tcp_socket(int sd);
  virtual ~tcp_socket(void);

  int connect(const std::string& ip, int port);
  void close(void);

 protected:
  void recv(void); 
  void send(void);

 private:
  long long rc = 0;
  long long sc = 0;
};

}  // namespace tcp
}  // namespace net
}  // namespace eznetpp

#endif  // INCLUDE_TCP_SOCKET_H_
