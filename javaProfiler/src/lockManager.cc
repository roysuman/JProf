/*
 * =====================================================================================
 *
 *       Filename:  lockManager.cc
 *
 *       Description:  Manage the lock.
 *
 *       Version:  1.0
 *       Created:  Sunday 20 November 2016 11:20:16  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Author:  SIGCONT (suman roy), email.suman.roy@gmail.com
 *       Organization:  OPEN SOURCE
 *       LICENSE: Apache License
 *
 * =====================================================================================
 */

#include "lockManager.h"

Mutex::Mutex(void) {
	pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&myMutex, &attr);
	pthread_mutexattr_destroy(&attr);
}


void Mutex::lock(void) {
	pthread_mutex_lock(&myMutex);
}


void Mutex::unLock(void) {
	pthread_mutex_unlock(&myMutex);
}


MutexLock::MutexLock(Mutex *mut_, const bool lockOrNot) {
	mut = mut_;
	mutLock = lockOrNot;
	if (mutLock) {
        mut->lock();
    }
}


MutexLock::~MutexLock(void) {
	if (mutLock) {
        mut->unLock();
    }
	mutLock = false;
}


void MutexLock::lock(void) {
	if (!mutLock) {
		mut->lock();
		mutLock = true;
	}
}


void MutexLock::unLock(void) {
	if (mutLock) {
		mutLock = false;
		mut->unLock();
	}
}
