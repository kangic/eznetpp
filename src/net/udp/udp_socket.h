// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_UDP_SOCKET_H_
#define INCLUDE_UDP_SOCKET_H_

#include "eznetpp.h"
#include "net/if_socket.h"

namespace eznetpp {
namespace net {
namespace udp {
class udp_socket : public eznetpp::net::if_socket {
 public:
  udp_socket(void);
  virtual ~udp_socket(void);

  int open(int port);
  void close(void);

 protected:
  void recv(void); 
  void send(void);

 private:
  int bind(int port);
};

}  // namespace udp
}  // namespace net
}  // namespace eznetpp

#endif  // INCLUDE_UDP_SOCKET_H_
