/* 
* classPorts.h
*
* Created: 04/02/2015 12:05:16
* Author: Vaio
*/


#ifndef __CLASSPORTS_H__
#define __CLASSPORTS_H__

// I/O Registers definitions
#include <avr/io.h>

class classPorts
{
//variables
public:
protected:
private:

//functions
public:
	classPorts();
	void ports_init(void);
	~classPorts();
protected:
private:
	classPorts( const classPorts &c );
	classPorts& operator=( const classPorts &c );

}; //classPorts

#endif //__CLASSPORTS_H__
