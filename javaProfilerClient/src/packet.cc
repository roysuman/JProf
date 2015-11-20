/*
 * =====================================================================================
 *
 *       Filename:  packet.cc
 *
 *    Description:  create network packet and process received net packet 
 *
 *        Version:  1.0
 *        Created:  Monday 24 August 2015 12:14:13  IST
 *       Revision:  none
 *       Compiler:  g++4.7
 *
 *         Author:  Suman Roy (), suman.roy@cognizant.com
 *   Organization:  Cognizant Technological Solutions
 *
 * =====================================================================================
 */
#include "packet.h"


/* 
unsigned char * Packet::readBuffer = NULL;
unsigned char* Packet::sendBuffer = NULL';
size_t Packet::readBufferHead = 0;
size_t packet::sendBufferHead = 0;

*/
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  createPacket
 *  Description:  create packet to send the server
 *              you might think that why i have made this complex by using switch , whereas am doingthe same work... 
 *              in future we might have add some extra flag with data point
 *              that time it will be easy to handle with existing code
 * =====================================================================================
 */
bool
Packet::createPacket( unsigned long  int &typeOpcode , unsigned int & fieldOpcode , const unsigned char *data , size_t &length ){

	bool returnVal = true;
	sendBufferHead = 0;
	//eachg time 

	switch ( typecode ){
		sendBufferHead +=EncDecode<unsigned int>::encode ( typeOpcode , sendBuffer , sendBufferHead );
		case JFLG_CALL_TRACE: // it's a call trace packet
			switch ( fieldOpcode ){
				case CALLTRACE_START: // gen packet to start the calltrace
				case CALLTRACE_STOP:
				case CALLTRACE_ERROR:
				case CALLTRACE_CLIENT_RESPONSE_OK:
					sendBufferHead +=EncDecode<unsigned int>::encode (  fieldOpcode , sendBuffer , sendBufferHead );
					//TODO send the data and change the flag
					tClient.send ( sendBuffer , sendBufferHead );
					break;
			}
		default:
			std::cout<<"Have to implement\n";
			returnVal = false;
	}
	return returnVal;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  processPacket
 *  Description:  process the recv packet from server.
 *                and dump the appro packet to the dedicated queue of that packet type. Currenly only one queue to stotr the callTrace.
 * =====================================================================================
 */

bool
Packet::processPacket( const unsigned char* data , size_t &length ){
	unsigned int typeOpcode , fieldOpcode;
	bool    returnVal ;
	size_t bufferIndex ;

	//INIT VAR
	bufferIndex = 0;
	returnVal = true;
	typeOpcode = EncDecode<unsigned int >::decode ( data , bufferIndex );
	fieldOpcode = EncDecode< unsigned int >::decode( data ,++bufferIndex );
	switch ( typeOpcode ){
		case JFLG_CALL_TRACE: // it's a call trace packet
			switch ( fieldOpcode ){
				case CALLTRACE_START: // gen packet to start the calltrace
					//TODO call trace started .. prepare environment to accept the callframes
					//DONE
					returnVal = setCallTraceFlag( );
					break;
				case CALLTRACE_STOP:
					//TODO calltrace stoped
				case CALLTRACE_ERROR:
					//TODO for both stop and error .. process the already available data in calltrace queue and reset the environment.
					returnVal = resetCallTraceFlag( );
					break;

				case CALLTRACE_SERVER_RESPONSE_OK:
					//TODO send packet from client //made an impact on server.
					break;
				case CALLTRACE_DATA:
					//TODO the data that need to store the callTrace queue
					returnVal = disectCallTraceProto( data , ++bufferIndex , length , trace ); 
					break;
			}
		default:
			std::cout<<"Now this error should not be gen..Handle it\n";
			returnVal = false;
	}
	return returnVal;
}

