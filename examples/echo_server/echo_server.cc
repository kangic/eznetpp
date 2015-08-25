#include "./echo_server.h"

#include <cstdio>
#include <memory>

#include "./echosvr_session.h"

echo_server::echo_server(eznetpp::sys::io_manager* io_mgr)
: _io_mgr(io_mgr) {
}

echo_server::~echo_server(void) {
}

int echo_server::open(int port, int backlog) {
  _acceptor.open(port, backlog);

  _io_mgr->register_socket_event_handler(&_acceptor, this);
}


void echo_server::on_accept(eznetpp::net::tcp::tcp_socket& sock, int err_no) {
  printf("received event - on_accept() : socket id %d\n", sock.descriptor());
  _io_mgr->register_socket_event_handler(&sock, new echosvr_session(sock));
}

void echo_server::on_close(int err_no) {

}
