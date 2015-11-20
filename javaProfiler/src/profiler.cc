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

/*  Creates a new jthread */
static jthread 
allocThread(JNIEnv *jniEnvironment) {
#ifdef DEBUG
	std::cout<<"##################ALLOC THREAD @@@@@@@@@@@@@@@"<<std::endl;
#endif
	jclass    thrClass;
	jmethodID cid;
	jthread   res;
	thrClass = jniEnvironment->FindClass( "java/lang/Thread");
	if ( thrClass == NULL ) {
		std::cerr<<"Cannot find Thread class\n";
	}
	cid = jniEnvironment->GetMethodID( thrClass, "<init>", "()V");
	if ( cid == NULL ) {
		std::cerr<<"Cannot find Thread constructor method\n";
	}
	res = jniEnvironment->NewObject( thrClass, cid);
	if ( res == NULL ) {
		std::cerr<<"Cannot create new Thread object";
	}else {
		jmethodID mid = jniEnvironment->GetMethodID
			(thrClass, "setName","(Ljava/lang/String;)V");
		 jniEnvironment->CallObjectMethod
			 (res, mid, jniEnvironment->NewStringUTF("Honest Profiler Daemon Thread"));                                                     }
	return res;
}

void 
JavaProfiler::sleepFor(unsigned int sleepTime){
	usleep ( sleepTime * 1 );//TODO handle return val
}

void
JavaProfiler::worker( void ){
	//wait for connection...
	//and after receiving the connection .. start monitoring JV
	//TODO some global variable set must be done by -- after analyzing the received pack from client
//        tcpSock->continueLoop(tcpSock ); 
	
	while (1){
		queue->pop( );
		sleepFor( 1);
//		sigHandler.updateIntervalTime ( );

	}
}


void
JavaProfiler::startCallBack( void ){
	jvmtiError    err;
	//method must be called by the signal handler
	jvmtiStartFunction callbackFunction = 
		[](jvmtiEnv *jvmtiEnvironment, JNIEnv *jniEnvironemnt, void *arg) {
	      JavaProfiler  *profiler = ( JavaProfiler *) arg;
	      profiler->worker();
		};
	err = jvmti->RunAgentThread(allocThread ( getJNIEnv()) , 
			callbackFunction, this, JVMTI_THREAD_NORM_PRIORITY);

	//TODO check error
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  startProfiler
 *  Description:  start the profiler
 * =====================================================================================
 */
bool
JavaProfiler::startProfiler (  ){
//	std::cout<<"Start Profiler="<<k<<std::endl;
	//set action
	sigHandler.setAction ( &sigHandler_ );

	startCallBack ( );
	sigHandler.updateIntervalTime ( );
	return true;
}

void
JavaProfiler::readCallTrace(int sigNum , siginfo_t *sigInfo  , void * context ){
	ASGCT_CallFrame    frames [ MAX_FRAME_TO_CAPTURE ];
	JNIEnv    *jniEnv;
	ASGCT_CallTrace    callTrace;
	//TODO init the frame to zero
//	memset ( frames , sizeof( ASGCT_CallFrame ) * MAX_FRAME_TO_CAPTURE );

	callTrace.frames = frames;
	jniEnv = getJNIEnv ( );
	if ( jniEnv == NULL ){
		//raised an error
	}else {
		callTrace.envId = jniEnv ;
		asynGCTType asynGCT = AsynGCT::getAsynGCT();
		(*asynGCT) ( &callTrace , MAX_FRAME_TO_CAPTURE , context );
		//write the stacktrace in a circuler queue
//		std::cout<<"Calltrace: Frame count [ "<<callTrace.numFrames<<"] "<<std::endl;
		if ( callTrace.numFrames > 0 ){
#ifdef DEBUG
			queue->push( callTrace ) == true ? std::cout<<"DATA ENTERED IN QUEUE\n":std::cout<<"DATA NOT ENTERED\n";

			std::cout<<"METHOD ID [ "<<callTrace.frames[0].methodId<<" ] "<<std::endl;
#else
			queue->push( callTrace );
#endif
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
JavaProfiler::getFrameInfo( jvmtiEnv *jvmti , const jmethodID &methodId , onTheFlyCallFrame &flyCallFrame ){
	jint error;
	//get method name
	JvmtiPTR< char > methodName( jvmti );
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
	error = jvmti->GetSourceFileName ( className , fileName.returnReference() );
	flyCallFrame.methodId = ( int64_t ) methodId;
	flyCallFrame.fileName = std::string( fileName.returnRef());
	flyCallFrame.className = std::string ( signaturePtr.returnRef());
	flyCallFrame.methodName = std::string ( methodName.returnRef());
#ifdef DEBUG
	std::cout<<"MethodId: [ "<<(int64_t)methodId<<" ] => FileName [ "<<fileName.returnRef()<<" ] => ClassName [ "<<signaturePtr.returnRef()<<" ] ==> MethodName [ "<<methodName.returnRef()<<" ] "<<std::endl;
#endif
	return true;
}
