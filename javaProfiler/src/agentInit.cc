/*
 * =====================================================================================
 *
 *       Filename:  agentInit.cpp
 *
 *    Description:  init the agent enverionment for JVMTI
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
JavaProfiler  *JPROFILER;
void 
loadMethodIds(jvmtiEnv *jvmti , jclass javaClass ){
	JvmtiPTR < jmethodID > methodsOfClass(jvmti );
	jvmtiError    error;
	jint          totalMethod;
	JvmtiPTR<jmethodID> allMethods( jvmti );
	error = jvmti->GetClassMethods( javaClass , &totalMethod ,allMethods.returnReference () );
	if ( error  != JVMTI_ERROR_NONE && error != 22 ){
		JvmtiPTR< char > classSig(jvmti );
		isError ( jvmti , jvmti->GetClassSignature( javaClass , 
					classSig.returnReference() , NULL ) , 
				"Error on loding class signature" );
	}
}

void JNICALL 
callBackVMInit( jvmtiEnv *jvmti , JNIEnv *jniEnvironment , jthread thread ){
	jint totalClass;
	jclass *classListRef;
	
	JvmtiPTR<jclass> allClass(jvmti);
	if ( isError( jvmti , jvmti->GetLoadedClasses(&totalClass ,
					allClass.returnReference() ) , 
				"Error on Loading Class " ) ){
	}
	classListRef = allClass.returnRef();
	for ( ssize_t loop = 0 ; loop < totalClass ; ++ loop ){
//		jclass    currentClass = classListRef [ i ];
		loadMethodIds( jvmti , classListRef [ loop ] );
	}
	//start profilling
	JPROFILER->startProfiler( );


}
void JNICALL 
callBackClassPepare(jvmtiEnv *jvmti, JNIEnv *jni,
	   jthread thread, jclass javaClass) {                               
	loadMethodIds(jvmti, javaClass);                       
}

void JNICALL 
callBackVMDeath(jvmtiEnv *jvmti, JNIEnv *jni) {
	//stop the vm
}
void JNICALL
callBackClassLoad( jvmtiEnv *jvmti , JNIEnv *jni , jthread thread ,
		jclass javaCLass ){

}

static bool setCapabilities( jvmtiEnv *jvmti ){
	//several capabilities for the JVMTI functions and events that we will be using
	jvmtiCapabilities    capa;
//	jvmtiError           error;
//	bool                 returnVal;
	memset( &capa , 0 , sizeof( jvmtiCapabilities ) );
/* 	capa.can_signal_thread = 1;
	capa.can_get_owned_monitor_info = 1;
	capa.can_generate_method_entry_events = 1;
	capa.can_generate_exception_events = 1;
	capa.can_generate_vm_object_alloc_events = 1;
	capa.can_tag_objects = 1;
	capa.can_get_constant_pool = 1;
	capa.can_generate_all_class_hook_events = 1;*/
	capa.can_get_source_file_name = 1;
	capa.can_get_line_numbers = 1;
	capa.can_get_bytecodes = 1;
	capa.can_get_constant_pool = 1;
	//TODO a logic to check all capabilities are available or not.
	//Set new capabilities by adding the capabilities pointed to by capa
	return isError ( jvmti ,  jvmti->AddCapabilities(&capa) ,
			"Error on Set capabilities");
}

static bool 
initCallbackMethods( jvmtiEnv *jvmti ){
	jvmtiEventCallbacks    *callBacks;
	jvmtiError           error;
	bool                 returnVal;

	callBacks = new jvmtiEventCallbacks ();

	memset ( callBacks , 0 , sizeof ( jvmtiEventCallbacks ) );
	callBacks->VMInit = &callBackVMInit;
	callBacks->VMDeath = &callBackVMDeath;
	callBacks->ClassLoad = &callBackClassLoad;
	callBacks->ClassPrepare = &callBackClassPepare;

	if ( isError ( jvmti ,jvmti->SetEventCallbacks( callBacks , 
					sizeof ( jvmtiEventCallbacks ) ),
				"Cannot Set JVMTI Callback" )){

		jvmtiEvent eventsList [] = { JVMTI_EVENT_VM_INIT , JVMTI_EVENT_VM_DEATH , 
			JVMTI_EVENT_CLASS_LOAD , JVMTI_EVENT_CLASS_PREPARE};
		ssize_t totalEvents = sizeof ( eventsList ) /sizeof ( jvmtiEvent );
		ssize_t eventLoop = 0;
		error = JVMTI_ERROR_NONE;
		for ( eventLoop = 0 ; eventLoop< totalEvents && error == JVMTI_ERROR_NONE ; ++ eventLoop ){
			error = jvmti->SetEventNotificationMode( JVMTI_ENABLE ,
				       	eventsList [ eventLoop  ], NULL );
		}
		returnVal =  
			isError( jvmti , error , "Set event notifications -- Failed" );
	}else returnVal = false;
	return returnVal;
}


/*	* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Agent_onLoad
 *  Description:  this is 
 * =====================================================================================
 */
JNIEXPORT jint JNICALL 
Agent_OnLoad(JavaVM *jvm, char *options, void *reserved) {
	static jvmtiEnv *jvmti = NULL;
	//parse the options
//TODO	parseAgentOptions( options );

//	jvmtiError    error;
	jint          res;

	//get the JVMTI environment
	res = jvm->GetEnv(reinterpret_cast< void **>(&jvmti) , JVMTI_VERSION_1 );
       if ( res != JNI_OK ){
	       std::cerr<<"Error: unable to get the vertion[ "<<JVMTI_VERSION_1
		       <<"] : JDK must be >= 5.0 ..GetEnv returned ["
		       <<res<<" ]"<<std::endl;
	       return JNI_OK;

       }
       if ( !setCapabilities( jvmti ) ){
	       std::cerr<<" Unable to set capabilities"<<std::endl;
	       return JNI_OK;
       }
       if ( !initCallbackMethods( jvmti ) ){
	       std::cout<<"Unable to init call back methodes"<<std::endl;
	       return JNI_OK;

       }
       AsynGCT::setAsynGCT( JvmMethod::GetJvmFunction<asynGCTType>("AsyncGetCallTrace") );
       JPROFILER = new JavaProfiler ( jvmti ,jvm );
       return 0;

}

void sigHandler_ ( int signum, siginfo_t *info, void *context ){
	JPROFILER->readCallTrace( signum , info , context );


}
/*
int main(){
	return 0;
}
*/
