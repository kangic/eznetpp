#include "./echoclient_connection.h"

#include <unistd.h>

echoclient_connection::echoclient_connection() {
}

echoclient_connection::~echoclient_connection() {
}

void echoclient_connection::on_recv(const std::string& msg, int len, int err_no) {
  printf("recieved %d bytes(%d)\n", len, err_no);
  sleep(5);
  send_to_socket(msg, len);
}

void echoclient_connection::on_send(unsigned int len, int err_no) {
  printf("sent %d bytes(%d)\n", len, err_no);
}

void echoclient_connection::on_close(int err_no) {
  printf("closed\n");
}
