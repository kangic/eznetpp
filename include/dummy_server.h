#ifndef _EZNETPP_SRC_DUMMY_SERVER_H_
#define _EZNETPP_SRC_DUMMY_SERVER_H_

#include <string>
#include <vector>

#include <pthread.h>

#include "./macros.h"
#include "./if_server.h"
#include "./event_dispatcher.h"

namespace eznetpp {
class dummy_server : public if_server {
public:
  dummy_server();
  virtual ~dummy_server();

public:
	// override
	int start_async_io();

	void set_env(const std::string& ip, unsigned int port, unsigned int max_accepts);

  int write(const dummy_connection& conn, const std::string& msg, int len);
	int broadcasting(const std::string& msg, int len);

	const std::vector<dummy_connection*>& get_connection_list();

protected:
  static void* accept_thread_caller(void* arg);
  void* accept_thread(void* arg);

private:
  std::string host_ip;
  int host_port;
  int max_accepts_cnt;

  int server_socket;

	pthread_t accept_th_id;

  std::vector<dummy_connection*> connections;
};
}  // namespace eznetpp

#endif  // _EZNETPP_SRC_DUMMY_SERVER_H_
