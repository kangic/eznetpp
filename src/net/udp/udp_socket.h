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
  udp_socket(int sd);
  virtual ~udp_socket(void);

  int connect(const std::string& ip, int port);
  void send(const std::string& msg, int len);
  void close(void);

 protected:
  void recv(void); 
};

}  // namespace udp
}  // namespace net
}  // namespace eznetpp

#endif  // INCLUDE_UDP_SOCKET_H_
