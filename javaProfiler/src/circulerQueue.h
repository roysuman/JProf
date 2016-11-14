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
#include <mutex>
#define QUEUE_CAPACITY 1024
#define BUFFER_SIZE    524288
typedef bool (*getFrameInfoPtr)(jvmtiEnv *jvmti, const jmethodID&, onTheFlyCallFrame&);

class CQueue {
	public:
		explicit CQueue(jvmtiEnv *jvmti_, getFrameInfoPtr frameInfo_):frameInfo(frameInfo_),jvmti(jvmti_),readHead(-1),writeHead(-1) {}
		virtual ~CQueue() {}
		getFrameInfoPtr frameInfo;
		bool push(ASGCT_CallTrace &);
		bool pop(void);
		jvmtiEnv    *jvmti;
		unsigned char buffer[ BUFFER_SIZE ];
		size_t    bufferIndex;
	private:
		int    readHead;
		int    writeHead;
		ASGCT_CallTrace callTraceBuffer[QUEUE_CAPACITY];
		ASGCT_CallFrame callFrameBuffer[QUEUE_CAPACITY][MAX_FRAME_TO_CAPTURE];
		pthread_mutex_t lock;
};
#endif
