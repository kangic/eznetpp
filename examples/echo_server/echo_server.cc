#include "./echo_server.h"

#include <cstdio>
#include <memory>

echo_server::echo_server(eznetpp::sys::io_manager* io_mgr)
: _io_mgr(io_mgr) {
}

echo_server::~echo_server(void) {
}

int echo_server::open(int port, int backlog) {
  _acceptor.open(port, backlog);

  _io_mgr->register_socket_event_handler(&_acceptor, this);
}


void echo_server::on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no) {
  /*
  eznetpp::connection* conn = new echosvr_connection();
  conn->socket_id(fd);
  add_to_connection_list(conn);
  */

  printf("received event - on_accept() : socket id %d\n", sock->descriptor());
}

void echo_server::on_close(int err_no) {

}
