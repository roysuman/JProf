/*
 * =====================================================================================
 *
 *       Filename:  processData.h
 *
 *       Description:  Analysis the captured stack trace frame
 *
 *       Version:  1.0
 *       Created:  Monday 21 November 2016 10:26:50  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Author:  SIGCONT (suman roy), email.suman.roy@gmail.com
 *       Organization:  OPEN SOURCE
 *       LICENSE: Apache License
 *
 * =====================================================================================
 */
#ifndef PROCESS_DATA_H
#define PROCESS_DATA_H

#include "onTheFlyData.h"
#include "global.h"

class ProcessData {
    public:
        static bool getFrameInfo(jvmtiEnv *jvmti, const jmethodID&, onTheFlyCallFrame&);
};
#endif
