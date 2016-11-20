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


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  setAction
 *  Description:  Change the action process should tahe on receipt of SIGPROF signal.
 *                Ref: http://man7.org/linux/man-pages/man2/rt_sigaction.2.html
 *       @param:  Function pointer, defines the signal handling function for signal
 *                SIGPROF.
 * =====================================================================================
 */
void Signal::setAction(void(*sigAction) (int, siginfo_t*, void*)) {
    struct sigaction act;
    // sa_handler specifies the action to be associated with SIGPROF.
    act.sa_handler = NULL;
    // sa_sigaction specifies the signal handling function for SIGPROF.
	act.sa_sigaction  = sigAction;
    // sa_flags specifies a set of flags which modify the behavior of
    // the signal SIGPROF.
	act.sa_flags =  SA_RESTART | SA_SIGINFO;
    // TODO uncomment of belowline shows following runtime error.
    // "Profiling timer expired"
    // memset(&act, '\0', sizeof(act));
    // These functions allow the manipulation of POSIX signal sets.
	sigemptyset(&act.sa_mask);
    // system call to change the action taken by the proces on receipt
    // of a SIGPROF signal.
	if (sigaction(SIGPROF, &act, NULL) < 0) {
		std::cerr<<"sigaction error with Error no "<<errno<<std::endl;
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  updateIntervalTime
 *  Description:  Update profiler timer to collect stack trace
 *      @return:  Return true on success else false.
 * =====================================================================================
 */
bool Signal::updateIntervalTime(void) {
	struct itimerval times;
	times.it_interval.tv_sec = 0;
	times.it_interval.tv_usec = timerInterval;
    //times.it_interval  = times.it_value;
	times.it_value = times.it_interval;
    // Set timer of an intervaltimer
    // Ref: https://linux.die.net/man/2/setitimer
	if (setitimer(ITIMER_PROF, &times, NULL) == -1) {
		std::cerr<<"Schiduling timing interval for Profiler fails wuith error "
            <<errno<<std::endl;
		return false;
	}
	return true;
}
