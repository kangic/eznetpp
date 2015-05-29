// Copyright [2015] <kangic21@gmail.com>

#include "../include/tcp_connection.h"

namespace eznetpp {

const int MAX_SOCK_ID = 0xFFFF;

tcp_connection::tcp_connection() {
}

tcp_connection::~tcp_connection() {
}

int tcp_connection::read_from_socket(void) {
  char read_buf[opt::max_transfer_bytes + 1] = {0, };

  int read_bytes = read(_sock_id, read_buf, opt::max_transfer_bytes);
  // TODO(kangic) : error code 추가
  on_read(read_buf, read_bytes, errno);
  return read_bytes;
}

int tcp_connection::write_to_socket(const std::string& data, int len) {
  int send_bytes = write(_sock_id, data.c_str(), len);
  // TODO(kangic) : error code 추가
  on_write(send_bytes, errno);
  return send_bytes;
}

int tcp_connection::close_socket(void) {
  int ret = 0;
  ret = close(_sock_id);
  on_close(ret);
  return ret;
}

}  // namespace eznetpp
