#include "./echo_client.h"

#include <cstdio>

echo_client::echo_client(eznetpp::sys::io_manager* io_mgr)
: _io_mgr(io_mgr){
}

echo_client::~echo_client(void) {
}

void echo_client::start(void) {
  _io_mgr->register_socket_event_handler(&_socket, this);

  _socket.connect("127.0.0.1", 56789);
}

void echo_client::on_connect(int err_no) {
  printf("on_connect\n");
  std::string hellomsg = "hello eznetpp";
  _socket.send(hellomsg, hellomsg.length());
}

void echo_client::on_recv(const std::string& msg, int len, int err_no) {
  _socket.send(msg, len);
  printf("on_recv\n");
}

void echo_client::on_send(unsigned int len, int err_no) {
  printf("on_send\n");
}

void echo_client::on_close(int err_no) {
  printf("on_close\n");
}
