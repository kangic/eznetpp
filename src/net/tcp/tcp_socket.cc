//  Copyright [2015] <kangic21@gmail.com>

#include "tcp_socket.h"

namespace eznetpp {
namespace net {
namespace tcp {


tcp_socket::tcp_socket(void) {
  _sock_domain = socket_domain::inet_v4;
  _sock_type = socket_type::tcp;

  // TODO(kangic) : PF_INET6
  _sd = ::socket(_sock_domain, _sock_type, 0);
  if (_sd == -1) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "socket() error(%d)", errno);
  }

}

tcp_socket::~tcp_socket(void) {

}

int tcp_socket::bind_and_listen(const char* ip, int port, int max_accept_cnt) {
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip); 
  server_addr.sin_port = htons(port);

  int ret;
  ret = bind(_sd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  if (ret != 0) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "::bind() error(%d)", errno);

    this->close();
    return errno;
  }

  ret = listen(_sd, max_accept_cnt);

  if (ret != 0) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "::listen() error(%d)", errno);

    this->close();
    return errno;
  }

  return 0;
}

int tcp_socket::accept(void) {
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  // todo : wait to accept a client connection
  int client_sock = ::accept(_sd, (struct sockaddr *)&client_addr
                          , &client_addr_len);

  if (client_sock < 0) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "::accept() error(%d)", errno);

    return -1;
  }

  eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "client socket id : %d", client_sock);


  return 0;
}

int tcp_socket::connect(const char* ip, int port) {
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip);
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

int tcp_socket::send(const char* msg, int len) {
  return ::send(_sd, msg, len, MSG_NOSIGNAL);
}

int tcp_socket::recv(char* msg, int len) {
 return ::recv(_sd, msg, len, MSG_NOSIGNAL);
}

int tcp_socket::close(void) {
  int ret = ::close(_sd);
  _sd = -1;
  return ret;
}


}  // namespace tcp
}  // namespace net
}  // namespace eznetpp


