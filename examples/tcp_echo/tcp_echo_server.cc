#include "./tcp_echo_server.h"

#include <cstdio>

#include "./tcp_echosvr_session.h"

tcp_echo_server::tcp_echo_server(eznetpp::sys::io_manager* io_mgr)
: _io_mgr(io_mgr) {
}

tcp_echo_server::~tcp_echo_server(void) {
}

int tcp_echo_server::open(int port, int backlog) {
  int ret = _acceptor.open(port, backlog);

  if (ret) {
    printf("%s\n", eznetpp::errcode::errno_to_str(ret).c_str());
    return -1;
  }
 
  _io_mgr->register_socket_event_handler(&_acceptor, this);

  return 0;
}

void tcp_echo_server::on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no) {
  //printf("received event - on_accept() : socket id %d(%p), connections : %ld\n", sock.descriptor(), &sock, _conns);
  printf("on_accept() : client ip %s, port %d\n", sock->peer().ip.c_str(), sock->peer().port);
  _io_mgr->register_socket_event_handler(sock, new tcp_echosvr_session(sock));
  ++_conns;
  if(_conns % 1000 == 0)
    printf("connection count = %ld\n", _conns);
}

void tcp_echo_server::on_close(int err_no) {

}
