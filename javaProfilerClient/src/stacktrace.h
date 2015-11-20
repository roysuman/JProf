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

#define MAX_FRAME_TO_CAPTURE 128
typedef struct callFrame_ callFrame;
struct onTheFlyCallFrame_{
	size_t     frameNo;
	int64_t    methodId;
	std::string fileName;
	std::string className;
	std::string methodName;
};
// call trace copied from old .h file and renamed

typedef struct {
	int64_t threadId;                   // Env where trace was recorded
	size_t numFrames;                  // number of frames in this trace
	callFrame frames[MAX_FRAME_TO_CAPTURE ];          // frames
}callTrace;
#endif
