//  Copyright [2015] <kangic21@gmail.com>

#include "if_socket.h"

#include "sys/io_manager.h"

namespace eznetpp {
namespace net {

if_socket::if_socket(void) {
}

if_socket::~if_socket(void) {
}

if_socket::socket_domain if_socket::domain(void) {
  return _sock_domain;
}

if_socket::socket_type if_socket::type(void) {
  return _sock_type;
}

void if_socket::set_peer_info(const std::string& ip, int port) {
  _peer.ip = ip;
  _peer.port = port;
}

// set socket options
int if_socket::set_nonblocking(void) {
  if (_sd == -1)
    return -1;

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


int if_socket::send_bytes(const std::string& data) {
  struct epoll_event ev;

  ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
  ev.data.ptr = this;

  int ret = epoll_ctl(eznetpp::sys::io_manager::_epoll_fd
      , EPOLL_CTL_MOD, _sd, &ev);

  if (ret)
    return errno;

  _sendmsgs_vec.emplace_back(data); 


  return 0;
}

}  // namespace net
}  // namespace eznetpp
