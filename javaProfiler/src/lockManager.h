/*
 * =====================================================================================
 *
 *       Filename:  lockManager.h
 *
 *       Description:  Provide the fucntionality for managing mutex lock.
 *
 *       Version:  1.0
 *       Created:  Sunday 20 November 2016 11:14:31  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Author:  SIGCONT (suman roy), email.suman.roy@gmail.com
 *       Organization:  OPEN SOURCE
 *       LICENSE: Apache License
 *  
 * =====================================================================================
 */
#ifndef LOCK_MANAGER_HPP
#define LOCK_MANAGER_HPP
#include <iostream>
#include<stdlib.h>
#include<mutex>

class Mutex {
	public:
		Mutex(void);
		virtual ~Mutex(void) {}
		void lock(void);
		void unLock(void);
	private:
		pthread_mutex_t    myMutex;
};

class MutexLock {
	public:
		MutexLock(Mutex* mut_, const bool lockOrNot = false);
    public:
        virtual ~MutexLock(void);
		void lock(void);
		void unLock(void);
	private:
		bool    mutLock;
		Mutex   *mut;
};
#endif
