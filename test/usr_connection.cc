#include "./usr_connection.h"

usr_connection::usr_connection() {
}

usr_connection::~usr_connection() {
}

void usr_connection::on_read(const std::string& msg, int len, int err_no) {
  printf("recieved %d bytes\n", len);
}

void usr_connection::on_write(unsigned int len, int err_no) {
  printf("sent %d bytes\n", len);
}

void usr_connection::on_close(int err_no) {
  printf("closed\n");
}
