/* 
* classDAC.h
*
* Created: 11/03/2015 15:05:04
* Author: Vaio
*/

#ifndef __CLASSDAC_H__
#define __CLASSDAC_H__

#include <avr/io.h>


class classDAC
{
//variables
public:
protected:
private:

//functions
public:
	classDAC();
	void dacb_init(void);
	void dacb0_write(unsigned int data);
	~classDAC();
protected:
private:
	classDAC( const classDAC &c );
	classDAC& operator=( const classDAC &c );

}; //classDAC

#endif //__CLASSDAC_H__
