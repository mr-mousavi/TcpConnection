/*
 * @author          Mohammad Reza Mousavi <smr.mousavi87@gmail.com>
 * @date            19th March 2022
 *
 * @project         Connection
 */
 
#ifndef CONNECTIONTCP_H
#define CONNECTIONTCP_H

#include "Connection.h"

class ConnectionTcp : public Connection {
public:
    /**
     * @copydoc Connection::Connection()
     */
    ConnectionTcp(int port, const char* ip);

    /**
     * @copydoc Connection::~Connection()
     */
    virtual ~ConnectionTcp();

    /**
     * @copydoc Connection::send()
     */
    virtual ssize_t send(const char* buffer, size_t bufferLen, int flag);

    /**
     * @copydoc Connection::receive()
     */
    virtual ssize_t receive(char* buffer, size_t bufferLen, int flag);

private:
    /**
     * @copydoc Connection::createSocket()
     */
#ifdef __linux__
    virtual int createSocket();
#elif _WIN32
    virtual SOCKET createSocket();
#endif

};

#endif /* CONNECTIONTCP_H */

