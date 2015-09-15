#include "./tcp_echosvr_session.h"

tcp_echosvr_session::tcp_echosvr_session(eznetpp::net::tcp::tcp_socket* sock) 
: _socket(sock) {
}

tcp_echosvr_session::~tcp_echosvr_session() {
}

void tcp_echosvr_session::on_recv(const std::string& msg, int len) {
  ++_recv_cnt;
  _recv_bytes += len;
  if (_recv_cnt % 10000 == 0) {
    printf("received %ld packets(%lld bytes)\n", _recv_cnt, _recv_bytes);
  }
  _socket->send_bytes(msg);
}

void tcp_echosvr_session::on_send(unsigned int len) {
  ++_send_cnt;
  _send_bytes += len;
  if (_send_cnt % 10000 == 0) {
    printf("sent %ld packets(%lld bytes)\n", _send_cnt, _send_bytes);
  }
}

void tcp_echosvr_session::on_close(int err_no) {
  if (err_no) {
    printf("err_no : %d(%s), socket : %d(%p)\n", err_no, eznetpp::errcode::errno_to_str(err_no).c_str(), _socket->descriptor(), _socket);
    return;
  }
}
