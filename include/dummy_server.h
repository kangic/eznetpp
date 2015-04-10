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
  virtual void on_accept(connection* conn, int err_no) = 0;
  virtual void on_read(connection* conn, const std::string& msg, int len
      , int err_no) = 0;
  virtual void on_write(connection* conn, unsigned int len, int err_no) = 0;
  virtual void on_close(connection* conn, int err_no) = 0;

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

  DISALLOW_COPY_AND_ASSIGN(dummy_server);
};
}  // namespace eznetpp

#endif  // INCLUDE_DUMMY_SERVER_H_
