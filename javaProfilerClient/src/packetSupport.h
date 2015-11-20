/*
 * =====================================================================================
 *
 *       Filename:  packetSupport.h
 *
 *    Description:  support methodes for packets
 *
 *        Version:  1.0
 *        Created:  Monday 24 August 2015 01:07:16  IST
 *       Revision:  none
 *       Compiler:  g++4.7
 *
 *         Author:  Suman Roy (), suman.roy@cognizant.com
 *   Organization:  Cognizant Technological Solutions
 *
 * =====================================================================================
 */
#include<iostream>
#include<stdlib.h>
#include<cstring.h>
#include "structure.h"
#include "globl.h"

class PacketBase{
	public:
		explicit PacketBase( ENV_READY = 0; ){}
		~PacketBase();
	protected:
		bool setCallTraceFlag ( void ){
			bool returnVal;
			returnVal = true;
			if ( ENV_READY & CLIENT_RDY_CALLTRACE ) {
				std::cerr<<"Atempting to set the call trace flag which is already set: ERROR\m";
				returnVal = false;
			}else ENV_READY |= CLIENT_RDY_CALLTRACE;
			return returnVal;
		}
		bool resetCallTraceFlag( void ){
			bool   returnVal;
			if ( ENV_READY & CLIENT_RDY_CALLTRACE ){
				ENV_READY = ENV_READY & ( ~CLIENT_RDY_CALLTRACE );
				returnVal = true;
			}else {
				std::cerr<<"Attempting to reset the call trace flag which was not set: ERROR\n";
				returnVal = false;
			}
			return returnVal;
		}
		bool disectCallTraceProto ( const char * , size_t &bufferIndex , size_t &length , callTrace &trace );
		bool isLengthRemaining( size_t bufferIndex , size_t totalLength ){
			if ( bufferIndex < totalLength ){
				return true;
			}return false;
		}
};
