/* 
* classTimers.h
*
* Created: 04/02/2015 12:21:25
* Author: Vaio
*/


#ifndef __CLASSTIMERS_H__
#define __CLASSTIMERS_H__

// I/O Registers definitions
#include <avr/io.h>


class classTimers
{
//variables
public:
protected:
private:

//functions
public:
	classTimers();
	// Disable a Timer/Counter type 0
	void tc0_disable(TC0_t *ptc);
	// Disable a Timer/Counter type 1
	void tc1_disable(TC1_t *ptc);
	// Timer/Counter TCD0 initialization
	void tcd0_init(void);
	// Timer/Counter TCD1 initialization
	void tcd1_init(void);
	
	void tce0_init(uint16_t per);
	~classTimers();
protected:
private:
	classTimers( const classTimers &c );
	classTimers& operator=( const classTimers &c );

}; //classTimers

#endif //__CLASSTIMERS_H__
