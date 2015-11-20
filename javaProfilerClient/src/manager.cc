//Mon Aug 24 21:45:49 IST 2015
#include "manager.h"

bool
ClientManager::initClient( void ){
	bool    returnVal;

	returnVal = client->establishConnection();

	return returnVal;
}

bool
ClientManager::disconnectClient( void ){
	bool    returnVal;

	returnVal = client->closeConnection();

	return returnVal;
}

bool
ClientManager::sendCallTraceCaptureRequest( void ){
	bool    returnVal;

	returnVal = pack->createPacket( JFLG_CALL_TRACE ,  CALLTRACE_START , NULL , 0 );
	return returnVal;
}

bool
ClientManager::stopCallTraceCaptureRequest( void ){
	bool    returnVal;

	returnVal = createPacket( JFLG_CALL_TRACE , CALLTRACE_STOP , NULL , 0 );
	return returnVal;
}
