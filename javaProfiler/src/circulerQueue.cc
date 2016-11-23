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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  push
 *  Description:  Push the collected stack trace into queue
 *       @Param:  callTraceItem
 *                  The stack trace data.
 * =====================================================================================
 */
bool CQueue::push(ASGCT_CallTrace& callTraceItem) {
	bool    returnVal;

	mLock->lock();
    //check whether queue is full or not
	if ((readHead == 0 && writeHead == QUEUE_CAPACITY - 1) || writeHead + 1 == readHead) {
		std::cerr<<"Circuler Queue Full"<<std::endl;
		returnVal = false; //TODO some mechanism to avoid data loss
	} else {
        //insert data
		if (writeHead == QUEUE_CAPACITY - 1) {
			writeHead = 0;
		} else {
            writeHead++;
        }
		callTraceBuffer[writeHead].envId = callTraceItem.envId;
		callTraceBuffer[writeHead].numFrames = callTraceItem.numFrames;
		callTraceBuffer[writeHead].frames  = callTraceItem.frames;
		for (jint loop = 0; loop<callTraceItem.numFrames; ++loop) {
			callFrameBuffer[writeHead] [loop].lineno = callTraceItem.frames[loop].lineno;
			callFrameBuffer[writeHead][loop].methodId = callTraceItem.frames[loop].methodId;
		}
		if (readHead == -1) {
            readHead = 0;
        }
		returnVal = true;
	}
    mLock->unLock();
	return returnVal;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  pop
 *  Description:  Pop data from circuler queue.
 * =====================================================================================
 */
bool CQueue::pop(void) {
	bool    returnVal, callFrameRet;
	size_t successFrameData;
	onTheFlyCallFrame callFrame;

	mLock->lock();
    if (readHead == -1) {
        returnVal = false;
    } else {
        successFrameData = 0;
        std::cout<<"-------\t---------\t------------------\t-------\n";
        std::cout<<"Frame Count = "<<callTraceBuffer[readHead].numFrames<<std::endl;
#ifdef DEBUG
        std::cout<<"\n1\t"<<callTraceBuffer[readHead].numFrames;
#endif
        for (jint loop = 0; loop < callTraceBuffer[readHead].numFrames; ++loop) {
            callFrameRet = frameInfo(jvmti, callFrameBuffer[readHead][loop].methodId, callFrameBuffer[readHead][loop].lineno, callFrame);
            //clear all data from the current frame
			memset(&callFrameBuffer[readHead][loop], 0,  sizeof(ASGCT_CallFrame));
            if (callFrameRet) {
                callFrame.frameNo = successFrameData++;
#ifdef SEEBUG
				std::cout<<"\nMethodId [ "<<callFrame.methodId
					<<" ] FileName [ "<<callFrame.fileName
					<<" ] Classname [ "<<callFrame.className
					<<" ] Method Name [ "<<callFrame.methodName
					<<" ] FrameNo [ "<<callFrame.frameNo<<"]"<< std::endl;
#endif
                std::cout<<"\t"<<callFrame.className.c_str()<<"."
                    <<callFrame.methodName.c_str()
                    <<"("<<callFrame.fileName.c_str()
                    <<":"<<callFrame.lineNumber
                    <<")"<<std::endl;
            }
        }
		if (readHead == writeHead) {
            readHead = writeHead = -1;
        } else {
            if (readHead == QUEUE_CAPACITY - 1) {
				readHead = 0;
            } else {
                readHead ++;
            }
		}
		returnVal = true;
	}
    mLock->unLock();
	return returnVal ;
}
