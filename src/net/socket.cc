//  Copyright [2015] <kangic21@gmail.com>

#include "socket.h"

namespace eznetpp {
namespace net {

////////////////////////////////////////////////////////////////////////////////
// set socket options
socket::socket_type socket::type(void) {
  return _sock_type;
}

int socket::set_nonblock(bool flag) {
  if (_sock_fd == -1)
    return _sock_fd;

  int flags = fcntl(_sock_fd, F_GETFL);

  if (flag)
    flags |= O_NONBLOCK;
  else
    flags |= ~O_NONBLOCK;

  return fcntl(_sock_fd, F_SETFL, flags);
}

int socket::set_tcpnodelay(bool flag) {
  if (_sock_fd == -1)
    return _sock_fd;

  return setsockopt(_sock_fd, IPPROTO_TCP, TCP_NODELAY
                    , reinterpret_cast<char*>(&flag), sizeof(int));
}

int socket::set_reuseaddr(bool flag) {
  if (_sock_fd == -1)
    return _sock_fd;

  return setsockopt(_sock_fd, SOL_SOCKET, SO_REUSEADDR
                    , reinterpret_cast<char*>(&flag), sizeof(int));
}

}  // namespace net
}  // namespace eznetpp
