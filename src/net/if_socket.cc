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

int if_socket::set_epollout_flag(bool flag) {
  struct epoll_event ev;

  ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
  if(flag)
    ev.events |= EPOLLOUT;
  ev.data.ptr = this;

  return epoll_ctl(eznetpp::sys::io_manager::_epoll_fd, EPOLL_CTL_MOD, _sd, &ev);
}

int if_socket::send_bytes(const std::string& data, const std::string& ip, int port) {
  {
    std::lock_guard<std::mutex> lock(_sendmsgs_mtx);

    //_sendmsgs_vec.emplace_back(data); 
    peer_addr peer_info;
    peer_info.ip = ip;
    peer_info.port = port;
    _sendmsgs_vec.emplace_back(std::make_pair(std::move(data), std::move(peer_info))); 
    if (set_epollout_flag(true) == -1) {
      eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
          , __FILE__, __FUNCTION__, __LINE__
          , "epoll_ctl() error(%d)", errno);
      _sendmsgs_vec.pop_back();
      return errno;
    }

  }

  return 0;
}

}  // namespace net
}  // namespace eznetpp
