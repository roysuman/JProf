#ifndef PACKET_H
#define PACKET_H 
//Sun Aug 23 22:12:59 IST 2015
#include <iostream>
#include<string.h>
#include "opcode.h"
#include "encDec.h"
#include "structure.h"
#define BUFFER_SIZE                524288
class Packet: protected PacketBase {
	public:
		explicit  Packet( TcpClient &tClient_ ):tClient(tClient_),readBufferHead(0), sendBuffer(0){}
		explicit ~packet(){}
		bool createPacket(unsigned long &, unsigned int &, const unsigned char * , size_t & );
		bool processPacket ( const unsigned  char * , size_t& );
	private:
		unsigned char readBuffer [ BUFFER_SIZE ];
		unsigned char sendBuffer[ BUFFER_SIZE ];
		size_t  readBufferHead;
		size_t  sendBufferHead;
		callTrace    trace;
		TcpClient    &tClient;
};
		

#endif /* PACKET_H */
