/*
 * =====================================================================================
 *
 *       Filename:  lock.cc
 *
 *    Description:  implementation of mutex methods
 *
 *        Version:  1.0
 *        Created:  Friday 21 August 2015 12:55:09  IST
 *       Revision:  none
 *       Compiler:  g++4.7
 *
 *         Author:  Suman Roy (), suman.roy@cognizant.com
 *   Organization:  Cognizant Technological Solutions
 *
 * =====================================================================================
 */
#include "lock.h"
Mutex::Mutex(){
	pthread_mutexattr_t attr;                                             pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&myMutex, &attr);
	pthread_mutexattr_destroy(&attr);
}

Mutex::~Mutex(){}

void 
Mutex::lock(){
	pthread_mutex_lock( &myMutex);
}

void
Mutex::unLock(){
	pthread_mutex_unlock( &myMutex);
}
MutexLock::MutexLock( Mutex *myMut , bool &lockOrNot ){
	mut = myMut;
	mutLock = lockOrNot;
	if ( mutLock )
		mut->lock();
}
MutexLock::~MutexLock(){
	if ( mutLock )
		mut->unLock();
	mutLock = false;
}

void
MutexLock::lock(){
	if ( !mutLock ){
		mut->lock();
		mutLock = true;
	}
}
void
MutexLock::unLock( void ){
	if ( mutLock ){
		mutLock = false;
		mut->unLock();
	}
}
