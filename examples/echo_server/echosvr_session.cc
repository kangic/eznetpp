#include "./echosvr_session.h"

echosvr_session::echosvr_session(eznetpp::net::tcp::tcp_socket& sock) 
: _socket(sock) {
}

echosvr_session::~echosvr_session() {
}

void echosvr_session::on_recv(const std::string& msg, int len, int err_no) {
  //printf("recieved %d bytes(%d)\n", len, err_no);
  _socket.send(msg, len);

  ++_recv_cnt;
  if (_recv_cnt % 1000 == 0)
    printf("[%d] receive packet count : %ld\n", _socket.descriptor(), _send_cnt);
}

void echosvr_session::on_send(unsigned int len, int err_no) {
  //printf("sent %d bytes(%d)\n", len, err_no);
  ++_send_cnt;
  if (_send_cnt % 1000 == 0)
    printf("[%d] send packet count : %ld\n", _socket.descriptor(), _send_cnt);
}

void echosvr_session::on_close(int err_no) {
  //printf("closed\n");
}
