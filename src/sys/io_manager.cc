/* Copyright (c) 2015 In Cheol, Kang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "io_manager.h"
#include "event/event_dispatcher.h"

namespace eznetpp {
namespace sys {

int io_manager::_epoll_fd = -1;

io_manager::io_manager(bool log_enable)
{
  eznetpp::util::logger::instance().set_enable_option(log_enable);
}

io_manager::~io_manager()
{

}

/*
 * create epoll file descriptor and events
 */
int io_manager::init(int max_desc_cnt)
{
  // create epoll fd and event structures
  _epoll_fd = epoll_create(max_desc_cnt);
  if (_epoll_fd == -1)
  {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                          , __FILE__, __FUNCTION__, __LINE__
                          , "epoll_create error(%d)"
                          , errno);

    return errno;
  }

  _events = new epoll_event[max_desc_cnt];
  if (_events == nullptr)
  {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
                          , __FILE__, __FUNCTION__, __LINE__
                          , "failed to create events(%d)"
                          , errno);

    return errno;
  }

  eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                        , __FILE__, __FUNCTION__, __LINE__
                        , "created epoll_fd : %d"
                        , _epoll_fd);

  _max_descs_cnt = max_desc_cnt;

  signal(SIGPIPE, SIG_IGN);

  return 0;
}

int io_manager::register_socket_event_handler(eznetpp::net::if_socket* sock
      , eznetpp::event::if_event_handler* handler)
{
  struct epoll_event ev{};

  ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP | EPOLLONESHOT;
  ev.data.ptr = sock;

  int ret = epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, sock->descriptor(), &ev);

  if (ret != 0) {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
        , __FILE__, __FUNCTION__, __LINE__
        , "failed epoll_ctl() : %d"
        , ret);

    return ret;
  }

  sock->set_event_handler(handler);

  return 0;
}

int io_manager::deregister_socket_event_handler(eznetpp::net::if_socket* sock)
{
  sock->set_event_handler(nullptr);
    
  return epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, sock->descriptor(), NULL);
}

int io_manager::update_epoll_event(eznetpp::net::if_socket* sock)
{
  struct epoll_event ev{};

  ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP | EPOLLONESHOT;
  ev.data.ptr = sock;

  return epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, sock->descriptor(), &ev);
}


int io_manager::loop()
{
  _loop_th = std::thread(&io_manager::epoll_loop, this);

  if (!_loop_th.joinable())
  {
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::error
        , __FILE__, __FUNCTION__, __LINE__
        , "failed to create epoll_loop thread(%d)"
        , errno);

    return -1;
  }

  _loop_th.join();

  return 0;
}

void io_manager::stop()
{
  eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "-> S");

  ::close(_epoll_fd);

  if (_events != nullptr)
  {
    delete _events;
    _events = nullptr;
  }

  bClosed = true;
  if (_loop_th.joinable())
  {
    _loop_th.detach();
  }

  eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "<- E");
}

void io_manager::epoll_loop()
{
  eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "start epoll_loop");

  eznetpp::event::event_dispatcher evt_dispatcher;

  while (true)
  {
    if (bClosed)
    {
      break;
    }

    int changed_events = epoll_wait(_epoll_fd, _events, _max_descs_cnt, 1000);
    eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                          , __FILE__, __FUNCTION__, __LINE__
                          , "changed_events : %d"
                          , changed_events);

    if (changed_events < 0)
    {
      eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                          , __FILE__, __FUNCTION__, __LINE__
                          , "epoll failed");

      continue;
    }

    eznetpp::event::io_event* evt = nullptr;
    for (int i = 0; i < changed_events; ++i)
    {
      auto sock = static_cast<eznetpp::net::if_socket*>(_events[i].data.ptr);

      eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                          , __FILE__, __FUNCTION__, __LINE__
                          , "changed descriptor : %d"
                          , sock->descriptor());

      if (_events[i].events & EPOLLIN)
      {
        int ret;
        bool read_again = true;
        while (read_again)
        {
          evt = sock->_recv(ret);

          // TODO : Need to refactoring simplify.
          eznetpp::event::if_event_handler* handler = sock->get_event_handler();
          read_again = false;

          if (handler->type() == eznetpp::event::if_event_handler::event_handler_type::tcp_acceptor)
          {
            if (ret > 0)
            {
              read_again = true;
            }
          }
          else
          {
            if (ret == eznetpp::opt::max_transfer_bytes)
            {
              read_again = true;
            }
          }

          if (read_again && evt != nullptr)
          {
            evt_dispatcher.dispatch_event(evt, sock);

            delete evt;
            evt = nullptr;
          }
        }
      }
      else if (_events[i].events & EPOLLOUT)
      {
        evt = sock->_send();
      }
      else if ((_events[i].events & EPOLLHUP)
               || (_events[i].events & EPOLLRDHUP)  // half close by the remote connection
               || (_events[i].events & EPOLLERR)
               || !(_events[i].events & EPOLLIN))
      {
        evt = sock->_close();

        delete sock;
        sock = nullptr;
      }

      update_epoll_event(sock);

      if (evt != nullptr) {
        evt_dispatcher.dispatch_event(evt, sock);

        delete evt;
        evt = nullptr;
      }
    }
  }

  eznetpp::util::logger::instance().log(eznetpp::util::logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "<- E");
}

}  // namespace sys
}  // namespace eznetpp
