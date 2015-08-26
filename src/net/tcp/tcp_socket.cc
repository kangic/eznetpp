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
  if (_sd == -1) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "socket() error(%d)", errno);
  }
}

tcp_socket::tcp_socket(int sd) {
  _sock_domain = socket_domain::inet_v4;
  _sock_type = socket_type::tcp;
  _sd = sd;
}

tcp_socket::~tcp_socket(void) {
}

/* move to tcp_connector
int tcp_socket::connect(const std::string& ip, int port) {
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
  server_addr.sin_port = htons(port);

  // connect to server(on_connect event)
  int ret = ::connect(_sd, (struct sockaddr *)&server_addr
                    , sizeof(server_addr));
  if (ret == -1) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "::connect() error(%d)", errno);

    this->close();
    return errno;
  }

  _peer.ip = ip;
  _peer.port = port;

  return 0;
}
*/

void tcp_socket::send(const std::string& msg, int len) {
  eznetpp::event::event_dispatcher::instance().push_event(
      new eznetpp::event::io_event(eznetpp::event::event_type::send, msg, len, this));
}

void tcp_socket::recv(void) {
  eznetpp::event::event_dispatcher::instance().push_event(
      new eznetpp::event::io_event(eznetpp::event::event_type::recv, this));
}

void tcp_socket::close(void) {
  eznetpp::event::event_dispatcher::instance().push_event(
      new eznetpp::event::io_event(eznetpp::event::event_type::close, this));
}

}  // namespace tcp
}  // namespace net
}  // namespace eznetpp
