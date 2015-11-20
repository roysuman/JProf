#ifndef CONNECTION_H
#define CONNECTION_H 

#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define SOCKET_INVALID -1                                        
#define ERROR_SOCKET   -1
#define ERROR_SCOCKET_BIND -1
#define TCP_BUFFER_SIZE                524288

//TODO only one instance for this class will be created in profiler.h

//TODO a logic for tcpConnection.h accept loopback... when a connection is active
//then the server can't accept any new connection from the client.
//so when the connection got closed then some flag need to set to indicate the 
//loop back that it'r elih=goble to accept any new connnection.

//TODO TCP DISCONNECTED means only send some dummy packet to maintain the connection -- NOTO no need to create the instance of packet to 
//generate this packet
class Connection{
	protected:
			typedef enum {
				TCP_CONNECTED = 0,
				TCP_DISCONNECTED = 10,
				TCP_CLOSED = 20
			}tcpState_;
	public:
		explicit Connection( void ){};//for server
		~Connection( void ){}
		bool sendQueuePush( unsigned char  * , size_t &size );
		void registerConnectionHandler ( int &conId );
	protected:
		
		static int connectionId; // needed to comunicate with sock.
		unsigned char *sendBuffer ;
		unsigned char *recvBuffer;
		size_t    sendBufferIndex;
		size_t    recvBufferIndex;
		size_t    sendBufferSize;
		size_t    recvBufferSize;
		tcpState_    tState;
		//once dend done change the mode on recv by enum
		bool initSendRecv( void ); // thread for send and recv with the help of connection var and create a loop tcp_connection.cpp
		bool send( void );
		bool recv( void );
		bool processRecvData( unsigned char * , size_t  );//TODO process data has all switch case combinations
		//if the data is a ENABLE and DISABLE DATA then Generate the CORRESPONDING RESPONSE DATA AND SEND BACK TO THE CLIENT..
		//SAME LOGIC FOR SERVER.
		

};

	          	

#endif /* TCPCONNECTION_H */
