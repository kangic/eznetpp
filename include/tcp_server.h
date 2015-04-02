// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_TCP_SERVER_H_
#define INCLUDE_TCP_SERVER_H_

#include <pthread.h>

#include "./macros.h"
#include "./if_server.h"

namespace eznetpp {
class tcp_server : public if_server {
 public:
  tcp_server();
  virtual ~tcp_server();

  // override
  int start_async_io();

  void set_env(int port, int max_accepts);

 protected:
  static void* accept_thread_caller(void* arg);
  void* accept_thread(void* arg);

  static void* read_thread_caller(void* arg);
  void* read_thread(void* arg);

 private:
  int setup_server_socket();


  ///////////////////////////////////
  // variables
  int host_port;
  int max_accepts_cnt;

  int server_socket;

  pthread_t accept_th_id;
  pthread_t read_th_id;
};
}  // namespace eznetpp

#endif  // INCLUDE_TCP_SERVER_H_
