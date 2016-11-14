/*
 * =====================================================================================
 *
 *       Filename:  profiler.cpp
 *
 *    Description:  
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

//Creates a new jthread
static jthread 
allocThread(JNIEnv *jniEnvironment) {
#ifdef DEBUG
	std::cout<<"################## ALLOC THREAD @@@@@@@@@@@@@@@"<<std::endl;
#endif
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
        const jmethodID methodId = jniEnvironment->GetMethodID
			(threadClass, "setName", "(Ljava/lang/String;)V");
        jniEnvironment->CallObjectMethod(threadObject, methodId, jniEnvironment->NewStringUTF("Honest Profiler Daemon Thread"));
    }
    return threadObject;
}

void 
JavaProfiler::sleepFor(const unsigned int &sleepTime) {
	usleep(sleepTime * 1);//TODO handle return val
}

void
JavaProfiler::worker() {
	//wait for connection...
	//and after receiving the connection .. start monitoring JV
	//TODO some global variable set must be done by -- after analyzing the received pack from client
//        tcpSock->continueLoop(tcpSock ); 
	
	while (1) {
		circulerQueue->pop();
		sleepFor(1);
//		sigHandler.updateIntervalTime ( );

	}
}

void
JavaProfiler::startCallBack() {
	//method must be called by the signal handler
	jvmtiStartFunction callbackFunction = 
		[](jvmtiEnv *jvmtiEnvironment, JNIEnv *jniEnvironemnt, void *arg) {
	      JavaProfiler  *profiler = (JavaProfiler*) arg;
	      profiler->worker();
		};
	const jvmtiError err = jvmti->RunAgentThread(allocThread(getJNIEnv()), 
			callbackFunction, this, JVMTI_THREAD_NORM_PRIORITY);
    if (err != JVMTI_ERROR_NONE) {
        std::cerr<<err<<std::endl;
    }

	//TODO check error
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  startProfiler
 *  Description:  start the profiler
 * =====================================================================================
 */
bool
JavaProfiler::startProfiler() {
	//set action
	sigHandler.setAction(&sigHandler_);

	startCallBack();
	sigHandler.updateIntervalTime();
	return true;
}

void
JavaProfiler::readCallTrace(int sigNum, siginfo_t *sigInfo, void * context) {
	ASGCT_CallFrame    frames[MAX_FRAME_TO_CAPTURE];
	ASGCT_CallTrace    callTrace;
	//TODO init the frame to zero
//	memset ( frames , sizeof( ASGCT_CallFrame ) * MAX_FRAME_TO_CAPTURE );
	callTrace.frames = frames;
	JNIEnv* jniEnv = getJNIEnv();
	if (jniEnv == NULL) {
		//raised an error
	} else {
		callTrace.envId = jniEnv;
		asynGCTType asynGCT = AsynGCT::getAsynGCT();
		(*asynGCT)(&callTrace, MAX_FRAME_TO_CAPTURE, context);
		//print stack trace
        std::cout<<"Calltrace: Frame count [ "<<callTrace.numFrames<<"] "<<std::endl;
		if (callTrace.numFrames > 0) {
			std::cout<<"METHOD ID [ "<<callTrace.frames[0].methodId<<" ] "<<std::endl;
		}
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getJNIEnv
 *  Description:  get the JNI enviroment poointer value for this thread
 * =====================================================================================
 */
JNIEnv*
JavaProfiler::getJNIEnv( void ){
	JNIEnv * jniEnv;
	int    returnVal;
	 returnVal = jvm->GetEnv((void **)&jniEnv, JNI_VERSION_1_6);
	if ( returnVal == JNI_EVERSION || returnVal == JNI_EDETACHED ){
		jniEnv = NULL ; 
	}
	return jniEnv;
}

bool
JavaProfiler::getFrameInfo(jvmtiEnv *jvmti ,const jmethodID &methodId ,onTheFlyCallFrame &flyCallFrame ){
	jint error;
	//get method name
	JvmtiPTR< char > methodName(jvmti);
	error = jvmti->GetMethodName( methodId , methodName.returnReference(), NULL, NULL);
	
	 
	//TODO check error
	//get class name
	jclass className;
	error = jvmti->GetMethodDeclaringClass( methodId , &className );
	//std::cout<<"ERROR [ "<<error<<" ] "<<std::endl;
	//get class signature
	JvmtiPTR<char> signaturePtr( jvmti );
	error = jvmti->GetClassSignature( className , signaturePtr.returnReference() , NULL ); //TODO check error
	
	// get file name
	JvmtiPTR<char> fileName( jvmti );
	error = jvmti->GetSourceFileName (className ,fileName.returnReference());
	flyCallFrame.methodId = (int64_t) methodId;
	flyCallFrame.fileName = std::string(fileName.returnRef());
	flyCallFrame.className = std::string(signaturePtr.returnRef());
	flyCallFrame.methodName = std::string(methodName.returnRef());
#ifdef DEBUG
	std::cout<<"MethodId: [ "<<(int64_t)methodId<<" ] => FileName [ "<<fileName.returnRef()<<" ] => ClassName [ "<<signaturePtr.returnRef()<<" ] ==> MethodName [ "<<methodName.returnRef()<<" ] "<<std::endl;
#endif
	return true;
}
