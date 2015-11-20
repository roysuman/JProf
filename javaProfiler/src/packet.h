#ifndef PACKET_H
#define PACKET_H
//all packet data struct
//Tue Aug 25 19:55:23 IST 2015
//


//HEADER TYPE
//OPCODE { CALLTRACE / HEAP ...}
//TOTAL LENGTH OF PACKET
//OPCODE{IF CALL TRACE THEN- { CALLTRACE DATA/CALLTRACE ENABLED OK / ENABLE CALL TRACE / CALL TRACE ENABLE ERROR / STOP CALL TRACE / STOP CALL TRACE SUCCESS / STOP CALL TRACE ERROR / SUCCESS REPLY OF CALL TRACE RECV }
//DATA OPTIONAL ( CALL TRACE DATA WILL BE THERE FOR OTHER OPTIONS NO DATA NEEDED )
//EOF MARK

//TODO only for callTrace raw data the size of packet is unknown .. else other packets packet size must be known

#define MAX_FRAME_TO_CAPTURE 128

typedef struct packetHead_ packetHead;
struct packetHead_{
	uint16 opCode;
	uint32 size;
};
typedef struct callFrame_ callFrame;
struct onTheFlyCallFrame_{
	size_t     frameNo;
	int64_t    methodId;
	std::string fileName;
	std::string className;
	std::string methodName;
};
// call trace copied from old .h file and renamed

typedef struct {
	int64_t threadId;                   // Env where trace was recorded
	size_t numFrames;                  // number of frames in this trace
	callFrame frames[MAX_FRAME_TO_CAPTURE ];          // frames
}callTrace;

class PacketBase{
	public:
		explicit Packet( ):writePos(0), readPos(0){  }
		~Packet(){ }

		packetHead pHead;
		unsigned char *buffer;
		uint32 size, writePos, readPos;
		void WriteUnsignedInt8(uint8 value) { *(uint8 *)(pBuffer + writePos) = value; writePos += sizeof(uint8); }
		void WriteUnsignedInt32(uint32 value) { *(uint32 *)(pBuffer + writePos) = value; writePos += sizeof(uint32); }
		void WriteUnsignedInt64(uint64 value) { *(uint64 *)(pBuffer + writePos) = value; writePos += sizeof(uint64); }
		void WriteUnsignedInt16(uint32 value) { *(uint16 *)(pBuffer + writePos) = value; writePos += sizeof(uint16); }
		void WriteSignedInt32(int32 value) { *(int32 *)(pBuffer + writePos) = value; writePos += sizeof(int32); }
		void WriteFloat(float value) { *(float *)(pBuffer + writePos) = value; writePos += sizeof(float); }
		void WriteDouble(double value) { *(double *)(pBuffer + writePos) = value; writePos += sizeof(double); }
		void WriteString(const char * str) { uint32 len = static_cast<uint32>(strlen(str)) + 1; memcpy(pBuffer + writePos, str, len); writePos += len; }
		//ADD STRUCT WRITE  METHODS 
		void WriteInt64_t(int64_t value) { *(int64_t *)(pBuffer + writePos) = value; writePos += sizeof(int64_t; }
		void writePacketHead( void );
		void writeCallFrame (  callFrame &cFrame );
		void writeCallTrace ( callFrame &frame );
		void addEOP ( void );


		void writeCallFrame 
		uint8 ReadUnsignedInt8() { uint8 value = *(uint8 *)(pBuffer + readPos); readPos += sizeof(uint8); return value; }
		uint8 ReadUnsignedInt8(uint32 Offset) const { uint8 value = *(uint8 *)(pBuffer + Offset); return value; }
		uint32 ReadUnsignedInt32() { uint32 value = *(uint32 *)(pBuffer + readPos); readPos += sizeof(uint32); return value; }
		uint32 ReadUnsignedInt32(uint32 Offset) const { uint32 value = *(uint32 *)(pBuffer + Offset); return value; }
		void ReadString(char *str) { uint32 len = static_cast<uint32>(strlen((char *)(pBuffer + readPos))) + 1; memcpy(str, pBuffer + readPos, len); readPos += len; }
		void ReadString(char *str, uint32 Offset, uint32 MaxLength) const;
		uint8 ReadInt64_t() { int64_t value = *( int64_t *)(pBuffer + readPos); readPos += sizeof(int64_t ); return value; }
		uint8 ReadInt64_t(uint32 Offset) const { int64_t value = *(int64_t *)(pBuffer + Offset); return value; }

		//ADD STRYCT READ METHODS

		void readPacketHead( void );
		void readCallFrame ( callFrame &cFrame );
		void readCallTrace ( callFrame &frame );
		uint32 GetWritePosition() { return writePos; }
		uint32 GetReadPosition() { return readPos; }
		void SetWritePosition(uint32 newPosition ) { writePos = newPosition; }
		void SetReadPosition(uint32 newPosition ) { readPos = newPosition ; }

		//CREATE PACKET
		bool createCallTraceResponse( uint16); // TODO AFTER RECVING A CALLTRACE DATA-- from the switch case we will be able to know the recv data

		//type and will be able to generate the corresponding response packet

		bool createCallTraceRequest( unint16 );//TODO same before
};

//TODO read data from circuler queue and create a packet instance and then push the data into the send buffer queue.

#endif /* PACKET_H */
