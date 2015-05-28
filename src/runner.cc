// Copyright[2015] <kangic21@gmail.com>

#include "../include/runner.h"

#include "../include/if_server.h"
#include "../include/if_client.h"

namespace eznetpp {

runner::runner(void) {
}

runner::~runner(void) {
  _server_container.clear();
  _client_container.clear();
}

// not thread safe
void runner::register_server(if_server* server) {
  std::vector<if_server*>::iterator iter;

  for (iter = _server_container.begin();
       iter != _server_container.end();
       ++iter) {
    if (*iter == server) {
      // todo : print error log
    }
  }

  _server_container.push_back(server);
}

// not thread safe
void runner::deregister_server(if_server* server) {
  std::vector<if_server*>::iterator iter;

  for (iter = _server_container.begin();
       iter != _server_container.end();
       ++iter) {
    if (*iter == server) {
      iter = _server_container.erase(iter);
    }
  }

  if (iter == _server_container.end()) {
    // todo : print error log
  }
}

// not thread safe
void runner::register_client(if_client* client) {
  std::vector<if_client*>::iterator iter;

  for (iter = _client_container.begin();
       iter != _client_container.end();
       ++iter) {
    if (*iter == client) {
      // todo : print error log
    }
  }

  _client_container.push_back(client);
}

// not thread safe
void runner::deregister_client(if_client* client) {
  std::vector<if_client*>::iterator iter;

  for (iter = _client_container.begin();
       iter != _client_container.end();
       ++iter) {
    if (*iter == client) {
      iter = _client_container.erase(iter);
    }
  }

  if (iter == _client_container.end()) {
    // todo : print error log
  }
}

void runner::run(void) {
  std::vector<if_server*>::iterator iter_server;
  std::vector<if_client*>::iterator iter_client;

  for (iter_server = _server_container.begin();
       iter_server != _server_container.end();
       ++iter_server) {
    if (0 != (*iter_server)->start_async_io())
      // todo : all stop to run the server already, print error log
      return;
  }

  for (iter_client = _client_container.begin();
       iter_client != _client_container.end();
       ++iter_client) {
    if (0 != (*iter_client)->start_async_io())
      // todo : all stop to run the server already, print error log
      return;
  }

  while (true) {
    // todo : catch for interrupting stop signal
  }
}

}  // namespace eznetpp
