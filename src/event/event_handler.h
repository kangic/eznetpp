//
// Created by raymondk on 17. 1. 20.
//

#ifndef INCLUDE_EVENT_HANDLER_H_
#define INCLUDE_EVENT_HANDLER_H_

#include <event/if_event_handler.h>
#include <net/tcp/tcp_socket.h>

namespace eznetpp {
namespace event {

class tcp_socket_event_handler : public event::if_event_handler
{
 public:
  tcp_socket_event_handler()
  {
    _handler_type = event_handler_type::tcp_socket;
  };
  virtual ~tcp_socket_event_handler() = default;

  virtual void on_recv(const std::string& msg, int len) = 0;
  virtual void on_send(int len) = 0;
  virtual void on_close(int err_no) = 0;

 protected:
  void on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no) final {};
  void on_recvfrom(const std::string& msg, int len
      , const std::string& peer_ip, int peer_port) final {};
  void on_sendto(int len) final {};
};

class tcp_acceptor_event_handler : public event::if_event_handler
{
 public:
  tcp_acceptor_event_handler()
  {
    _handler_type = event_handler_type::tcp_acceptor;
  };
  virtual ~tcp_acceptor_event_handler() = default;

  virtual void on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no) = 0;
  virtual void on_close(int err_no) = 0;

 protected:
  void on_recv(const std::string& msg, int len) final {};
  void on_send(int len) final {};
  void on_recvfrom(const std::string& msg, int len
      , const std::string& peer_ip, int peer_port) final {};
  void on_sendto(int len) final {};
};

class udp_socket_event_handler : public event::if_event_handler
{
 public:
  udp_socket_event_handler()
  {
    _handler_type = event_handler_type::udp_socket;
  };
  virtual ~udp_socket_event_handler() = default;

  virtual void on_recvfrom(const std::string& msg, int len
      , const std::string& peer_ip, int peer_port) = 0;
  virtual void on_sendto(unsigned int len) = 0;
  virtual void on_close(int err_no) = 0;

 protected:
  void on_accept(eznetpp::net::tcp::tcp_socket* sock, int err_no) {};
  void on_recv(const std::string& msg, int len) final {};
  void on_send(int len) final {};
};

}  // namespace event
}  // namespace eznetpp

#endif //INCLUDE_EVENT_HANDLER_H_
