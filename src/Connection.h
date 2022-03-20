/*
 * @author          Mohammad Reza Mousavi <smr.mousavi87@gmail.com>
 * @date            19th March 2022
 *
 * @project         Connection
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#elif __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <string>
#include <vector>


/**
 * @brief it wraps socket descriptor and sockaddr_in
 */

struct SocketWrapper {
#ifdef __linux__
    int fd;
#elif _WIN32
  SOCKET sock;
#endif
    sockaddr_in sockAddr;
} __attribute((packed));

class Connection {
public:

    enum ErrorCode {
        ERROR_CODE_CREATE_SOCKET = -1,
        ERROR_CODE_CONNECTION = -2,
        ERROR_CODE_STARTUP_IN_WIN =-3,
		ERROR_CODE_IOCTL =-4,
		
    };
    /**
     * Construct connection with the given parameters
     * 
     * @param port of remote host to connect
     * @param ip of remote host to connect
     * @param connectionType the type of connection
     */
    Connection(int port, const char* ip);

    /**
     * Default destructor of the connection. It will free the resource and 
     * close socket if it is still open.
     */
    virtual ~Connection();

    /**
     * It checks whether or not an error occures up to here.
     * @return true if there is any error otherwise false
     */
    bool hasError() const;

    /**
     * Retrieve the msg of last occurred msg. It will not deleted from 
     * error list
     * @return the msg related to last msg occurred
     */
    const std::string& getLastErrorMsg() const;

    /**
     * This method establishes connection.
     * @return -1 on failure, 0 on success
     */
    virtual int connect();

    /**
     * This method close the file descriptor of connection.
     * @return -1 on failure, 0 on success
     */
    virtual int disconnect();

    /**
     * Check whether or not the connection is established and available.
     * @return true if connection is available otherwise false
     */
    bool isConnected();
    /**
     * This method clears the socket buffer. It will receive from 
     * socket until it reaches end.
     * @return void
     */
    void clearBuffer();

    /**
     * This method sends given buffer over current established connection.
     * 
     * @param buffer to send over current established socket
     * @param bufferLen the length of give buffer
     * @param posixFlag the flags that will be passed to standard 
     * send function.
     * @return on success the length of sent buffer, on error -1
     */
    virtual ssize_t send(const char*buffer, size_t bufferLen, int posixFlag) = 0;

    /**
     * 
     * @param buffer to fill with recieved data
     * @param bufferLen the length of data the must be recieved
     * @param posixFlag th flags that will be passed to standard
     * recv function
     * @return the number of bytes received, -1 on error
     */
    virtual ssize_t receive(char*buffer, size_t bufferLen, int posixFlag) = 0;

    SocketWrapper & getSocketWrapper();

    /**
     * getter method to get port
     * @return port number
     */
    int & getPort();

    /**
     * getter method to get ip
     * @return ip
     */
    const char* getIp();
	
	/**
	*@brief this function set ioctl param to socket.
	*@param cmd. A command to perform on the socket s.
	*@param argp.A pointer to a parameter for cmd.
	*@return return 0 on success and SOCKET_ERROR on failure
	*/ 
	int setIoctl(long cmd, u_long *argp);

#ifdef _WIN32
    int winSocketStartUp() ;
#endif

protected:
    /**
     * Socket of connection wrapping fd, ip, ...
     */
    SocketWrapper socketWrapper;

    /**
     * Add error msg to error list
     * @param errorMsg string explain error
     */
    void addError(int errorCode, const std::string errorMsg = "");

    /**
     * Clear error list
     */
    void flushErrors();

    /**
     * get error message with given ErrorCode
     * @param errCode ErrorCode
     * @return ErrorCode as string
     */
    std::string getErrorMsg(ErrorCode errCode);

private:
    /**
     * Establish socket with given class parameter
     * @return -1 on failure, 0 on success
     */
#ifdef __linux__
    virtual int createSocket() = 0;
#elif _WIN32
    virtual SOCKET createSocket() = 0;
#endif

    /**
     * Contacting list of error. If any error occurres during 
     * process the error should be added to this error
     */
    std::vector<std::pair<int, std::string> > errors;

    int port;
    const char * ip;
};

#endif /* CONNECTION_H */

