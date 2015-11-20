/*
 * =====================================================================================
 *
 *       Filename:  global.hpp
 *
 *    Description:  all global datastructure
 *
 *        Version:  1.0
 *        Created:  Monday 22 June 2015 11:30:32  IST
 *       Revision:  none
 *       Compiler:  g++4.7
 *
 *         Author:  Suman Roy (), suman.roy@cognizant.com
 *   Organization:  Cognizant Technological Solutions
 *
 * =====================================================================================
 */
#ifndef GLOBAL_HPP_
#define GLOBAL_HPP_
#include <jvmti.h>
#include <jvmticmlr.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <dlfcn.h>
#include <jvmti.h>
#include <jni.h>
#include <stdint.h>
#include <signal.h>
#include <cstring>
#include <type_traits> 
#include <inttypes.h> 
#include <sys/types.h> 
#include <stdint.h> 
#include <assert.h>
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
	  TypeName();                                    \
  DISALLOW_COPY_AND_ASSIGN(TypeName)



static bool
isError( jvmtiEnv *jvmti , jvmtiError errNum , const char *errorString ){
	bool    returnVal;

	returnVal = true;
	if ( errNum != JVMTI_ERROR_NONE ){
		char *error = NULL;
//		char *error2 = NULL;
	//	char *error2 = NULL;
		jvmti->GetErrorName(errNum , &error );
		/*
		error2 = error == NULL ? "Not Registered Error": error;
		std::cerr<<" JVMTI Error : "<<error2<"UDefined Error"<<errorString<<std::endl;*/
		returnVal = false;
	}
	return returnVal;
}
template < class T>
class JvmtiPTR{
	private:
		jvmtiEnv *__jvmti__;
		T *__pointerRef__;
	public:
		JvmtiPTR( jvmtiEnv *jvmti ): __jvmti__( jvmti ), __pointerRef__( NULL ){}
		JvmtiPTR( jvmtiEnv *jvmti , T *refPtr): __jvmti__( jvmti ), __pointerRef__( refPtr ){}
		~JvmtiPTR();
		T **returnReference();
		T *returnRef();
			
};
template <class T> JvmtiPTR<T>::~JvmtiPTR(){
	if ( __pointerRef__ != NULL ){
		isError ( __jvmti__ ,__jvmti__->Deallocate (( unsigned char * )__pointerRef__ ) , " Error on Deallocation memory for loaded class" );
	}
}
template < class T >
T ** JvmtiPTR<T>::returnReference(){
	return &__pointerRef__;
}
template < class T >
T * JvmtiPTR< T>::returnRef(){
	return __pointerRef__;
}
extern void sigHandler_ ( int signum, siginfo_t *info, void *context );


// Short version: reinterpret_cast produces undefined behavior in many
// cases where memcpy doesn't.

template <class Dest, class Source>
inline Dest bit_cast(const Source& source) {
	// Compile time assertion: sizeof(Dest) == sizeof(Source)
//     // A compile error here means your Dest and Source have different sizes.

	typedef char VerifySizesAreEqual[sizeof(Dest) == sizeof(Source) ? 1 : -1]
		__attribute__ ((unused));
	Dest dest;
	memcpy(&dest, &source, sizeof(dest));
	return dest;
}




// JvmMethod for getting the Jvm function for AsyncGetCallTrace.           
class JvmMethod {                                                          
	public:
		template<class FunctionType>
			static inline FunctionType GetJvmFunction(const char *function_name) { 
				 // get handle to library
				 static void *handle = dlopen("libjvm.so", RTLD_LAZY);
				 if (handle == NULL) {
					 std::cerr<<"NULL"<<std::endl;
					 return NULL;
				 }
				 // get address of function, return null if not found
				 return bit_cast<FunctionType>(dlsym(handle, function_name)); 
			}
};


#endif
