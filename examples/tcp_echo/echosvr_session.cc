#include "./echosvr_session.h"

echosvr_session::echosvr_session(eznetpp::net::tcp::tcp_socket* sock) 
: _socket(sock) {
}

echosvr_session::~echosvr_session() {
}

void echosvr_session::on_recv(const std::string& msg, int len, int err_no) {
  if (err_no) {
    printf("len : %d, err_no : %d(%s), socket : %d(%p)\n"
        , len, err_no, eznetpp::errcode::errno_to_str(err_no).c_str()
        , _socket->descriptor(), _socket);

    //return;
  }

  ++_recv_cnt;
  _recv_bytes += len;
  if (_recv_cnt % 10000 == 0) {
    printf("received %ld packets(%lld bytes)\n", _recv_cnt, _recv_bytes);
  }
  _socket->send_bytes(msg);
}

void echosvr_session::on_send(unsigned int len, int err_no) {
  if (err_no) {
    printf("err_no : %d(%s)\n", err_no, eznetpp::errcode::errno_to_str(err_no).c_str());
    //return;
  }
  
  ++_send_cnt;
  _send_bytes += len;
  if (_send_cnt % 10000 == 0) {
    printf("sent %ld packets(%lld bytes)\n", _send_cnt, _send_bytes);
  }
}

void echosvr_session::on_close(int err_no) {
  if (err_no) {
    printf("err_no : %d(%s), socket : %d(%p)\n", err_no, eznetpp::errcode::errno_to_str(err_no).c_str(), _socket->descriptor(), _socket);
    return;
  }
}
