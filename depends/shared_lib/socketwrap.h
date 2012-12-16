#ifndef SOCKETWRAPH
#define SOCKETWRAPH

/*
  *x->Windows sockets wrapper
*/

#include <winsock2.h>
#pragma link "Ws2_32.lib"

#define _UL_ (unsigned long)

#define write(handle,buf,len) send(handle , (const char*)buf , len , 0)
#define read(handle,buf,len) recv(handle , (char*)buf , len , 0)
#define close(handle) closesocket(handle)
#define socket(af , type , protocol) WSASocket(af , type , protocol , 0 , 0 , WSA_FLAG_OVERLAPPED)

#endif
 