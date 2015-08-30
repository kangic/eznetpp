#include "./echo_server.h"

#include <cstdio>

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

void echo_server::on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no) {
  //printf("received event - on_accept() : socket id %d(%p), connections : %ld\n", sock.descriptor(), &sock, _conns);
  printf("on_accept() : client ip %s, port %d\n", sock->peer().ip.c_str(), sock->peer().port);
  _io_mgr->register_socket_event_handler(sock, new echosvr_session(sock));
  ++_conns;
  if(_conns % 1000 == 0)
    printf("connection count = %ld\n", _conns);
}

void echo_server::on_close(int err_no) {

}
