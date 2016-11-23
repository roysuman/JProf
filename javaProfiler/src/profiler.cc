/*
 * =====================================================================================
 *
 *       Filename:  profiler.cpp
 *
 *    Description: ref profiler.h 
 *
 *        Version:  1.0
 *        Created:  Saturday 15 August 2015 01:56:19  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ROY (), email.suman.roy@gmail.com
 *   Organization:  NA
 *
 * =====================================================================================
 */

#include "profiler.h"

// Creates a new jthread

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  allocThread
 *  Description: Create a java thread which will run entirely in native code.
 *       @Param: jniEnvironment
 *                 The JNI enviroment.
 *      @Return: Returns a jthread instance.
 * =====================================================================================
 */
// TODO: If thread creation fails then,how to handle return value?? 
static jthread allocThread(JNIEnv *jniEnvironment) {
    const jclass threadClass = jniEnvironment->FindClass("java/lang/Thread");

	if (threadClass == NULL) {
		std::cerr<<"Cannot find Thread class\n";
	}
	const jmethodID constructorMethod = jniEnvironment->GetMethodID(threadClass, "<init>", "()V");
	if (constructorMethod == NULL) {
		std::cerr<<"Cannot find Thread constructor method\n";
	}
	const jthread threadObject = jniEnvironment->NewObject(threadClass, constructorMethod);
	if (threadObject == NULL) {
		std::cerr<<"Cannot create new Thread object";
	} else {
        const jmethodID methodId = jniEnvironment->
            GetMethodID(threadClass, "setName", "(Ljava/lang/String;)V");
        jniEnvironment->CallObjectMethod(threadObject, methodId,
                jniEnvironment->NewStringUTF("Honest Profiler Daemon Thread"));
    }
    return threadObject;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sleepFor
 *  Description:  Suspend execution of caller thread in microsecond interval.
 *       @Param:  sleepTime
 *                  Time to suspend execution.
 *      @Return:  Return 0 on success, -1 on error.
 * =====================================================================================
 */
const int JavaProfiler::sleepFor(const unsigned int &sleepTime) {
	return usleep(sleepTime * 1); //TODO: handle error
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  worker
 *  Description:  The worker method, whose responsibility is to provide
 *                the final outcome.
 * =====================================================================================
 */
void JavaProfiler::worker(void) {
	while (true) {
		circulerQueue->pop();
		sleepFor(1);
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  startCallBack
 *  Description:  start the execution of the agent thread.
 * =====================================================================================
 */
void JavaProfiler::startCallBack(void) {
	// method must be called by the signal handler
	jvmtiStartFunction callbackFunction = 
		[](jvmtiEnv *jvmtiEnvironment, JNIEnv *jniEnvironemnt, void *arg) {
	      JavaProfiler  *profiler = (JavaProfiler*) arg;
	      profiler->worker();
		};
    // REF: https://docs.oracle.com/javase/7/docs/platform/jvmti/jvmti.html#RunAgentThread
	const jvmtiError err = jvmti->RunAgentThread(allocThread(getJNIEnv()), 
			callbackFunction, this, JVMTI_THREAD_NORM_PRIORITY);
    if (err != JVMTI_ERROR_NONE) {
        std::cerr<<"Execution of Agent thread failed with error "<<err<<std::endl;
    }
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  startProfiler
 *  Description:  start the profiler
 * =====================================================================================
 */
bool JavaProfiler::startProfiler(void) {
	// modify the singnal handler for SIGPROF signal
	sigHandler.setAction(&sigHandler_);
	startCallBack();
	sigHandler.updateIntervalTime();
	return true;
}

void JavaProfiler::stopProfiler(void) {
    signal(SIGPROF, SIG_IGN);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  readCallTrace
 *  Description:  Grab all stack trace present in JVM. The singnal handler method
 *                invokes this method in a timeinterval on occurance of SIGPROF
 *                signal.
 *       @Param:  signum
 *                  The signalnumber, this method gets invoke on occurrence of this
 *                  signal.
 *       @Param:  info
 *                  The siginfo_t structure is passed as the second parameter.
 *       @Param:  context
 *                  The profiler context.
 * =====================================================================================
 */
void JavaProfiler::readCallTrace(int sigNum, siginfo_t *sigInfo, void* context) {
	ASGCT_CallFrame    frames[MAX_FRAME_TO_CAPTURE];
	ASGCT_CallTrace    callTrace;
	// TODO: init the frame to zero
    // memset ( frames , sizeof( ASGCT_CallFrame ) * MAX_FRAME_TO_CAPTURE );
	callTrace.frames = frames;
	JNIEnv* jniEnv = getJNIEnv();
	if (jniEnv == NULL) {
		// TODO raise an error
	} else {
		callTrace.envId = jniEnv;
		asynGCTType asynGCT = AsynGCT::getAsynGCT();
		(*asynGCT)(&callTrace, MAX_FRAME_TO_CAPTURE, context);
		if (callTrace.numFrames > 0) {
            circulerQueue->push(callTrace);
#ifdef DEBUG
            std::cout<<"Pushed data in Queue\n"
                <<"Calltrace:\n"
                <<"Frame count [ "<<callTrace.numFrames<<"          ]\n"
                <<"METHOD ID   [ "<<callTrace.frames[0].methodId<<" ] "<<std::endl;
#endif
		}
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getJNIEnv
 *  Description:  get the JNI enviroment poointer of caller thread.
 *      @Return:  return an instance(pointer) of JNIEnv.
 * =====================================================================================
 */
JNIEnv* JavaProfiler::getJNIEnv(void){
	JNIEnv * jniEnv;
    const int returnVal = jvm->GetEnv((void **)&jniEnv, JNI_VERSION_1_6);
	if (returnVal == JNI_EVERSION || returnVal == JNI_EDETACHED ){
		jniEnv = NULL ; 
	}
	return jniEnv;
}
