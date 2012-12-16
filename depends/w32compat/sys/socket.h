#ifndef COMPAT_SYS_SOCKET_H
#define COMPAT_SYS_SOCKET_H

#include <winsock2.h>

#define EINPROGRESS WSAEINPROGRESS
#define ENOTCONN WSAENOTCONN
#define vsnprintf _vsnprintf
#define snprintf _snprintf

#define fork() -1

#define pipe(x) -1

#endif /* COMPAT_SYS_SOCKET_H */
