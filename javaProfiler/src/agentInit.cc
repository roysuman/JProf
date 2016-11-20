/*
 * =====================================================================================
 *
 *       Filename:  agentInit.cpp
 *
 *    Description:  init the agent enverionment for JVMTI.
 *
 *        Version:  1.0
 *        Created:  Saturday 20 June 2015 11:40:48  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ROY (), email.suman.roy@gmail.com
 *   Organization:  NA
 *
 * =====================================================================================
 */
#include "global.h"
#include "profiler.h"
#include "stacktrace.h"


asynGCTType AsynGCT::asyngct;
static JavaProfiler  *JPROFILER;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  loadMethodIds
 *  Description:  Load all methods Ids for class.
 *       @Param:  jvmti
 *                  The environment pointer.
 *       @Param:  javaClass
 *                  The class to query.
 * =====================================================================================
 */
void loadMethodIds(jvmtiEnv *jvmti, jclass javaClass) {
	JvmtiPTR<jmethodID> methodsOfClass(jvmti);
	jvmtiError    error;
	jint          totalMethod;
	JvmtiPTR<jmethodID> allMethods(jvmti);
    //ref http://docs.oracle.com/javase/1.5.0/docs/guide/jvmti/jvmti.html#GetClassMethods
	error = jvmti->GetClassMethods(javaClass, &totalMethod, allMethods.returnReference());
	if (error != JVMTI_ERROR_NONE && error != 22){
		JvmtiPTR<char> classSig(jvmti);
		isError(jvmti, jvmti->GetClassSignature(javaClass, 
					classSig.returnReference(), NULL), 
				"Error on loding class signature");
	}
}


void JNICALL callBackVMInit(jvmtiEnv *jvmti, JNIEnv *jniEnvironment, jthread thread) {
	jint totalClass;
	jclass *classListRef;

	JvmtiPTR<jclass> allClass(jvmti);
	if (isError(jvmti, jvmti->GetLoadedClasses(&totalClass,
					allClass.returnReference()), 
				"Error on Loading Class ")) {
	}
	classListRef = allClass.returnRef();
	for (ssize_t loop = 0; loop < totalClass; ++ loop) {
		loadMethodIds(jvmti, classListRef[loop]);
	}
	//start profilling
	JPROFILER->startProfiler();
}


void JNICALL callBackClassPepare(jvmtiEnv *jvmti, JNIEnv *jni,
	   jthread thread, jclass javaClass) {                               
	loadMethodIds(jvmti, javaClass);                       
}


static void JNICALL callBackVMDeath(jvmtiEnv *jvmti, JNIEnv *jni) {
    //TODO allclearance operation.
}


