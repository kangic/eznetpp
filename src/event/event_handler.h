// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_EVENT_HANDLER_H_
#define INCLUDE_EVENT_HANDLER_H_

#include "eznetpp.h"
#include "net/if_socket.h"
#include "net/tcp/tcp_socket.h"

namespace eznetpp {
namespace event {

class event_handler {
 public:
  event_handler(void) = default;
  virtual ~event_handler(void) = default;

  enum event_handler_type {
    tcp_socket = 0,
    tcp_acceptor,
    udp_socket,
  };
  event_handler_type type() { return _handler_type; };
  virtual void on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no) = 0;
  virtual void on_connect(int err_no) = 0;
  virtual void on_recv(const std::string& msg, int len, int err_no) = 0;
  virtual void on_send(unsigned int len, int err_no) = 0;
  virtual void on_close(int err_no) = 0;

 protected:
  event_handler_type _handler_type;
};

class tcp_socket_event_handler : public event_handler {
 public:
  tcp_socket_event_handler(void) { 
    _handler_type = event_handler_type::tcp_socket;
  };
  virtual ~tcp_socket_event_handler(void) = default;

  virtual void on_connect(int err_no) = 0;
  virtual void on_recv(const std::string& msg, int len, int err_no) = 0;
  virtual void on_send(unsigned int len, int err_no) = 0;
  virtual void on_close(int err_no) = 0;

 protected:
  void on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no){};
};

class tcp_acceptor_event_handler : public event_handler {
 public:
  tcp_acceptor_event_handler(void) {
    _handler_type = event_handler_type::tcp_acceptor;
  };
  virtual ~tcp_acceptor_event_handler(void) = default;

  virtual void on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no) = 0;
  virtual void on_close(int err_no) = 0;

 protected:
  void on_connect(int err_no){};
  void on_recv(const std::string& msg, int len, int err_no){};
  void on_send(unsigned int len, int err_no){};
};

class udp_socket_event_handler : public event_handler {
 public:
  udp_socket_event_handler(void) { 
    _handler_type = event_handler_type::udp_socket;
  };
  virtual ~udp_socket_event_handler(void) = default;

  virtual void on_recvfrom(const std::string& msg, int len
      , const std::string& peer_ip, int peer_port, int err_no) = 0;
  virtual void on_sendto(unsigned int len, int err_no) = 0;
  virtual void on_close(int err_no) = 0;
};

}  // namespace event
}  // namespace eznetpp
#endif  // INCLUDE_EVENT_HANDLER_H_
