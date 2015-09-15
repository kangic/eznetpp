#include "./tcp_echo_client.h"

#include <cstdio>

tcp_echo_client::tcp_echo_client(eznetpp::sys::io_manager* io_mgr)
: _io_mgr(io_mgr){
}

tcp_echo_client::~tcp_echo_client(void) {
}

void tcp_echo_client::start(void) {
  _io_mgr->register_socket_event_handler(&_socket, this);

  _socket.connect("127.0.0.1", 6666);
}

void tcp_echo_client::on_connect(int err_no) {
  printf("on_connect\n");
  std::string hellomsg = "hello eznetpp";
  _socket.send_bytes(hellomsg);
}

void tcp_echo_client::on_recv(const std::string& msg, int len) {
  _socket.send_bytes(msg);
  printf("on_recv\n");
}

void tcp_echo_client::on_send(unsigned int len) {
  printf("on_send\n");
}

void tcp_echo_client::on_close(int err_no) {
  printf("on_close\n");
}
