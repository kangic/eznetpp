// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <map>
namespace eznetpp {

namespace opt {
  const int max_transfer_bytes = 4096;
}  // namespace opt

namespace errcode {
  std::map<int, std::string> errcode_table = {
    {0, "success"}
    , {EAGAIN, "EAGAIN"}
  };
  
  /*
  std::map<int, std::string> errcode_table = {

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
  };
  */
}  // namespace errcode

}  // namespace eznetpp

#endif  // INCLUDE_COMMON_H_
