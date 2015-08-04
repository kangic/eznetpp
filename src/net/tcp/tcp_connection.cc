// Copyright [2015] <kangic21@gmail.com>

#include "tcp_connection.h"

namespace eznetpp {

const int MAX_SOCK_ID = 0xFFFF;

tcp_connection::tcp_connection() {
}

tcp_connection::~tcp_connection() {
}

int tcp_connection::recv_from_socket(void) {
  char read_buf[opt::max_transfer_bytes + 1] = {0, };

  int read_bytes = recv(_sock_id, read_buf, opt::max_transfer_bytes
                        , MSG_NOSIGNAL);
  // TODO(kangic) : error code 추가
  on_recv(read_buf, read_bytes, errno);
  return read_bytes;
}

int tcp_connection::send_to_socket(const std::string& data, int len) {
  int send_bytes = send(_sock_id, data.c_str(), len, MSG_NOSIGNAL);
  // TODO(kangic) : error code 추가
  on_send(send_bytes, errno);
  return send_bytes;
}

int tcp_connection::close_socket(void) {
  int ret = 0;
  ret = close(_sock_id);
  on_close(ret);
  return ret;
}

}  // namespace eznetpp
