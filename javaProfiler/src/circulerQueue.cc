/*
 * =====================================================================================
 *
 *       Filename:  circulerQueue.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  Monday 17 August 2015 09:42:45  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ROY (), email.suman.roy@gmail.com
 *   Organization:  NA
 *
 * =====================================================================================
 */
#include "circulerQueue.h"
bool
CQueue::push ( ASGCT_CallTrace &callTraceItem ){
#ifdef DEBUG
	std::cout<<"MethodId1 [ "<<callTraceItem.frames[0].methodId<<" ] "<<std::endl;
#endif
	bool    returnVal;
	pthread_mutex_lock( &lock);

	//check whether queue is full or not
	if ( ( readHead == 0 && writeHead == QUEUE_CAPACITY - 1) || writeHead + 1 == readHead ){
		std::cout<<"Circuler Queue Full"<<std::endl;
		returnVal = false; //TODO some mechanism to avoid data loss
	}else{
		//insert data
		if ( writeHead == QUEUE_CAPACITY - 1 ){
			writeHead = 0;
		}else writeHead ++;
		callTraceBuffer[writeHead].envId = callTraceItem.envId;
		callTraceBuffer[writeHead].numFrames = callTraceItem.numFrames;
		callTraceBuffer[ writeHead ].frames  = callTraceItem.frames;
		for ( jint loop = 0 ; loop < callTraceItem.numFrames ; ++ loop ){
			callFrameBuffer[ writeHead ] [ loop ].lineno = callTraceItem.frames[loop].lineno;
			callFrameBuffer[ writeHead][ loop ].methodId = callTraceItem.frames[ loop ].methodId;
#ifdef DEBUG
			std::cout<<"MethodId X [ "<<callTraceBuffer[ writeHead].frames[ loop ].methodId<<" MethodId Y [ "<<callTraceItem.frames[ loop ].methodId<<" ] "<<std::endl;
		//	frameInfo(jvmti , callFrameBuffer[ writeHead][loop]);
#endif
		}
		if ( readHead == -1 ) readHead = 0;
		returnVal = true;
	}
	pthread_mutex_unlock(&lock);
	return returnVal;
}
bool
CQueue::pop( void ){  // TODO future flag value must be come from caller method
//	std::cout<<"READ HEAD [ "<<readHead<<" ] \n";
	bool    returnVal , callFrameRet;
	size_t successFrameData;
	unsigned int    flag= 0x00;
	size_t     bufferHead = 0 ;//sizeof( flag );
	size_t     bufferHeadToInsertTheLength;
	size_t     length;
	onTheFlyCallFrame callFrame;
	pthread_mutex_lock(&lock);
	if ( readHead == -1 ) returnVal = false;
	else{
		//TODO read the data from the queue and create an instance of packet
		//then make the on the fly data then -- push the data into the sendBuffer -- connection.h
		//TODO there must be a SWITCH case to differentiate between a calltrace and heap dump data.
		successFrameData = 0;
		std::cout<<"Thread Id [ "<<( int64_t) callTraceBuffer[ readHead].envId<<" ] \n";
		std::cout<<" No Of Frame [ "<<callTraceBuffer[readHead].numFrames<<" ] "<<std::endl;

		
		
		//now analyze the buffer data and send to the clienti
		for ( jint loop = 0 ; loop < callTraceBuffer[readHead].numFrames ; ++loop ){
			
#ifdef DEBUG
			std::cout<<"MethodId2 [ "<<callFrameBuffer[readHead][loop].methodId<<"] "<<std::endl;
#endif
			callFrameRet = frameInfo(jvmti , callFrameBuffer[ readHead][loop].methodId , callFrame );
			//clear all data from the current frame
			memset(&callFrameBuffer[ readHead][loop],0, sizeof(ASGCT_CallFrame));
			if ( callFrameRet ){
				callFrame.frameNo = successFrameData++;

				std::cout<<"\nMethodId [ "<<callFrame.methodId
					<<" ] FileName [ "<<callFrame.fileName
					<<" ] Classname [ "<<callFrame.className
					<<" ] Method Name [ "<<callFrame.methodName
					<<" ] FrameNo [ "<<callFrame.frameNo<<"]"<< std::endl;
					size_t a = 0;
				if ( ! ( flag && JFLG_ONTHE_FLY_CALL_TRACE )){
					flag |= JFLG_ONTHE_FLY_CALL_TRACE;
					
					bufferHead = EncDecode<unsigned int>::encode ( flag , buffer , bufferHead );

//					std::cout<< EncDecode< unsigned int >::decode ( buffer , a )<<std::endl;
//					std::cout<<"Buffer head [ [ "<< bufferHead<<" ] "<<std::endl;
					bufferHeadToInsertTheLength= bufferHead;
					bufferHead += 3;
					a = bufferHead;
					bufferHead = EncDecode< int64_t > ::encode ( (int64_t)callTraceBuffer [ readHead ].envId , buffer , bufferHead ); //TODO a map for thred id
					std::cout<< EncDecode< int64_t >::decode ( buffer , a )<<std::endl;
				}
				
					a = bufferHead;

				bufferHead = EncDecode<int64_t>::encode( callFrame.methodId , buffer , bufferHead );
				std::cout<< "Method Id "<<EncDecode< int64_t >::decode ( buffer , a )<<std::endl;

				length = ( size_t) callFrame.fileName.length();				
				EncDecode<size_t>::nth24( buffer , bufferHead , length );
				memcpy( ( void*) ( buffer  + bufferHead ) , (void*)callFrame.fileName.c_str() , length );
				bufferHead += length;

				length = (size_t) callFrame.className.length();
				EncDecode<size_t>::nth24( buffer , bufferHead , length );
				memcpy( ( void*)( buffer + bufferHead ) , (void*) callFrame.className.c_str() , length);
				bufferHead += length;

				length = ( size_t ) callFrame.methodName.length();
				EncDecode<size_t>::nth24( buffer , bufferHead , length );
				memcpy( (void*)(buffer + bufferHead),(void*) callFrame.methodName.c_str(), length );
				bufferHead += length;

				bufferHead = EncDecode<size_t>::encode  ( callFrame.frameNo , buffer , bufferHead);
				if ( bufferHead > BUFFER_SIZE ) {
				       returnVal = false;
				       std::cerr<<"Buffer size exited\n";
				}	       
			
			}else std::cerr<<"Error on geting call frame data\n";
		}
		if ( bufferHead > 0 ){
			EncDecode<size_t>::nth24( buffer , bufferHeadToInsertTheLength , bufferHead ); // total length of the buffer { flag -> totalPayload -> threadID
			//send the data
			returnVal = sendData( buffer , bufferHead );
		}



		if ( readHead == writeHead ) readHead = writeHead = -1;
		else {
			if ( readHead == QUEUE_CAPACITY - 1 )
				readHead = 0;
			else readHead ++;
		}
		returnVal = true;
	}
	pthread_mutex_unlock( &lock );
	return returnVal ;
}

