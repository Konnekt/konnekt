#ifndef COMPAT_SYS_IOCTL_H
#define COMPAT_SYS_IOCTL_H

#define _WINSOCKAPI_
#include <winsock2.h>
#include <windows.h>
//#include <winioctl.h>
#include <stdarg.h>
#ifdef __cplusplus
extern "C" {
#endif
	int ioctl(int fd, int request, ...);
#ifdef __cplusplus
}
#endif
/*
int ioctl(int fd, int request, ...)
{
	va_list ap;
	unsigned long *foo;

	va_start(ap, request);
	foo = va_arg(ap, unsigned long*);
	va_end(ap);
	
	return ioctlsocket(fd, request, foo);

}*/

#endif /* COMPAT_SYS_IOCTL_H */