void JNICALL callBackClassLoad(jvmtiEnv *jvmti, JNIEnv *jni, jthread thread,
		jclass javaCLass) {
    //TODO
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  setCapabililities
 *  Description:  Enable several capabilities for the JVMTI functions and events that
 *                profiler will be using. It is generally desired, and in some cases
 *                required, to add these capabilities in the Agent_OnLoad function
 *   @Parameter:  jvmti
 *                  The environment pointer.
 *     @Returns:  true if successfully set all capabilities else false on failure.
 * =====================================================================================
 */
static bool setCapabilities(jvmtiEnv *jvmti) {
	jvmtiCapabilities    capabilities;
	memset(&capabilities, 0, sizeof(jvmtiCapabilities));
    //TODO:add all necessary capabilities
/*
    capa.can_signal_thread = 1;
	capa.can_get_owned_monitor_info = 1;
	capa.can_generate_method_entry_events = 1;
	capa.can_generate_exception_events = 1;
	capa.can_generate_vm_object_alloc_events = 1;
	capa.can_tag_objects = 1;
	capa.can_get_constant_pool = 1;
	capa.can_generate_all_class_hook_events = 1;
*/
	capabilities.can_get_source_file_name = 1;
	capabilities.can_get_line_numbers = 1;
	capabilities.can_get_bytecodes = 1;
	capabilities.can_get_constant_pool = 1;
	//TODO: a logic to check all capabilities are available or not.
	return isError(jvmti, jvmti->AddCapabilities(&capabilities),
			"Error on Set capabilities");
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  initCallBackMethods
 *  Description:  init all call back methods.
 *   @Parameter:  jvmti
 *                  The environment pointer.
 *     @Returns:  true if successfully enabled call backmethods else false on failure.
 * =====================================================================================
 */
static bool initCallBackMethods(jvmtiEnv *jvmti) {
	jvmtiEventCallbacks  *callBacks;
	jvmtiError           error;
	bool                 returnVal;

	callBacks = new jvmtiEventCallbacks();

	memset (callBacks, 0, sizeof(jvmtiEventCallbacks));
	callBacks->VMInit = &callBackVMInit;
	callBacks->VMDeath = &callBackVMDeath;
	callBacks->ClassLoad = &callBackClassLoad;
	callBacks->ClassPrepare = &callBackClassPepare;

	if (isError(jvmti, jvmti->SetEventCallbacks(callBacks, 
					sizeof(jvmtiEventCallbacks)),
				"Cannot Set JVMTI Callback")) {
		jvmtiEvent eventsList[] = {JVMTI_EVENT_VM_INIT, JVMTI_EVENT_VM_DEATH, 
			JVMTI_EVENT_CLASS_LOAD, JVMTI_EVENT_CLASS_PREPARE};
		ssize_t totalEvents = sizeof(eventsList) / sizeof(jvmtiEvent);
		ssize_t eventLoop = 0;
		error = JVMTI_ERROR_NONE;
		for (eventLoop = 0; eventLoop < totalEvents && error == JVMTI_ERROR_NONE; ++eventLoop) {
			error = jvmti->SetEventNotificationMode(JVMTI_ENABLE,
				       	eventsList[eventLoop], NULL);
		}
		returnVal =
			isError(jvmti, error, "Set event notifications -- Failed");
	} else {
        returnVal = false;
    }
	return returnVal;
}


/** 
 * ===  FUNCTION  ======================================================================
 *         Name:  Agent_onLoad
 *  Description:  The agent must have this function, this function is invoked when
 *                librarry is getting loaded. The Agent_OnLoad function is used to set up
 *                functionality that is required prior to initializing the Java virtual
 *                machine.
 *  @Parameter:   jvm
 *                   denote a jav vm.
 *  @Parameter:   options
 *  @Parameter:   reserved
 *  @Return:      JNI_OK
 *                     allways JNI_OK getting returned to denote success.
 * =====================================================================================
 */
JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *jvm, char *options, void *reserved) {
    static jvmtiEnv *jvmti = NULL;

    //get the JVMTI environment
    const jint res = jvm->GetEnv(reinterpret_cast<void**>(&jvmti), JVMTI_VERSION_1);
    if (res != JNI_OK) {
        std::cerr<<"Error: unable to get the vertion["<<JVMTI_VERSION_1
            <<"] : JDK must be >= 5.0 ..GetEnv returned ["
            <<res<<" ]"<<std::endl;
        return JNI_OK;
    }
    if (!setCapabilities(jvmti)) {
        std::cerr<<" Unable to set capabilities"<<std::endl;
        return JNI_OK;
    }
    if (!initCallBackMethods(jvmti)) {
        std::cout<<"Unable to init call back methodes"<<std::endl;
        return JNI_OK;
    }

    AsynGCT::setAsynGCT(JvmMethod::GetJvmFunction<asynGCTType>("AsyncGetCallTrace"));
    JPROFILER = new JavaProfiler (jvmti, jvm);
    return JNI_OK;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sigHandler_
 *  Description:  The signalhandler that is hettig called on occurrance of signal
 *                SIGPROF and it calls the method which collect stacktrace and push
 *                the collected sample into circuler queue for late processing.
 *       @Param:  signum
 *                  The signalnumber for which singal handler.
 *       @Param:  info
 *                  The siginfo_t structure is passed as the second parameter to a user
 *                  signal handler function.
 *       @Param:  context
 *                  The profiler context.
 * =====================================================================================
 */
void sigHandler_(int signum, siginfo_t *info, void *context) {
	JPROFILER->readCallTrace(signum, info, context);
}
