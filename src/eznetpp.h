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

#ifndef INCLUDE_EZNETPP_H_
#define INCLUDE_EZNETPP_H_

// c headers
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>

// c++ headers
#include <algorithm>
#include <atomic>
#include <chrono>
#include <csignal>
#include <condition_variable>
#include <cstdarg>
#include <cstdio>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

// eznetpp headers
#include "util/logger.h"

namespace eznetpp {

namespace errcode {
  static std::map<int, std::string> errno_str_table = {
    {0, "success"},
    {EAGAIN, "sys_eagain"},
    {EWOULDBLOCK, "sys_ewouldblock"}, 
    {EBADF, "sys_ebadf"}, 
    {EBUSY, "sys_ebusy"}, 
    {EDEADLK, "sys_edeadlk"}, 
    {EPERM, "sys_eperm"}, 
    {EDESTADDRREQ, "sys_edestaddrreq"}, 
    {EDQUOT, "sys_edquot"}, 
    {EFAULT, "sys_efault"}, 
    {EFBIG, "sys_efbig"}, 
    {EINTR, "sys_eintr"}, 
    {EINVAL, "sys_einval"}, 
    {EIO, "sys_eio"}, 
    {EISDIR, "sys_eisdir"}, 
    {ENOSPC, "sys_enospc"}, 
    {EPIPE, "sys_epipe"}, 
    {ENOMEM, "sys_enomem"}, 
    {EMFILE, "sys_emfile"}, 
    {ENOLCK, "sys_enolck"}, 
    {ENOTDIR, "sys_enotdir"}, 
    {EDOM, "sys_edom"}, 
    {EISCONN, "sys_eisconn"}, 
    {ENOPROTOOPT, "sys_enoprotoopt"}, 
    {ENOTSOCK, "sys_enotsock"}, 
    {ENOBUFS, "sys_enobufs"}, 
    {ECONNABORTED, "sys_econnaborted"}, 
    {EOPNOTSUPP, "sys_eopnotsupp"}, 
    {EPROTO, "sys_eproto"}, 
    {EACCES, "sys_eacces"}, 
    {EADDRINUSE, "sys_eaddrinuse"}, 
    {EAFNOSUPPORT, "sys_eafnosupport"}, 
    {EADDRNOTAVAIL, "sys_eaddrnotavail"}, 
    {EALREADY, "sys_ealready"}, 
    {EINPROGRESS, "sys_einprogress"}, 
    {ETIMEDOUT, "sys_etimedout"}, 
    {ELOOP, "sys_eloop"}, 
    {ENAMETOOLONG, "sys_enametoolong"}, 
    {ENOENT, "sys_enoent"}, 
    {EROFS, "sys_erofs"}, 
    {ECONNRESET, "sys_econnreset"},

    // eznetpp lib errno
  };

  static const std::string& errno_to_str(int err_no)
  {
    return errno_str_table[err_no];
  }
}

namespace net {
  typedef struct _peer_addr
  {
    std::string ip = "";
    int port = -1;
  } peer_addr;
}
 
namespace opt {
  const int max_transfer_bytes = 4096;
}  // namespace opt
}  // namespace eznetpp

// macros
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete; \
  TypeName& operator=(const TypeName&) = delete;

#endif  // INCLUDE_EZNETPP_H_
