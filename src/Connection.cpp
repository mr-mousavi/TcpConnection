/*
 * @author          Mohammad Reza Mousavi <smr.mousavi87@gmail.com>
 * @date            19th March 2022
 *
 * @project         Connection
 */

#include "Connection.h"
#include <cstring>
#include <unistd.h>
#include <stdio.h>

Connection::Connection(int port, const char* ip)
{
    memset(&socketWrapper, 0, sizeof (socketWrapper));
#ifdef __linux
    socketWrapper.fd = -1;
#elif _WIN32
    socketWrapper.sock = INVALID_SOCKET;
#endif
    socketWrapper.sockAddr.sin_family = AF_INET;
    socketWrapper.sockAddr.sin_addr.s_addr = inet_addr(ip);
    socketWrapper.sockAddr.sin_port = htons(port);
    this->port = port;
    this->ip = ip;
}

Connection::~Connection()
{
    disconnect();
}

bool Connection::hasError() const
{
    return !errors.empty();
}

const std::string& Connection::getLastErrorMsg() const
{
    return errors.back().second;
}

int Connection::connect()
{
#ifdef __linux__
    if (createSocket() < 0)
#elif _WIN32
    if(createSocket()==INVALID_SOCKET)
#endif
        return -1;

    socklen_t socketAddrLen = sizeof (socketWrapper.sockAddr);
#ifdef __linux__
    if (::connect(socketWrapper.fd,
            (struct sockaddr*) &socketWrapper.sockAddr, socketAddrLen) < 0) {
#elif _WIN32
    if (::connect(socketWrapper.sock,
            (struct sockaddr*) &socketWrapper.sockAddr, socketAddrLen) < 0) {
#endif
        addError((int) ERROR_CODE_CONNECTION, getErrorMsg(ERROR_CODE_CONNECTION));
        disconnect();
        return -1;
    }
    return 0;
}

int Connection::disconnect()
{
#ifdef __linux__
    int fd = socketWrapper.fd;
    socketWrapper.fd = -1;

    if(fd > 0)
        return close(fd);
#elif _WIN32
    SOCKET s = socketWrapper.sock;
    socketWrapper.sock = INVALID_SOCKET;

    if(s != INVALID_SOCKET)
        return closesocket(s);
#endif
    return 0;
}

bool Connection::isConnected()
{
#ifdef __linux__
    return socketWrapper.fd > 0;
#elif _WIN32
    return socketWrapper.sock != INVALID_SOCKET;
#endif
}

void Connection::clearBuffer()
{
    char buffer[512];
    while (true) {
#ifdef __linux__
        if (recv(socketWrapper.fd, buffer, 512, MSG_DONTWAIT) < 512)
#elif _WIN32
        if (recv(socketWrapper.sock, buffer, 512, 0) < 512)
#endif
            break;
        usleep(100000);
    }
}

void Connection::addError(int errorCode, const std::string errorMsg)
{
    errors.push_back(std::make_pair(errorCode, errorMsg));
}

void Connection::flushErrors()
{
    errors.clear();
}

SocketWrapper& Connection::getSocketWrapper()
{
    return socketWrapper;
}

const char* Connection::getIp()
{
  return ip;
}

#ifdef _WIN32
int Connection::winSocketStartUp()
{
    WSADATA  wsa;
    if(WSAStartup(MAKEWORD(2,2),&wsa)!=0){
      addError((int) ERROR_CODE_STARTUP_IN_WIN, getErrorMsg(ERROR_CODE_STARTUP_IN_WIN));
      return -1;
    }
    return 0;
}
#endif

int& Connection::getPort()
{
    return port;
}

int Connection::setIoctl(long cmd, u_long *argp)
{
#ifdef _WIN32
    if (ioctlsocket(socketWrapper.sock, cmd, argp) == SOCKET_ERROR){
        return -1;
    }
#endif
	return 0;
}
	
std::string Connection::getErrorMsg(ErrorCode errCode)
{
    std::string errStr = "";
    switch (errCode) {
    case ERROR_CODE_CREATE_SOCKET:
        errStr = "Create socket failed";
        break;
    case ERROR_CODE_CONNECTION:
        errStr.append("Could not connect to ");
        errStr.append(ip);
        break;
    case ERROR_CODE_STARTUP_IN_WIN:
        errStr.append("Could not startup ");
        break;
	case ERROR_CODE_IOCTL:
        errStr.append("Could not set ioctl ");
        break;
    default:
        errStr = "Not defined error";
        break;

    }

    return errStr;
}
