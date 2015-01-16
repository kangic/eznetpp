#ifndef _EZNETPP_SRC_DUMMY_SERVER_SOCKET_H_
#define _EZNETPP_SRC_DUMMY_SERVER_SOCKET_H_

#include <string>
#include <vector>

#include "./macros.h"
#include "./if_event_listener.h"
#include "./event_dispatcher.h"

namespace eznetpp {
class dummy_server_socket
  : public if_event_listener {
 public:
  dummy_server_socket();
  virtual ~dummy_server_socket();

 public:
  // override
  virtual void handle_accept();
  virtual void handle_read();
  virtual void handle_write();

  int write(int to_connector, const char* buffer, int len);

 protected:
  void *read_thread();

 private:
  std::string host_ip;
  int host_port;
  int max_waiting_cnt;

  int server_socket;
  std::vector<int> connector_sockets;
};
}  // namespace eznetpp

#endif  // _EZNETPP_SRC_DUMMY_SERVER_SOCKET_H_
