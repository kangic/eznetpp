#ifndef _EZNETPP_SRC_DUMMY_SERVER_SOCKET_H_
#define _EZNETPP_SRC_DUMMY_SERVER_SOCKET_H_

#include <string>
#include <vector>

#include "./macros.h"
#include "./if_event_listener.h"
#include "./event_dispatcher.h"

namespace eznetpp {
class if_connection;
class dummy_server_socket
  : public if_event_listener {
public:
  dummy_server_socket();
  virtual ~dummy_server_socket();

public:
  // override
  virtual void on_accept();
  virtual void on_read();
  virtual void on_write();

	void set_env(const std::string& ip, unsigned int port, unsigned int max_accepts);

  int write(int to_connector, const char* buffer, int len);

protected:
  void* read_thread();

private:
	void start_accept();

private:
  std::string host_ip;
  int host_port;
  int max_accepts_cnt;

  int server_socket;

  std::vector<if_connection&> connections;
};
}  // namespace eznetpp

#endif  // _EZNETPP_SRC_DUMMY_SERVER_SOCKET_H_
