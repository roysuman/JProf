/*
 * =====================================================================================
 *
 *       Filename:  lock.h
 *
 *    Description:  Mutex lock
 *
 *        Version:  1.0
 *        Created:  Friday 21 August 2015 12:34:48  IST
 *       Revision:  none
 *       Compiler:  g++4.7
 *
 *         Author:  Suman Roy (), suman.roy@cognizant.com
 *   Organization:  Cognizant Technological Solutions
 *
 * =====================================================================================
 */
#ifndef LOCK_H
#define LOCK_H 
#include <iostream>
#include<stdlib.h>
#include<mutex>

class Mutex{
	public:
		Mutex();
		~Mutex();
		void lock();
		void unLock();
//		void tryLock();
	private:
		pthread_mutex_t    myMutex;
};

class MutexLock{
	public:
		MutexLock( Mutex * , bool& );
		~MutexLock();
		void lock();
		void unLock();
	private:
		bool    mutLock;
		Mutex   *mut;
};
#endif
