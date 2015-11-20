/*
 * =====================================================================================
 *
 *       Filename:  circulerQueue.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  Monday 17 August 2015 09:42:45  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ROY (), email.suman.roy@gmail.com
 *   Organization:  NA
 *
 * =====================================================================================
 */
#include "circulerQueue.h"
bool
CQueue::push ( callTrace &callTraceItem ){
#ifdef DEBUG
	std::cout<<"MethodId1 [ "<<callTraceItem.frames[0].methodId<<" ] "<<std::endl;
#endif
	bool    returnVal;
	pthread_mutex_lock( &lock);

	//check whether queue is full or not
	if ( ( readHead == 0 && writeHead == QUEUE_CAPACITY - 1) || writeHead + 1 == readHead ){
		std::cout<<"Circuler Queue Full"<<std::endl;
		returnVal = false; //TODO some mechanism to avoid data loss
	}else{
		//insert data
		if ( writeHead == QUEUE_CAPACITY - 1 ){
			writeHead = 0;
		}else writeHead ++;
		callTraceBuffer[writeHead].threadID = callTraceItem.threadID;
		callTraceBuffer[writeHead].numFrames = callTraceItem.numFrames;
		callTraceBuffer[ writeHead ].frames  = callTraceItem.frames;
		for ( jint loop = 0 ; loop < callTraceItem.numFrames ; ++ loop ){
			callFrameBuffer[ writeHead ] [ loop ].frameNo = callTraceItem.frames[loop].frameNo;
			callFrameBuffer[ writeHead ] [ loop ].methodId = callTraceItem.frames[loop].methodId;
			callFrameBuffer[ writeHead][ loop ].fileName = callTraceItem.frames[ loop ].fileName;
			callFrameBuffer[ writeHead][ loop ].className = callTraceItem.frames[ loop ].className;

			callFrameBuffer[ writeHead][ loop ].methodName = callTraceItem.frames[ loop ].methodName;
		}
		if ( readHead == -1 ) readHead = 0;
		returnVal = true;
	}
	pthread_mutex_unlock(&lock);
	return returnVal;
}
bool
CQueue::pop( void ){  // TODO future flag value must be come from caller method
//	std::cout<<"READ HEAD [ "<<readHead<<" ] \n";
	bool    returnVal , callFrameRet;
	size_t successFrameData;
	unsigned int    flag= 0x00;
	size_t     bufferHead = 0 ;//sizeof( flag );
	size_t     bufferHeadToInsertTheLength;
	size_t     length;
	onTheFlyCallFrame callFrame;
	pthread_mutex_lock(&lock);
	if ( readHead == -1 ) returnVal = false;
	else{
		if ( readHead == writeHead ) readHead = writeHead = -1;
		else {
			if ( readHead == QUEUE_CAPACITY - 1 )
				readHead = 0;
			else readHead ++;
		}
		returnVal = true;
	}
	pthread_mutex_unlock( &lock );
	return returnVal ;
}

