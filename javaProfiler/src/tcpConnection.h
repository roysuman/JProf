/*
 * =====================================================================================
 *
 *       Filename:  tecpConnection.h
 *
 *    Description:  tcp connection 
 *
 *        Version:  1.0
 *        Created:  Friday 21 August 2015 02:31:58  IST
 *       Revision:  none
 *       Compiler:  g++4.7
 *
 *         Author:  Suman Roy (), suman.roy@cognizant.com
 *   Organization:  Cognizant Technological Solutions
 *
 * =====================================================================================
 */
#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
#include <iostream>
#include <sys/socket.h>                                           
#include <netinet/in.h>                                           
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>                                                
#include <unistd.h>                                               
#include <errno.h>                                                
#include <cstring>
#include "lock.h"
#include "connection.h"
#define SOCKET_INVALID -1                                        
#define ERROR_SOCKET   -1
#define ERROR_SCOCKET_BIND -1
typedef void (*regisConn_ )(int &);
class TcpConnection{
	public:
		explicit TcpConnection( unsigned short port_ , regisConn_ regisConnIns  ):port(port_),regisConn( regisConnIns ),loopActive(true){}
		bool open( unsigned short port = 0 );
		bool close( void ); // macke sock = -1
		void continueLoop( void *);
		bool sockIsOpen( void);
		void accept(void );
		void stopLoop( void );
		~TcpConnection(){}
		bool isLoopActive( void );
	private:
		unsigned short    port;
		regisConn_ regisConn;
		int       sock;
		bool      loopActive;
		Mutex     socket;
		Mutex     loop;
};
	

#endif
