/*
 * =====================================================================================
 *
 *       Filename:  tecpConnection.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  Friday 21 August 2015 03:12:00  IST
 *       Revision:  none
 *       Compiler:  g++4.7
 *
 *         Author:  Suman Roy (), suman.roy@cognizant.com
 *   Organization:  Cognizant Technological Solutions
 *
 * =====================================================================================
 */
#include "tcpConnection.h"


void 
TcpConnection::stopLoop( void ){
	//TODO lock
	if ( loopActive ) loopActive = false;
	//TODO unlock 
}

bool
TcpConnection::isLoopActive( void ){
	bool    active;
	//TODO mutex
	active = loopActive;
	//TODO mutex unlock
	return active;
}

void
TcpConnection::continueLoop( void * thisObj_ ){
	TcpConnection *thisObj = ( TcpConnection *)thisObj_;
	//TODO lock for looping
	while ( thisObj->isLoopActive() ){
		//TODO SLEEP 
		thisObj->accept();
	}
	//TODO unlock
}

bool
TcpConnection::open( unsigned short portIn ){
	bool    returnVal;
	struct sockaddr_in address;

	returnVal = true;

	if ( sock != 0 ){
		std::cerr<<"Socket already open"<<std::endl;
		returnVal = false;
	}
	if ( returnVal ){
		if ( portIn != 0 ) port = portIn;
		
		memset((char *) &address, 0, sizeof(address));                    
		address.sin_family = AF_INET;                                     
		address.sin_port = htons( port);                                  
		address.sin_addr.s_addr = htonl(INADDR_ANY);    

		sock = ::socket(AF_INET, SOCK_STREAM, 0);                           
		if (sock == SOCKET_INVALID ) {                                     
			std::cerr<<"Invalid socket\n";
			returnVal = false;
		}
	}
	if ( returnVal ){
		if (bind(sock, (struct sockaddr *) &address, sizeof(address)) == ERROR_SCOCKET_BIND ) {
			::close(sock);
			sock = 0;
			std::cout<<"Bind <0\n";
			returnVal = false;
		}else{
	//		int socketBufsize = 64 * 1024;
	//		setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*) &bufsize, sizeof(bufsize));
	//		fcntl(sock, F_SETFL, O_NONBLOCK);
			if (listen(sock, SOMAXCONN) == ERROR_SOCKET ) {
				::close(sock);
				std::cerr<<"Error on listen socket\n";
				returnVal = false;
				sock = 0;
			}
		}
	}
	return returnVal;
}
void
TcpConnection::accept( void ){
	bool    returnVal;
	struct sockaddr_in fromAddress;
	unsigned int fromAddressLen;

	fromAddressLen = sizeof ( fromAddress );
	int    tempSock;
	fromAddress.sin_family = AF_INET;
	if ( sock == ERROR_SOCKET ){
		returnVal =- false;
	}
	if ( returnVal ){
		while ( ( tempSock = ::accept( sock , (struct sockaddr *)&fromAddress , &fromAddressLen ) ) != SOCKET_INVALID ){
			regisConn( tempSock );
			//TODO stop accepting any other connection
			//server can support a sinlge connection at a time
		}
	}
}	

bool
TcpConnection::close( void  ){
	bool    returnVal;
	returnVal = false;
	if ( sock ) returnVal = ( bool) !::close( sock );
	sock = 0;
	return returnVal;
}

