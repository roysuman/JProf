#include "connection.h"

size_t Connection::sendBufferIndex = 0;
size_t Connection::recvBufferIndex = 0;
int Connection::connectionId = -1;
unsigned char Connection::sendBuffer [ TCP_BUFFER_SIZE ];
unsigned char Connection::readBuffer [ TCP_BUFFER_SIZE ];
void
Connection::registerConnectionHandler( int &conId ){
	//TODO muex
	connectionId = conId;
	//TODO unlock
}

bool
Connection::sendData( unsigned char * data , size_t &size ){
	int    status;
	bool      returnVal;

	returnVal = true;
	if ( connectionId == 0 ) returnVal = false;
	if ( returnVal ){
		status = send ( connectionId , data , size , 0 );
		if ( status < 0 ){
			std::cerr<<"Error on sending data to connection:"<<std::endl;
			returnVal = false;
		} else if ( status == 0 ){
			std::cerr<<"Connection closed\n";
			connectionId = 0;
			returnVal = false;
		}
	}
	return returnVal;
}

bool
Connection::recvData( void ){
	bool    returnVal;
	int   status;

	returnVal = true;
	if ( connectionId == 0 ) returnVal = false;
	if ( returnVal ){
		status = recv ( connectionId , ( char*)&readBuffer[recvBufferIndex] ,TCP_BUFFER_SIZE, 0 );
		if ( status > 1 );
		//	processData ( processRecvData ( readBuffer , status ) );
		else if ( status == ERROR_SOCKET ){
			std::cerr<<"Error on receving data\n";
			returnVal = false;
		} else if ( status == 0 ){
			std::cerr<<"Connection closed\n";
			returnVal = false;
		}
	}
	return returnVal;
}





