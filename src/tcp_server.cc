// Copyright [2015] <kangic21@gmail.com>

#include "../include/tcp_server.h"

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../include/logger.h"

#include "../include/dummy_connection.h"

namespace eznetpp {

const int default_port_num = 6666;

tcp_server::tcp_server(void)
: host_port(default_port_num)
, max_accepts_cnt(10), server_socket(-1)
, accept_th_id(-1), read_th_id(-1) {
}

tcp_server::~tcp_server(void) {
  pthread_join(accept_th_id, 0);
}

void tcp_server::set_env(int port, int max_accepts) {
  host_port = port;
  max_accepts_cnt = max_accepts;
}

int tcp_server::start_async_io() {
  logger::instance().log(logger::log_level::debug
      , "tcp_server::start_async_io() ->\n");

  // initialization
  int ret = setup_server_socket();
  if (ret != 0) {
    return ret;
  }

  ret = pthread_create(&accept_th_id, nullptr, accept_thread_caller
      , reinterpret_cast<void *>(this));

  if (ret != 0) {
    logger::instance().log(logger::log_level::error
        , "tcp_server::start_async_io()-pthread_create failed(%d)\n", errno);
    return -1;
  }

  logger::instance().log(logger::log_level::debug
      , "tcp_server::start_async_io() <-\n");

  return 0;
}

// work thread for accepting to a client
void* tcp_server::accept_thread_caller(void* arg) {
  return (reinterpret_cast<tcp_server*>(arg))->accept_thread(arg);
}


void* tcp_server::accept_thread(void* arg) {
  logger::instance().log(logger::log_level::debug
      , "tcp_server::accept_thread() ->\n");

  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  while (1) {
    // todo : wait for accepting a client
    int client_sock = accept(server_socket, (struct sockaddr *)&client_addr
        , &client_addr_len);

    logger::instance().log(logger::log_level::debug
        , "tcp_server::accept_thread() - client socket id : %d\n", client_sock);

    connection* dc = new dummy_connection();

    on_accept(*dc, 0);
  }

  logger::instance().log(logger::log_level::debug
      , "tcp_server::accept_thread() ->\n");

  return nullptr;
}

// create server socket -> bind -> listen
int tcp_server::setup_server_socket() {
  logger::instance().log(logger::log_level::debug
      , "tcp_server::setup_server_socket() ->\n");

  // TODO(kangic) : AF_INET6
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(host_port);

  int ret;
  ret = bind(server_socket, (struct sockaddr *)&server_addr
      , sizeof(server_addr));

  if (ret != 0) {
    logger::instance().log(logger::log_level::debug
        , "tcp_server::setup_server_socket() - bind error(%d)\n", errno);

    return errno;
  }

  ret = listen(server_socket, max_accepts_cnt);

  if (ret != 0) {
    logger::instance().log(logger::log_level::debug
        , "tcp_server::setup_server_socket() - listen error(%d)\n", errno);

    return errno;
  }

  logger::instance().log(logger::log_level::debug
      , "tcp_server::setup_server_socket() <-\n");

  return 0;
}

}  // namespace eznetpp
