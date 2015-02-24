#include "../include/dummy_server.h"

#include <cstdio>

#include <errno.h>

namespace eznetpp {

const int default_port_num = 6666;

dummy_server::dummy_server(void)
: host_ip(""), host_port(default_port_num)
, max_accepts_cnt(10), server_socket(-1)
, accept_th_id(-1) {
	connections.clear();
}

dummy_server::~dummy_server(void) {
	pthread_join(accept_th_id, 0);
}

void dummy_server::set_env(const std::string& ip, unsigned int port
		, unsigned int max_accepts) {
	host_ip = ip;
	host_port = port;
	max_accepts_cnt = max_accepts;

	// create server socket
	server_socket = 6;	// call socket();
}

int dummy_server::start_async_io() {
	printf("dummy_server::start_async_io() ->\n");
	int ret = pthread_create(&accept_th_id, NULL, accept_thread_caller, (void *)this);

	printf("dummy_server::start_async_io() - ret : %d\n", ret);
	if (ret != 0) {
		printf("dummy_server::start_async_io() - pthread_create failed(%d)\n", errno);
		return -1;
	}

	return 0;
}

int dummy_server::write(const dummy_connection& conn, const std::string& msg
		, int len) {
 	return 0; 
}

// work thread for accepting to a client
void* dummy_server::accept_thread_caller(void* arg) {
	printf("dummy_server::accept_thread_caller() ->\n");
	return ((dummy_server*)arg)->accept_thread(arg);
}

void* dummy_server::accept_thread(void* arg) {
	printf("hello\n");

	while(1) {

	}

	return NULL;
}

}  // namespace eznetpp
