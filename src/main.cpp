/*
 * @author          Mohammad Reza Mousavi <smr.mousavi87@gmail.com>
 * @date            19th March 2022
 *
 * @project         Connection
 */

#include <cstdio>
#include <cstdlib>
#include "ConnectionTcp.h"

#define RAW_PACKET_SIZE 100
#define SOCKET_RECEIVE_LEN 100


int main(int argc, char** argv) {
    
	int port= 1212;
	const char* ip = "127.0.0.1";
	ConnectionTcp* connection = new ConnectionTcp(port, ip);

#ifdef _WIN32
    if (connection->winSocketStartUp() < 0) {
        ZF_LOGE("Failed to startup win socket.");
        returnCode -1;
    }
#endif
	
	//connect to server...
	connection->connect();
	unsigned char packet[RAW_PACKET_SIZE] = {0};
	 
	 //send packet to server...
	int ret = connection->send(reinterpret_cast<char*> (packet), len, 0);
    if (ret != len) {
        printf("Error while sending packet..., %d %s\n", ret, strerror(errno));
        retNo= -1;
    }
	
	char recvBuffer[SOCKET_RECEIVE_LEN] = {0};
	int rv = 0;
	int timeout =50000;
	
	 //receive packet from server...
	while (1) {
		rv = connection->receive(recvBuffer, length, 0);
#ifdef __linux__
		usleep(10000); //10 ms
#elif _WIN32
		sleep(10);//10 ms
#endif
		  
		if (rv > 0) 
			break;
		
		if (--timeout < 0) {
			ZF_LOGE("timeout %d  occurred for recv data.", timeout);
			break;
		}
	
	}
		
	connection->disconnect();
	delete connection;
	
    return 0;
}

