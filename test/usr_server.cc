#include "./usr_server.h"

#include <cstdio>

usr_server::usr_server(void) {
}

usr_server::usr_server(const std::string ip, unsigned int port
    , unsigned int max_accepts) {
}

usr_server::~usr_server(void) {
}

void usr_server::on_accept(const eznetpp::dummy_connection& conn, int err_no) {
  printf("received event - on_accpet()\n");
}

void usr_server::on_read(const eznetpp::dummy_connection& conn
    , const std::string& msg, int len, int err_no) {
  printf("received event - on_read()\n");
}

void usr_server::on_write(const eznetpp::dummy_connection& conn
    , unsigned int len, int err_no) {
  printf("received event - on_write()\n");
}

void usr_server::on_close(const eznetpp::dummy_connection& conn, int err_no) {
  printf("received event - on_close()\n");
}
