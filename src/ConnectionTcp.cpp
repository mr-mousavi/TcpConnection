/*
 * @author          Mohammad Reza Mousavi <smr.mousavi87@gmail.com>
 * @date            19th March 2022
 *
 * @project         Connection
 */
#include "ConnectionTcp.h"
#include <stdio.h>
#ifdef _WIN32
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#endif

ConnectionTcp::ConnectionTcp(int port, const char* ip)
: Connection(port, ip)
{

}

ConnectionTcp::~ConnectionTcp()
{
}

ssize_t ConnectionTcp::send(const char* buffer, size_t bufferLen, int flag)
{
#ifdef __linux__
    return ::send(socketWrapper.fd, buffer, bufferLen, flag);
#elif _WIN32
  return ::send(socketWrapper.sock, buffer, bufferLen, flag);
#endif
}

ssize_t ConnectionTcp::receive(char* buffer, size_t bufferLen, int flag)
{
#ifdef __linux__
  return ::recv(socketWrapper.fd, buffer, bufferLen, flag);
#elif _WIN32
  return ::recv(socketWrapper.sock, buffer, bufferLen, flag);
#endif
}

#ifdef __linux__
int ConnectionTcp::createSocket()
{
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd <= 0) {
        addError((int) ERROR_CODE_CREATE_SOCKET, getErrorMsg(ERROR_CODE_CREATE_SOCKET));
        return -1;
    }

    socketWrapper.fd = fd;
    return 0;
}
#endif

#ifdef _WIN32
SOCKET ConnectionTcp::createSocket()
{
   SOCKET s;
   if((s=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==INVALID_SOCKET){
        addError((int) ERROR_CODE_CREATE_SOCKET, getErrorMsg(ERROR_CODE_CREATE_SOCKET));
        WSACleanup();
        return s;
    }

//	unsigned long ul = 1;
//	int ret=ioctlsocket(s, FIONBIO, (unsigned long *) &ul);
//	if (ret == SOCKET_ERROR){
//		WSACleanup();
//		return s;
//	}
	
    socketWrapper.sock = s;
    return s;
}
#endif
