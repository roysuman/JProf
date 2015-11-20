/*
 * =====================================================================================
 *
 *       Filename:  opcode.h
 *
 *    Description:  all opcode
 *
 *        Version:  1.0
 *        Created:  Monday 24 August 2015 12:03:41  IST
 *       Revision:  none
 *       Compiler:  g++4.7
 *
 *         Author:  Suman Roy (), suman.roy@cognizant.com
 *   Organization:  Cognizant Technological Solutions
 *
 * =====================================================================================
 */
#ifndef OPCODE_H
#define OPCODE_H
#define CALLTRACE_START 1
#define CALLTRACE_STOP  2
#define CALLTRACE_ERROR 3
#define CALLTRACE_SERVER_RESPONSE_OK 4
#define CALLTRACE_CLIENT_RESPONSE_OK 5
#define CALLTRACE_DATA               6


#define JFLG_CALL_TRACE           0x01   
#define JFLAG_FUTURE_USE1         0x02   
#define JFLAG_FUTURE_USE2         0x04   
#define JFLAG_FUTURE_USE3         0x10 

#endif
