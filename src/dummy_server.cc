#include "../include/dummy_server.h"
#include "../include/logger.h"

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
	logger::instance().log(logger::log_level::debug, "dummy_server::start_async_io() ->\n");
	int ret = pthread_create(&accept_th_id, NULL, accept_thread_caller, (void *)this);

	if (ret != 0) {
		logger::instance().log(logger::log_level::error, "dummy_server::start_async_io() - pthread_create failed(%d)\n", errno);
		return -1;
	}

	logger::instance().log(logger::log_level::debug, "dummy_server::start_async_io() <-\n");
	return 0;
}

int dummy_server::write(const dummy_connection& conn, const std::string& msg
		, int len) {
 	return 0; 
}

// work thread for accepting to a client
void* dummy_server::accept_thread_caller(void* arg) {
	return ((dummy_server*)arg)->accept_thread(arg);
}

void* dummy_server::accept_thread(void* arg) {
	logger::instance().log(logger::log_level::debug, "dummy_server::accept_thread() ->\n");

	while(1) {

	}

	logger::instance().log(logger::log_level::debug, "dummy_server::accept_thread() ->\n");
	return NULL;
}

}  // namespace eznetpp
