#include "./echosvr_session.h"

echosvr_session::echosvr_session(eznetpp::net::tcp::tcp_socket& sock) 
: _socket(sock) {
}

echosvr_session::~echosvr_session() {
}

void echosvr_session::on_recv(const std::string& msg, int len, int err_no) {
  printf("recieved %d bytes(%d)\n", len, err_no);
  _socket.send(msg, len);
}

void echosvr_session::on_send(unsigned int len, int err_no) {
  printf("sent %d bytes(%d)\n", len, err_no);
}

void echosvr_session::on_close(int err_no) {
  printf("closed\n");
}
