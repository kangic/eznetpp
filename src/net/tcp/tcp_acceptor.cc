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
  int ret = this->bind_and_listen(port, backlog);

  if (ret != 0)
    return ret;

  set_nonblocking();
  set_reuseaddr();

  return 0;
}

void tcp_acceptor::recv(void) {
  printf("tcp_acceptor - recv\n");
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  int sock_fd = ::accept(_sd, (struct sockaddr *)&client_addr, &client_addr_len);

  if (sock_fd == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return;
    }
  }

  eznetpp::event::event_dispatcher::instance().push_event(new eznetpp::event::io_event
      (eznetpp::event::event_type::accept, sock_fd, errno, inet_ntoa(client_addr.sin_addr), client_addr.sin_port, this));
}

void tcp_acceptor::close(void) {
  ::close(_sd);

  eznetpp::event::event_dispatcher::instance().push_event(
      new eznetpp::event::io_event(eznetpp::event::event_type::close, errno, this));
}

int tcp_acceptor::bind_and_listen(int port, int backlog) {
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  server_addr.sin_port = htons(port);

  int ret = bind(_sd, (struct sockaddr *)&server_addr, sizeof(server_addr));

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


}  // namespace tcp
}  // namespace net
}  // namespace eznetpp
