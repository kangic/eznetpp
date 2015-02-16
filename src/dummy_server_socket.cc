#include <cstdio>

#include "./dummy_server_socket.h"

namespace eznetpp {

dummy_server_socket::dummy_server_socket(void) {
}

dummy_server_socket::~dummy_server_socket(void) {
}

void dummy_server_socket::on_accept() {
  printf("received event - on_accpet()\n");
}

void dummy_server_socket::on_read() {
  printf("received event - on_read()\n");
}

void dummy_server_socket::on_write() {
  printf("received event - on_write()\n");
}

void dummy_server_socket::set_env(const std::string& ip, unsigned int port
		, unsigned int max_accepts) {
	host_ip = ip;
	host_port = port;
	max_accepts_cnt = max_accepts;

	// create server socket
	server_socket = 6;	// call socket();
}

int dummy_server_socket::write(int to_connector, const char* buffer, int len) {
  
}

}  // namespace eznetpp
