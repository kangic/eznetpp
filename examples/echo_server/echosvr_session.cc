#include "./echosvr_session.h"

echosvr_session::echosvr_session(eznetpp::net::tcp::tcp_socket* sock) 
: _socket(sock) {
}

echosvr_session::~echosvr_session() {
  printf("session destructor\n");
}

void echosvr_session::on_recv(const std::string& msg, int len, int err_no) {
  if (err_no) {
    printf("len : %d, err_no : %d(%s), socket : %d(%p)\n"
        , len, err_no, eznetpp::errcode::errno_to_str(err_no).c_str()
        , _socket->descriptor(), _socket);
    return;
  }

  //printf("received %d bytes\n", len);
  _socket->send(msg, len);
}

void echosvr_session::on_send(unsigned int len, int err_no) {
  if (err_no) {
    printf("err_no : %d(%s)\n", err_no, eznetpp::errcode::errno_to_str(err_no).c_str());
    return;
  }
  
  //printf("sent %d bytes\n", len);
}

void echosvr_session::on_close(int err_no) {
  printf("closed\n");
  if (err_no) {
    printf("err_no : %d(%s), socket : %d(%p)\n", err_no, eznetpp::errcode::errno_to_str(err_no).c_str(), _socket->descriptor(), _socket);
    return;
  }
}
