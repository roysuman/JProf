/*
 * =====================================================================================
 *
 *       Filename:  signaHandler.cpp
 *
 *    Description:  signal handler
 *
 *        Version:  1.0
 *        Created:  Saturday 15 August 2015 08:10:50  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ROY (), email.suman.roy@gmail.com
 *   Organization:  NA
 *
 * =====================================================================================
 */

#include "signaHandler.h"

void
Signal::setAction( void ( *sigAction ) ( int , siginfo_t * , void * ) ){
	struct sigaction act;
	act.sa_handler = NULL;
	act.sa_sigaction  = sigAction;
	act.sa_flags =  SA_RESTART | SA_SIGINFO;
//	memset (&act, '\0', sizeof(act));
	sigemptyset(&act.sa_mask);
	if (sigaction(SIGPROF, &act, NULL) < 0) {
		std::cerr<<" sigaction error with Error no "<<errno<<std::endl;
	}
}

bool
Signal::updateIntervalTime( void ){
	struct itimerval times;
	times.it_interval.tv_sec = 0;
	times.it_interval.tv_usec = timerInterval;
//	times.it_interval  = times.it_value;
	times.it_value = times.it_interval;
	if (  setitimer (ITIMER_PROF, &times, NULL) == -1 ){
		std::cerr<<"Error on schiduling timing interval Error"<<errno<<std::endl;
		return false;
	}
	return true;
}
