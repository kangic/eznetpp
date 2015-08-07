//  Copyright [2015] <kangic21@gmail.com>

#include "socket.h"

namespace eznetpp {
namespace net {

socket::socket(void) {

}

socket::socket(int sd)
: _sd(sd) {
}

socket::~socket(void) {

}

socket::socket_domain socket::domain(void) {
  return _sock_domain;
}

socket::socket_type socket::type(void) {
  return _sock_type;
}

////////////////////////////////////////////////////////////////////////////////
// set socket options
int socket::set_nonblock(bool flag) {
  if (_sd == -1)
    return _sd;

  int flags = fcntl(_sd, F_GETFL);

  if (flag)
    flags |= O_NONBLOCK;
  else
    flags |= ~O_NONBLOCK;

  return fcntl(_sd, F_SETFL, flags);
}

int socket::set_tcpnodelay(bool flag) {
  if (_sd == -1)
    return _sd;

  return setsockopt(_sd, IPPROTO_TCP, TCP_NODELAY
                    , reinterpret_cast<char*>(&flag), sizeof(int));
}

int socket::set_reuseaddr(bool flag) {
  if (_sd == -1)
    return _sd;

  return setsockopt(_sd, SOL_SOCKET, SO_REUSEADDR
                    , reinterpret_cast<char*>(&flag), sizeof(int));
}

}  // namespace net
}  // namespace eznetpp
