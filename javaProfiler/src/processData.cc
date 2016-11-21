/*
 * =====================================================================================
 *
 *       Filename:  processData.cc
 *
 *       Description:  Process the captured stack trace/frame
 *
 *       Version:  1.0
 *       Created:  Monday 21 November 2016 10:46:50  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Author:  SIGCONT (suman roy), email.suman.roy@gmail.com
 *       Organization:  OPEN SOURCE
 *       LICENSE: Apache License
 *
 * =====================================================================================
 */

#include "processData.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getFrameInfo
 *  Description: Get FrameInfo.
 *       @Param: jvmti
 *                 jvmtiEnv pointer.
 *       @Param: methodId
 *                 jmethodID reference.
 *       @Param: flyCallFrame
 *                 Container to hold faced data.
 *      @Return: return true on success, false on failure.
 * =====================================================================================
 */
bool ProcessData::getFrameInfo(jvmtiEnv* jvmti, const jmethodID& methodId,
        onTheFlyCallFrame& flyCallFrame) {
	jint error;
	jclass className;

	JvmtiPTR<char> methodName(jvmti);
    // Ref: https://docs.oracle.com/javase/7/docs/platform/jvmti/jvmti.html#GetMethodName
	error = jvmti->GetMethodName(methodId, methodName.returnReference(), NULL, NULL);
	error = jvmti->GetMethodDeclaringClass(methodId, &className);
    if (error != 0) {
        return false;
    }

	JvmtiPTR<char> signaturePtr(jvmti);
	// Ref: https://docs.oracle.com/javase/7/docs/platform/jvmti/jvmti.html#GetClassSignature
	error = jvmti->GetClassSignature(className, signaturePtr.returnReference(), NULL);
    if (error != 0) {
        return false;
    }

	JvmtiPTR<char> fileName(jvmti);
    // Ref: https://docs.oracle.com/javase/7/docs/platform/jvmti/jvmti.html#GetSourceFileName
	error = jvmti->GetSourceFileName(className, fileName.returnReference());
    if (error != 0) {
        return false;
    }

	flyCallFrame.methodId = (int64_t)methodId;
	flyCallFrame.fileName = std::string(fileName.returnRef());
	flyCallFrame.className = std::string(signaturePtr.returnRef() + 1);
	flyCallFrame.methodName = std::string(methodName.returnRef());
#ifdef DEBUG
    std::cout<<"Poped data from queue\n"
        <<"MethodId   [ "<<(int64_t)methodId<<"        ]\n"
        <<"FileName   [ "<<fileName.returnRef()<<"     ]\n"
        <<"ClassName  [ "<<signaturePtr.returnRef()<<" ]\n"
        <<"MethodName [ "<<methodName.returnRef()<<"   ] "<<std::endl;
#endif
	return true;
}

