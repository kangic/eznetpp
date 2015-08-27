//  Copyright [2015] <kangic21@gmail.com>

#include "tcp_socket.h"
#include "event/event.h"
#include "event/event_dispatcher.h"

namespace eznetpp {
namespace net {
namespace tcp {

tcp_socket::tcp_socket(void) {
  _sock_domain = socket_domain::inet_v4;
  _sock_type = socket_type::tcp;

  // TODO : implement for PF_INET6
  _sd = ::socket(_sock_domain, _sock_type, 0);
}

tcp_socket::tcp_socket(int sd) {
  _sock_domain = socket_domain::inet_v4;
  _sock_type = socket_type::tcp;
  _sd = sd;
}

tcp_socket::~tcp_socket(void) {
}

int tcp_socket::connect(const std::string& ip, int port) {
  if (_sd == -1) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "socket() error(%d)", errno);
    return -1;
  }

  eznetpp::event::event_dispatcher::instance().push_event(
      new eznetpp::event::io_event(eznetpp::event::event_type::connect, ip, port, this));

  return 0;
}

void tcp_socket::send(const std::string& msg, int len) {
  eznetpp::event::event_dispatcher::instance().push_event(
      new eznetpp::event::io_event(eznetpp::event::event_type::tcp_send, msg, len, this));
}

void tcp_socket::recv(void) {
  eznetpp::event::event_dispatcher::instance().push_event(
      new eznetpp::event::io_event(eznetpp::event::event_type::tcp_recv, this));
}

void tcp_socket::close(void) {
  eznetpp::event::event_dispatcher::instance().push_event(
      new eznetpp::event::io_event(eznetpp::event::event_type::close, this));
}

}  // namespace tcp
}  // namespace net
}  // namespace eznetpp
