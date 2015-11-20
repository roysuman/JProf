//Sun Aug 23 19:20:20 IST 2015
#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
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
#define TCP_BUFFER_SIZE 524288
class TcpClient{
	public:
		explicit TcpClient( const unsigned short port_, const char * serverIp_ ):port(port_),serverIp(serverIp_),connection(-1){ }

		bool establishConnection ( void );
		bool closeConnection( void );
		bool send( const unsigned char * , const size_t & );

		bool recv( void ); 
		virtual bool processData( const char* , size_t & )= 0;
		~TcpClient();
	private:
		int    connection;
		unsigned short port;
		char *         serverIp;
		unsigned char * recvBuffer[ TCP_BUFFER_SIZE ];
};
#endif /* TCPCONNECTION_H */
