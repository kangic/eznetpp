// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_TCP_SERVER_H_
#define INCLUDE_TCP_SERVER_H_

#include "eznetpp.h"
#include "event/event_handler.h"
#include "net/tcp/tcp_acceptor.h"
#include "sys/io_manager.h"

namespace eznetpp {
namespace net {
namespace tcp {
class tcp_server : public eznetpp::event::acceptor_event_handler {
 public:
  tcp_server();
  virtual ~tcp_server();

  int open(eznetpp::sys::io_manager& io_mgr
      , int port, int backlog = 5);

  eznetpp::sys::io_manager& get_io_mgr() { return _io_mgr; };

 private:
  eznetpp::sys::io_manager& _io_mgr;
  tcp_acceptor _acceptor;

  DISALLOW_COPY_AND_ASSIGN(tcp_server);
};

}  // namespace tcp
}  // namespace net
}  // namespace eznetpp

#endif  // INCLUDE_TCP_SERVER_H_
