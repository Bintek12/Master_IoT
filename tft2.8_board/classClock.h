/* 
* classClock.h
*
* Created: 04/02/2015 11:36:07
* Author: Vaio
*/


#ifndef __CLASSCLOCK_H__
#define __CLASSCLOCK_H__
#include <avr/io.h>

class classClock
{
//variables
public:
protected:
private:

//functions
public:
	classClock();
	void system_clocks_init(void);
	~classClock();
protected:
private:
	classClock( const classClock &c );
	classClock& operator=( const classClock &c );

}; //classClock
//extern public classClock clock;

#endif //__CLASSCLOCK_H__
