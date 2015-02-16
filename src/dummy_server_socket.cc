#include <cstdio>

#include "./dummy_server_socket.h"

namespace eznetpp {

dummy_server_socket::dummy_server_socket(void) {
}

dummy_server_socket::~dummy_server_socket(void) {
}

void dummy_server_socket::handle_accept() {
  printf("received event - handle_accpet()\n");
}

void dummy_server_socket::handle_read() {
  printf("received event - handle_read()\n");
}

void dummy_server_socket::handle_write() {
  printf("received event - handle_write()\n");
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
