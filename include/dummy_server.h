// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_DUMMY_SERVER_H_
#define INCLUDE_DUMMY_SERVER_H_

#include <string>
#include <vector>

#include "./macros.h"
#include "./if_server.h"

namespace eznetpp {
class dummy_server : public if_server {
 public:
  dummy_server();
  virtual ~dummy_server();

 public:
  // override
  int start_async_io();

  void set_env(int port, int max_accepts);

  const std::vector<connection*>& get_connection_list();

 protected:
  static void* accept_thread_caller(void* arg);
  void* accept_thread(void* arg);

 private:
  int host_port;
  int max_accepts_cnt;

  int server_socket;

  pthread_t accept_th_id;

  std::vector<connection*> connections;
};
}  // namespace eznetpp

#endif  // INCLUDE_DUMMY_SERVER_H_
