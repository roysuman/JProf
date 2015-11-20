//Sun Aug 23 19:20:20 IST 2015
#include "tcpConnection.h"

bool
TcpClient::establishConnection( void ){
	bool         returnVal;
	truct sockaddr_in serverAddr; 
	socklen_t    addr_size;

	returnVal = true;
	connection  = socket(AF_INET, SOCK_STREAM, 0);
	if ( connection == -1 ){
		std::cerr<<"Not able to create socket"<<std::endl;
		returnVal = false;
	}
	if ( returnVal ){
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port);
		serverAddr.sin_addr.s_addr = inet_addr(serverIp);
		memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
		addr_size = sizeof serverAddr;
		connect( connection  , (struct sockaddr *) &serverAddr, addr_size) == 0 ? returnVal = true:returnVal = false;
		if ( !returnVal){
			std::cerr<<"Not abble to establish connection with server:"<<std::endl;
			connection = -10;
		}
	}
	return returnVal;
}

bool
TcpClient::closeConnection( void ){
	bool returnVal;

	if ( connection >= 0 ){
		::close ( connection ) == 0 ? returnVal = true : returnVal = false;
	}
	return returnVal;
}

bool
TcpClient::send ( const unsigned char * data , const size_t &length ){
	bool    returnVal;
	int  status;

	returnVal = true;

	if ( connection > 0 ){
		status = ::send ( connection , data , length , 0 );
		if ( status < 0 ) {
			std::cerr<<"Error on sending data\n";
		returnVal = false;
		} else if ( connection == 0 ){
			std::cerr<<"Connection closed\n";
			returnVal = false;	
			connection = -1;
		}
	}else returnVal = false;
	return returnVal;
}
bool
TcpClient::recv(void ){
	bool    returnVal;
	int     status;
	if ( connection > 0 ){
		status = recv ( connection , ( char*)&recvBuffer[ 0 ] ,TCP_BUFFER_SIZE, 0 );
		if ( status > 1 );
			//TODO process data
		else if ( status == ERROR_SOCKET ){
			std::cerr<<"Error on receving data\n"<<std::endl;
			returnVal = false;
		}
		else if ( status == 0 ){
			std::cerr<<"Connection closed\n";
			returnVal = false;
		}
	}else returnVal = false;
	return returnVal;
}
TcpClient::~TcpClient( void ){
	closeConnection( );
}






