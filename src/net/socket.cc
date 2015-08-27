//  Copyright [2015] <kangic21@gmail.com>

#include "socket.h"

namespace eznetpp {
namespace net {

socket::socket(void) {
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
int socket::set_nonblocking(void) {
  if (_sd == -1)
    return _sd;

  int flags = fcntl(_sd, F_GETFL);

  flags |= O_NONBLOCK;

  return fcntl(_sd, F_SETFL, flags);
}

int socket::set_tcpnodelay(void) {
  if (_sd == -1)
    return _sd;

  int on = 1;
  return setsockopt(_sd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(int));
}

int socket::set_reuseaddr(void) {
  if (_sd == -1)
    return _sd;

  int on = 1;
  return setsockopt(_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
}

void socket::set_peerinfo(const std::string& ip, int port) {
  _peer.ip = ip;
  _peer.port = port;
}

}  // namespace net
}  // namespace eznetpp
