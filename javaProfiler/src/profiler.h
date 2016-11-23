/*
 * =====================================================================================
 *
 *       Filename:  profiler.hpp
 *
 *    Description:  The maib handler ofprofiler.
 *
 *        Version:  1.0
 *        Created:  Friday 14 August 2015 09:36:40  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ROY (), email.suman.roy@gmail.com
 *   Organization:  NA
 *
 * =====================================================================================
 */
#ifndef PROFILER_HPP
#define  PROFILER_HPP 
#include <signal.h>
#include "global.h"
#include "stacktrace.h"
#include "signaHandler.h"
#include "processData.h"
#include "circulerQueue.h"

//core class of profiler
class JavaProfiler: protected ProcessData {
	public:
		explicit JavaProfiler(jvmtiEnv* jvmtiEnv, JavaVM* javaVM):jvmti(jvmtiEnv), jvm(javaVM), sigHandler(1) {
            circulerQueue = new CQueue(jvmtiEnv, &JavaProfiler::getFrameInfo);
        }

        virtual ~JavaProfiler(void) {
            delete circulerQueue;
        }

        void readCallTrace(const int sigNum, siginfo_t* sigInfo, void* context);

    protected:
        bool startProfiler(void);
        void stopProfiler(void);

        private:
        jvmtiEnv*  jvmti;
        JavaVM*    jvm;
        Signal sigHandler;
        void startCallBack(void);
        void worker(void);
        JNIEnv* getJNIEnv (void);
        CQueue* circulerQueue;

        const int sleepFor(const unsigned int&);
};
#endif
