// Copyright[2015] <kangic21@gmail.com>

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
#include <condition_variable>
#include <cstdarg>
#include <cstdio>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
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

  static const std::string& errno_to_str(int err_no) {
    return errno_str_table[err_no];
  }
}

namespace opt {
  const int max_transfer_bytes = 4096;
}  // namespace opt
}  // namespace eznetpp

     /*
    success = 0,

    sys_eagain = EAGAIN,
    sys_ewouldblock = EWOULDBLOCK,
    sys_ebadf = EBADF,
    sys_ebusy = EBUSY,
    sys_edeadlk = EDEADLK,
    sys_eperm = EPERM,
    sys_edestaddrreq = EDESTADDRREQ,
    sys_edquot = EDQUOT,
    sys_efault = EFAULT,
    sys_efbig = EFBIG,
    sys_eintr = EINTR,
    sys_einval = EINVAL,
    sys_eio = EIO,
    sys_eisdir = EISDIR,
    sys_enospc = ENOSPC,
    sys_epipe = EPIPE,
    sys_enomem = ENOMEM,
    sys_emfile = EMFILE,
    sys_enolck = ENOLCK,
    sys_enotdir = ENOTDIR,
    sys_edom = EDOM,
    sys_eisconn = EISCONN,
    sys_enoprotoopt = ENOPROTOOPT,
    sys_enotsock = ENOTSOCK,
    sys_enobufs = ENOBUFS,
    sys_econnaborted = ECONNABORTED,
    sys_eopnotsupp = EOPNOTSUPP,
    sys_eproto = EPROTO,
    sys_eacces = EACCES,
    sys_eaddrinuse = EADDRINUSE,
    sys_eafnosupport = EAFNOSUPPORT,
    sys_eaddrnotavail = EADDRNOTAVAIL,
    sys_ealready = EALREADY,
    sys_einprogress = EINPROGRESS,
    sys_etimedout = ETIMEDOUT,
    sys_eloop = ELOOP,
    sys_enametoolong = ENAMETOOLONG,
    sys_enoent = ENOENT,
    sys_erofs = EROFS,
    */

// macros
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&); \
  void operator=(const TypeName&)

#endif  // INCLUDE_EZNETPP_H_
