/*
 * =====================================================================================
 *
 *       Filename:  circulerQueue.h
 *
 *    Description: A circuler queue to grab the calltrace STACK data captured by asyngetcall trace
 *
 *        Version:  1.0
 *        Created:  Monday 17 August 2015 09:30:30  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ROY (), email.suman.roy@gmail.com
 *   Organization:  NA
 *
 * =====================================================================================
 */
#ifndef CIRCULERQUEUE_H
#define CIRCULERQUEUE_H
#include "stacktrace.h"
#include "global.h"
#include "onTheFlyData.h"
#include "connection.h"
#include <mutex>
#define QUEUE_CAPACITY 1024
#define BUFFER_SIZE                524288
typedef bool ( *sendData_ ) ( unsigned char * , size_t &size );
typedef bool ( *getFrameInfoPtr)(jvmtiEnv *jvmti ,  const jmethodID &, onTheFlyCallFrame&); 
class CQueue{
	public:
		explicit CQueue(jvmtiEnv *jvmti_, getFrameInfoPtr frameInfo_ , sendData_ sendDataIns):frameInfo( frameInfo_),jvmti(jvmti_),sendData( sendDataIns ),readHead(-1),writeHead(-1) {   

		}
		~CQueue(){}
		getFrameInfoPtr frameInfo;
		bool push( ASGCT_CallTrace &);
		bool pop(void );
		jvmtiEnv    *jvmti;
		unsigned char buffer[ BUFFER_SIZE ];
		size_t    bufferIndex;
		sendData_  sendData;
	private:
		int    readHead;
		int    writeHead;
		ASGCT_CallTrace callTraceBuffer [ QUEUE_CAPACITY ];
		ASGCT_CallFrame callFrameBuffer[ QUEUE_CAPACITY ] [ MAX_FRAME_TO_CAPTURE ];
		pthread_mutex_t lock;
	
//		void getFrameInfo( jvmtiEnv* , const ASGCT_CallFrame &callFrame );



};
#endif
