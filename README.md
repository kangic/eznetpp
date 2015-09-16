# eznetpp

This is an asynchronous network library for C++ programmers.

eznetpp library supports :
* Can develop a network application easy.
* Can concentrate to make your business logic.


## Requirements

* C++ compiler with C++11 support.
  - eznetpp was developed by g++ version 4.8.4.
* CMake(>=2.8)


## Getting started

* Clone this repository
  > git clone https://github.com/kangic/eznetpp/eznetpp.git

* Build a library
  * Create output files in `{EZNETPP_ROOT}/build` directory after run a `cmake . && make` command.


## How to use
### Role of each component

* IO Manager class
  * It's a main class to run the loop and dispatch the socket i/o events. 
  * Register the socket object to poll its i/o and the event_handler object to receive
  socket's event.

* Socket / Event Handler pairs
  * tcp_acceptor / tcp_acceptor_event_handler
    - `on_accept`, `on_close`
  * tcp_socket / tcp_socket_event_handler
    - `on_connect`, `on_recv`, `on_send`, `on_close`
  * udp_socket / udp_socket_event_handler
    - `on_recvfrom`, `on_sendto`, `on_close`

### TCP Echo Server Example

```cpp
#include "eznetpp.h"
#include "net/tcp/tcp_acceptor.h"
#include "net/tcp/tcp_socket.h"
#include "sys/io_manager.h"

class tcp_echosvr_session : public eznetpp::event::tcp_socket_event_handler {
 public:
  tcp_echosvr_session(eznetpp::net::tcp::tcp_socket* sock) {
    _socket = sock;
  }
  virtual ~tcp_echosvr_session() = default;

 public:
  // override
  void on_recv(const std::string& msg, int len) {
    printf("received %d bytes\n", len);
    _socket->send_bytes(msg);
  }
  void on_send(unsigned int len) {
    printf("sent %d bytes\n", len);
  }
  void on_close(int err_no) {
    printf("closed the session(%d)\n", err_no);
  }

  // do not need 
  void on_connect(int err_no) {};

 private:
  eznetpp::net::tcp::tcp_socket* _socket;
};

class tcp_echo_server : public eznetpp::event::tcp_acceptor_event_handler {
 public:
  tcp_echo_server(eznetpp::sys::io_manager* io_mgr) {
    _io_mgr = io_mgr;
  }
  virtual ~tcp_echo_server() = default;

  int open(int port, int backlog) {
    int ret = _acceptor.open(port, backlog);

    if (ret) {
      printf("%s\n", eznetpp::errcode::errno_to_str(ret).c_str());
      return -1;
    }

    _io_mgr->register_socket_event_handler(&_acceptor, this);

    return 0;
  }

  // override
  void on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no) {
    _io_mgr->register_socket_event_handler(sock, new tcp_echosvr_session(sock));
  }
  void on_close(int err_no) {
    printf("closed the acceptor(%d)\n", err_no);
  }

 private:
  eznetpp::sys::io_manager* _io_mgr = nullptr;
  eznetpp::net::tcp::tcp_acceptor _acceptor;
};

int main(void) {
  // num of event dispatching worker threads, log enable option
  eznetpp::sys::io_manager io_mgr(4, false);  

  io_mgr.init();

  tcp_echo_server server(&io_mgr);
  server.open(56789, 5);
  
  io_mgr.loop();

  return 0;
}
``` 

* There are some samples in `examples` directory for using eznetpp.
