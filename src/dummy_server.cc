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
}

/*
void dummy_server::on_accept(const dummy_connection& conn) {
  printf("received event - on_accpet()\n");
}

void dummy_server::on_read(const dummy_connection& conn, const std::string& msg
                     , int len) {
  printf("received event - on_read()\n");
}

void dummy_server::on_write(const dummy_connection& conn, unsigned int len) {
  printf("received event - on_write()\n");
}
*/

void dummy_server::set_env(const std::string& ip, unsigned int port
		, unsigned int max_accepts) {
	host_ip = ip;
	host_port = port;
	max_accepts_cnt = max_accepts;

	// create server socket
	server_socket = 6;	// call socket();
}

int dummy_server::async_run() {
	int ret = pthread_create(&accept_th_id, NULL, accept_thread_caller, this);

	if (ret != 0) {
		return errno;
	}
}

int dummy_server::write(const dummy_connection& conn, const std::string& msg
		, int len) {
 	return 0; 
}

// work thread for accepting to a client
void* dummy_server::accept_thread_caller(void* arg) {
	dummy_server* dss = (dummy_server*)arg;
	return dss->accept_thread(arg);
}

void* dummy_server::accept_thread(void* arg) {
	printf("hello");

	return NULL;
}

}  // namespace eznetpp
