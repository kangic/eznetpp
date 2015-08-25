//  Copyright [2015] <kangic21@gmail.com>

#include "tcp_acceptor.h"

#include "event/event.h"
#include "event/event_dispatcher.h"

namespace eznetpp {
namespace net {
namespace tcp {

tcp_acceptor::tcp_acceptor(void) {
}

tcp_acceptor::~tcp_acceptor(void) {
}

int tcp_acceptor::open(int port, int backlog) {
  set_nonblocking(true);
  set_reuseaddr(true);

  return this->bind_and_listen(port, backlog);
}

void tcp_acceptor::read_operation() {
  int sock_fd = this->accept();

  eznetpp::event::io_event* evt = new eznetpp::event::io_event
      (eznetpp::event::event_type::on_accept
       , sock_fd, errno, "", this);

  eznetpp::event::event_dispatcher::instance().push_event(evt);
}

int tcp_acceptor::bind_and_listen(int port, int backlog) {
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
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

  ret = listen(_sd, backlog);

  if (ret != 0) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "::listen() error(%d)", errno);

    this->close();
    return errno;
  }

  return 0;
}

int tcp_acceptor::accept(void) {
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


  return client_sock;
}


}  // namespace tcp
}  // namespace net
}  // namespace eznetpp
