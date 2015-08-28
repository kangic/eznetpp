//  Copyright [2015] <kangic21@gmail.com>

#include "if_socket.h"

namespace eznetpp {
namespace net {

if_socket::if_socket(void) {
}

if_socket::~if_socket(void) {
}

if_socket::socket_domain socket::domain(void) {
  return _sock_domain;
}

if_socket::socket_type socket::type(void) {
  return _sock_type;
}

////////////////////////////////////////////////////////////////////////////////
// set if_socket options
int if_socket::set_nonblocking(void) {
  if (_sd == -1)
    return _sd;

  int flags = fcntl(_sd, F_GETFL);

  flags |= O_NONBLOCK;

  return fcntl(_sd, F_SETFL, flags);
}

int if_socket::set_tcpnodelay(void) {
  if (_sd == -1)
    return _sd;

  int on = 1;
  return setsockopt(_sd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(int));
}

int if_socket::set_reuseaddr(void) {
  if (_sd == -1)
    return _sd;

  int on = 1;
  return setsockopt(_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
}

void if_socket::set_peer_info(const std::string& ip, int port) {
  _peer.ip = ip;
  _peer.port = port;
}

}  // namespace net
}  // namespace eznetpp
