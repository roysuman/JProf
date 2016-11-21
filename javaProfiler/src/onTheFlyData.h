/*
 * =====================================================================================
 *
 *       Filename:  onTheFlyData.h
 *
 *       Description: The main idea of this class is to maintain on thefly data.
 *                    The data that will be sent to profiler server.
 *
 *       Version:  1.0
 *       Created:  Monday 14 November 2016 11:06:58  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Author:  SIGCONT (suman roy), email.suman.roy@gmail.com
 *
 * =====================================================================================
 */
#ifndef ON_THE_FLY_DATA_H
#define ON_THE_FLY_DATA_H

#define MAX_FRAME_TO_CAPTURE 256

#include <unistd.h>
#include <sys/types.h>
#include <iostream>

typedef struct onTheFlyCallFrame_ onTheFlyCallFrame;
struct onTheFlyCallFrame_ {
	size_t     frameNo;
	int64_t    methodId;
	std::string fileName;
	std::string className;
	std::string methodName;
};

typedef struct {
	int64_t threadId;                   // Env where trace was recorded
	size_t numFrames;                  // number of frames in this trace
	onTheFlyCallFrame frames[MAX_FRAME_TO_CAPTURE];          // frames
} callTrace;
#endif
