#include "./usr_server.h"

#include <cstdio>

usr_server::usr_server(void) {

}

usr_server::usr_server(const std::string ip, unsigned int port
		, unsigned int max_accepts)
{
}

usr_server::~usr_server(void) {
}

void usr_server::on_accept(const eznetpp::dummy_connection& conn) {
  printf("received event - on_accpet()\n");
}

void usr_server::on_read(const eznetpp::dummy_connection& conn, const std::string& msg
                     , int len) {
  printf("received event - on_read()\n");
}

void usr_server::on_write(const eznetpp::dummy_connection& conn, unsigned int len) {
  printf("received event - on_write()\n");
}
