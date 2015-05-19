#include "./echosvr_connection.h"

echosvr_connection::echosvr_connection() {
}

echosvr_connection::~echosvr_connection() {
}

void echosvr_connection::on_read(const std::string& msg, int len, int err_no) {
  printf("recieved %d bytes(%d)\n", len, err_no);
  write_to_socket(msg, len);
}

void echosvr_connection::on_write(unsigned int len, int err_no) {
  printf("sent %d bytes(%d)\n", len, err_no);
}

void echosvr_connection::on_close(int err_no) {
  printf("closed\n");
}
