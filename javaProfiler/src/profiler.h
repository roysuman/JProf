/*
 * =====================================================================================
 *
 *       Filename:  profiler.hpp
 *
 *    Description:  all main work of profiler
 *
 *        Version:  1.0
 *        Created:  Friday 14 August 2015 09:36:40  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ROY (), email.suman.roy@gmail.com
 *   Organization:  NA
 *
 * =====================================================================================
 */
#ifndef PROFILER_HPP
#define  PROFILER_HPP 
#include "global.h"
#include "stacktrace.h"
#include "signaHandler.h"
#include "circulerQueue.h"
#include "tcpConnection.h"
//#include "onTheFlyData.h"
#include "connection.h"
class JavaProfiler{
	public:
		explicit JavaProfiler (  jvmtiEnv *jvmtiEnv, JavaVM * javaVM ):jvmti( jvmtiEnv ), jvm(javaVM), sigHandler( 1){
		queue = new CQueue( jvmtiEnv , &JavaProfiler::getFrameInfo , &Connection::sendData );
		tcpSock = new TcpConnection ( 8080  , &Connection::registerConnectionHandler);
		tcpSock->open();

	}
	~JavaProfiler(){ 
		delete queue;
		delete tcpSock;
	}
	bool startProfiler( );
	//read from call trace //TODO assign it with timer(SIGNAL) 
	void readCallTrace(int sigNum , siginfo_t *sigInfo  , void * context );
	private:
	jvmtiEnv  *jvmti;
	JavaVM    *jvm;
	Signal sigHandler;
	void startCallBack( void ); 
	void worker( void );
	JNIEnv * getJNIEnv ( void );
	CQueue   *queue;
	TcpConnection *tcpSock;
	void sleepFor( unsigned int );
	static bool getFrameInfo( jvmtiEnv *jvmti ,  const jmethodID & , onTheFlyCallFrame&);

};
#endif
