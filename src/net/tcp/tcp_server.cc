//  Copyright [2015] <kangic21@gmail.com>

#include "tcp_server.h"

#include "tcp_connection.h"

namespace eznetpp {
namespace net {
namespace tcp {

tcp_server::tcp_server(void) {
}

tcp_server::~tcp_server(void) {
}

int tcp_server::open(std::shared_ptr<eznetpp::sys::io_manager> io_mgr
    , int port, int backlog) {
  _io_mgr = io_mgr;
  _acceptor.open(port, backlog);

  _io_mgr.add_socket(&_acceptor, this);
}

}  // namespace tcp
}  // namespace net
}  // namespace eznetpp
