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
    return -1;

  set_nonblocking();
  set_reuseaddr();

  return 0;
}

void tcp_acceptor::recv(void) {
  eznetpp::event::event_dispatcher::instance().push_event(new eznetpp::event::io_event
      (eznetpp::event::event_type::accept, this));
}

void tcp_acceptor::close(void) {
  eznetpp::event::event_dispatcher::instance().push_event(
      new eznetpp::event::io_event(eznetpp::event::event_type::close, this));
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
