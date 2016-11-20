/*
 * =====================================================================================
 *
 *       Filename:  stacktrace.hpp
 *
 *    Description:  stack trace information
 *
 *        Version:  1.0
 *        Created:  Saturday 15 August 2015 03:48:00  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ROY (), email.suman.roy@gmail.com
 *   Organization:  NA
 *
 * =====================================================================================
 */
#ifndef STACKTRACE_HPP
#define STACKTRACE_HPP
#include "global.h"

 typedef struct {
	 jint lineno;           // line number in the source file
	 jmethodID methodId;    // method executed in this frame
 } ASGCT_CallFrame;

// call trace copied from old .h file and renamed
typedef struct {
	JNIEnv *envId;          // Env where trace was recorded
	jint numFrames;         // number of frames in this trace
	ASGCT_CallFrame *frames; // frames
} ASGCT_CallTrace;

typedef void (*asynGCTType) (ASGCT_CallTrace*, jint, void*); 
 
//wraper to hold the reference of AsyncGetCallTrace method
class AsynGCT {
    public:
		static void setAsynGCT(asynGCTType callType) {
			asyngct = callType;
		}
		static asynGCTType getAsynGCT(void) {
			return asyngct;
		}

	private:
		static asynGCTType asyngct;
        DISALLOW_IMPLICIT_CONSTRUCTORS(AsynGCT);
};
#endif
