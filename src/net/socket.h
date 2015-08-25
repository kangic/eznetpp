// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_SOCKET_H_
#define INCLUDE_SOCKET_H_

#include "eznetpp.h"

/*
 * Socket Interface Class
 */
namespace eznetpp {
namespace net {
class socket {
 public:
  socket(void);
  virtual ~socket(void);

  enum socket_domain { inet_v4 = PF_INET, inet_v6 = PF_INET6, unix = PF_LOCAL };
  enum socket_type { tcp = SOCK_STREAM, udp = SOCK_DGRAM };

  typedef struct _peer_addr {
    std::string ip = "";
    int port = -1;
  } peer_addr;

  // socket descriptor
  int descriptor(void) const { return _sd; }

  socket_domain domain(void);
  socket_type type(void);

  int set_nonblocking(bool flag);
  int set_tcpnodelay(bool flag);
  int set_reuseaddr(bool flag);

  int close(void);

  /*
   * This function will implemented by each inherited class.(acceptor, connector)
   */
  virtual void read_operation() = 0;

  socket_domain _sock_domain = socket_domain::inet_v4;
  socket_type _sock_type = socket_type::tcp;
  int _sd = -1;
  peer_addr _peer;
};

}  // namespace net
}  // namespace eznetpp

#endif  // INCLUDE_SOCKET_H_
