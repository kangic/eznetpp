//  Copyright [2015] <kangic21@gmail.com>

#include "tcp_socket.h"
#include "event/event.h"
#include "event/event_dispatcher.h"

#include <cstring>

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

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
  server_addr.sin_port = htons(port);

  // connect to server(on_connect event)
  int ret = ::connect(_sd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  eznetpp::event::event_dispatcher::instance().push_event(
      new eznetpp::event::io_event(eznetpp::event::event_type::connect
        , ret, errno, ip, port, this));

  return 0;
}

void tcp_socket::send(const std::string& msg, int len) {
  if (_sd == -1)
    return;

  //int ret = ::send(_sd, msg.c_str(), len, MSG_NOSIGNAL);
  int ret = ::send(_sd, msg.c_str(), len, 0);
  if (ret == -1) {
    close();
  } else if (ret > 0) {
    eznetpp::event::event_dispatcher::instance().push_event(
        new eznetpp::event::io_event(eznetpp::event::event_type::tcp_send
          , ret, errno, this));
  }
}

void tcp_socket::recv(void) {
  if (_sd == -1)
    return;

  std::string data(eznetpp::opt::max_transfer_bytes, '\0');
  //int len = ::recv(_sd, &data[0], eznetpp::opt::max_transfer_bytes, MSG_NOSIGNAL);
  int len = ::recv(_sd, &data[0], eznetpp::opt::max_transfer_bytes, 0);
  if (len == 0) {
    close();
  } else if (len == -1) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
        , __FILE__, __FUNCTION__, __LINE__
        , "recv() error(%d)", errno);
    close();
  } else if (len > 0) {
    eznetpp::event::event_dispatcher::instance().push_event(
        new eznetpp::event::io_event(eznetpp::event::event_type::tcp_recv, len
          , errno, std::move(data), 0, this));
  }
}

void tcp_socket::close(void) {
  int ret = ::close(_sd);
  _sd = -1;

  eznetpp::event::event_dispatcher::instance().push_event(
      new eznetpp::event::io_event(eznetpp::event::event_type::close, ret, errno, this));
}

}  // namespace tcp
}  // namespace net
}  // namespace eznetpp
