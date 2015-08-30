#include "./echosvr_session.h"

echosvr_session::echosvr_session(eznetpp::net::tcp::tcp_socket* sock) 
: _socket(sock) {
}

echosvr_session::~echosvr_session() {
}

void echosvr_session::on_recv(const std::string& msg, int len, int err_no) {
  if (err_no) {
    printf("err_no : %d(%s)\n", err_no, eznetpp::errcode::errno_to_str(err_no).c_str());
    return;
  }

  printf("recieved %d bytes\n", len);
  _socket->send(msg, len);
}

void echosvr_session::on_send(unsigned int len, int err_no) {
  if (err_no) {
    printf("err_no : %d(%s)\n", err_no, eznetpp::errcode::errno_to_str(err_no).c_str());
    return;
  }
  
  printf("sent %d bytes\n", len);
}

void echosvr_session::on_close(int err_no) {
  printf("closed\n");
}
