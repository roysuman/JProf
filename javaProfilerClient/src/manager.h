/*
 * =====================================================================================
 *
 *       Filename:  manager.h
 *
 *    Description:  manageing the client part
 *
 *        Version:  1.0
 *        Created:  Monday 24 August 2015 11:07:59  IST
 *       Revision:  none
 *       Compiler:  g++4.7
 *
 *         Author:  Suman Roy (), suman.roy@cognizant.com
 *   Organization:  Cognizant Technological Solutions
 *
 * =====================================================================================
 */
#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include<stdlib.h>
#include<cstring>


#include "tcpConnection.h"
#include "packet.h"
#include "opcode.h"

class ClientManager{
	public:
		explicit ClientManager( ){
			client = new TcpClient( 8080 , "127.0.0.1" );
			pack   = new Packet ();
		}
		~ClientManager(){
			delete client;
			delete pack;
		}
		bool initClient( void );
		bool disconnectClient( void );

		bool sendCallTraceCaptureRequest( void );
		bool stopCallTraceCaptureRequest( void );
		pthread_t thread;
	private:
		bool isClientInitFlag;
		TcpClient    *client;
		Packet       *pack;
};

#endif
