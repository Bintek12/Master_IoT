/* 
* classMnu.h
*
* Created: 06/04/2015 16:07:30
* Author: Vaio
*/


#ifndef __CLASSMNU_H__
#define __CLASSMNU_H__

#include <avr/io.h>
#include <avr/portpins.h>
#include <stdbool.h>
#include <math.h>
#define F_CPU (32000000UL)
#include <util/delay.h>
#include "classSPI.h"
#include "UTFT.h"
#include "classAD7843.h"
#include "classI2C.h"
#include "tipos.h"

extern UTFT utft;
extern classAD7843 AD7843;
extern classI2C m41t00;
extern classSPI SPIc;

class classMnu
{
//variables
public:
protected:
private:
	uint8_t decenas_s;
	uint8_t unidades_s;
	uint8_t decenas_m;
	uint8_t unidades_m;
	uint8_t decenas_h;
	uint8_t unidades_h;
	
	uint8_t dec_miles;
	uint8_t miles;
	uint8_t centenas;
	uint8_t decenas;
	uint8_t unidades;

//functions
public:
	classMnu();
	~classMnu();
	void time_date_read(void);
	void Display_time(void);
	void Display_temp(int16_t temp);
protected:
private:
	classMnu( const classMnu &c );
	classMnu& operator=( const classMnu &c );

}; //classMnu

#endif //__CLASSMNU_H__
