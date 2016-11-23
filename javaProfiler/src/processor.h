/*
 * =====================================================================================
 *
 *       Filename:  processor.h
 *
 *       Description:  The HOD of profiler, who init the profiler.
 *
 *       Version:  1.0
 *       Created:  Monday 21 November 2016 11:16:48  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Author:  SIGCONT (suman roy), email.suman.roy@gmail.com
 *       Organization:  OPEN SOURCE
 *       LICENSE: Apache License
 * =====================================================================================
 */
#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "profiler.h"
#include "processData.h"

class Processor: public JavaProfiler {
    public:
        explicit Processor(jvmtiEnv* jvmtiEnv, JavaVM* javaVM): JavaProfiler(jvmtiEnv, javaVM) {}

        virtual ~Processor(void) {}

        inline void init(void) {
            (void)startProfiler();
        }

        inline void exit(void) {
            (void)stopProfiler();
        }
};

#endif
