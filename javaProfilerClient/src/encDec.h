/*
 * =====================================================================================
 *
 *       Filename:  encDec.h
 *
 *    Description:  data through the network level.
 *
 *        Version:  1.0
 *        Created:  Thursday 20 August 2015 10:40:13  IST
 *       Revision:  none
 *       Compiler:  g++4.7
 *
 *         Author:  Suman Roy (), suman.roy@cognizant.com
 *   Organization:  Cognizant Technological Solutions
 *
 * =====================================================================================
 */
#ifndef ENCDEC_H
#define ENCDEC_H

#include <iostream>
#include <stdlib.h>
#include  <cstring>

#define END 0x80
#define CONTINUE 0x00
#define POWER7 0x7f
#define prnt(s,n) for( int j=0; j< n; j++ ) std::cout<< (int) s[j] << "-"

#define ntoh24(p)  ((unsigned int)*((const unsigned char *)(p)+0)<<16|  \
		(unsigned int)*((const unsigned char *)(p)+1)<<8|   \
		(unsigned int)*((const unsigned char *)(p)+2)<<0)

#define letoh24(p) ((unsigned int)*((const unsigned char *)(p)+2)<<16|  \
		(unsigned int)*((const unsigned char *)(p)+1)<<8|   \
		(unsigned int)*((const unsigned char *)(p)+0)<<0)
/*-----------------------------------------------------------------------------
 *  *01( onr bit ) flag used to identify that a series of call frame data is
 *  being send. 
 *  then int to represent the thread id.( at client we no need to knoe the
 *  thread id, we will generate unique thread id for each threads)
 *  then the entire series of callFrames 
 *  at the end a special flag to represent that data end.That is max frame count 
 
 *
 *-----------------------------------------------------------------------------*/
template < class T > 
class EncDecode{
	public:
		static T decode ( unsigned char* , size_t&  ) ;
		static size_t  encode ( T ,unsigned  char *, size_t  );
		static bool nth24 ( unsigned char * , size_t& , T& );
};

template < class T > T
EncDecode< T >::decode (  unsigned char* s , size_t& pos ) {
	T x =0 ;
	int numInts = ( sizeof( x ) - 1 ) / 4 + 1;
       int *r = ( int * )&x;
       int i = pos, mp = 0;
       for ( int j = 0 ; j<numInts ; ++j ){
	       while ( ! ( s[ i ] & END ) ) {
		       r [ j ] |= ( s [ i++ ] & ~ END ) <<mp;
		       mp += 7;
	       }
	       r [ j ] |= ( s [ i++ ] & ~END ) <<mp ;
       }
       pos = i;
       return x;
}

template < class T > size_t  
EncDecode < T > :: encode ( T x , unsigned char * s , size_t bufferPoint){
       int numInts = ( sizeof ( x ) - 1 ) / 4 + 1;       
       int si=bufferPoint , res = 0 ;
	int t ;

       int * p = ( int *) &x;
       
       for ( int i = 0 ; i < numInts ; ++i ){
	      t = p [i ];
	       if ( i == (  numInts - 1 )){//last int-bytes may not be of4bytes, so copying data.
		       t =0;
		       memcpy ( (void*)&t , ( void *)&p[i] , sizeof(x) - i * sizeof ( int ) );
	       }
		       if ( !t ) s [ si++] = ( END | t );
		       else
			       while( t > 0 ){
				       res = t & 0x7f;
				       s [ si++ ] = ( t > 0x7f  ?CONTINUE : END ) | res;
				       t>>=7;
			       }
	       }
	       std::cout<<std::endl;
	       t=bufferPoint;
	       return si;
}


template< class T> bool 
EncDecode< T> :: nth24 ( unsigned char *tvb , size_t &offset , T &data ) {
	std::cout<<"Ofset [ "<<offset<<" ]\n";
	tvb [ offset ] = data & 0xff;                             
	tvb [ ++offset ] = ( data >> 8 ) & 0xff ;
	tvb [ ++offset ] = ( data >>16 ) & 0xff ;              
	offset++;//pointing next
       /*	
	std::cout<<"Ofset [ "<<offset<<" ]\n";
	std::cout<<"Check [ " <<ntoh24( tvb  +( offset - 2 ) )<<" ] [ "<<letoh24( tvb + ( offset - 2 )) <<" ] \n";
	
exit(0);
*/
	//tvb [ ++offset ] = ( data >>32 ) & 0xff ;
	return true;                                              
                    
} 
#endif