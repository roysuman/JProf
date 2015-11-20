/*
 * =====================================================================================
 *
 *       Filename:  packetSupport.cc
 *
 *    Description: for info read corresponding header file 
 *
 *        Version:  1.0
 *        Created:  Monday 24 August 2015 02:43:25  IST
 *       Revision:  none
 *       Compiler:  g++4.7
 *
 *         Author:  Suman Roy (), suman.roy@cognizant.com
 *   Organization:  Cognizant Technological Solutions
 *
 * =====================================================================================
 */
#include "packetSupport.h"
bool
PacketBase::disectCallTraceProto( const char * data , size_t &bufferIndex , size_t &length , callTrace &trace ){
	bool returnVal;

	size_t    callFrameIndex;
	size_t    packetLength;
	size_t    ecLength;
	char *    ecData;
	size_t   effectiveBufferIndex;

	returnVal = true;

	packetLength = letoh24 ( data + bufferIndex );
	bufferIndex += 3;

	effectiveBufferIndex = bufferIndex; 

	callFrameIndex = 0;
	trace.threadID = EncDecode<int64_t>::decode( data , bufferIndex );

	//TODO check the remaining length before extracting any data.. and on error set the returnVal
	do{
		trace.frames[ callFrameIndex].methodId = EncDecode<int64_t>::decode( data , bufferIndex );

		ecLength = letoh24( data );
		bufferIndex += 3;
		memcpy ( (void*)( data + bufferIndex ) , &ecData , ecLength );
		data [ ecLength + 1 ] = '\0';
		trace.frames[callFrameIndex].fileName = std::string(data);
		bufferIndex += ecLength;

		ecLength = letoh24( data );
		bufferIndex += 3;
		memcpy ( (void*)(data + bufferIndex ), &ecData , ecLength );
		data [ ecLength + 1 ] = '\0';
		trace.frames[callFrameIndex].className = std::string(data);
		bufferIndex += ecLength;

		ecLength = letoh24( data );
		bufferIndex += 3;
		memcpy ( (void*)(data + bufferIndex) , &ecData , ecLength );
		data [ ecLength + 1 ] = '\0';
		trace.frames[callFrameIndex].methodName = std::string(data);
		bufferIndex += ecLength;

		trace.frames[ callFrameIndex++].frameNo = EncDecode<size_t>::decode( data , bufferIndex );
	}while ( isLengthRemaining ( bufferIndex , length + effectiveBufferIndex  ) ); 
	trace.numFrames =  ( callFrameIndex - 1 ) ;
	return returnVal;
}
