/*
 * =====================================================================================
 *
 *       Filename:  signaHandler.hpp
 *
 *    Description:  handle the signal for calling AsyncCallTrace
 *
 *        Version:  1.0
 *        Created:  Saturday 15 August 2015 04:47:37  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ROY (), email.suman.roy@gmail.com
 *   Organization:  NA
 *
 * =====================================================================================
 */
#ifndef SIGNALHANDLER_HPP
#define SIGNALHANDLER_HPP
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <iostream>
#include <string.h>
class Signal {
	public:
		Signal(const int interval):timerInterval(interval){}
		bool updateIntervalTime(void);
		void setAction(void(*sigAction)(int, siginfo_t*, void*));
	private:
		const int timerInterval;
};
#endif
