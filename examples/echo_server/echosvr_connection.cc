#include "./echosvr_connection.h"

echosvr_connection::echosvr_connection() {
}

echosvr_connection::~echosvr_connection() {
}

void echosvr_connection::on_recv(const std::string& msg, int len, int err_no) {
  printf("recieved %d bytes(%d)\n", len, err_no);
  send_to_socket(msg, len);
}

void echosvr_connection::on_send(unsigned int len, int err_no) {
  printf("sent %d bytes(%d)\n", len, err_no);
}

void echosvr_connection::on_close(int err_no) {
  printf("closed\n");
}
