/* 
* classAD7843.h
*
* Created: 06/02/2015 21:05:22
* Author: Vaio
*/


#ifndef __CLASSAD7843_H__
#define __CLASSAD7843_H__

#include <avr/io.h>
#include <avr/portpins.h>
#include <math.h>
#include "UTFT.h"
#define F_CPU (32000000UL)
#include <util/delay.h>


#define TP_CS            PIN4_bm           //PORTD
#define TP_PEN           PIN3_bm           //PORTD
#define TP_SDI           PIN6_bm           //PORTD
#define TP_SDO           PIN5_bm           //PORTD
#define TP_CLK           PIN7_bm           //PORTD

#define TP_CS_LOW()      PORTD.OUTCLR = TP_CS;
#define TP_CS_HIGH()     PORTD.OUTSET = TP_CS;

#define TP_SDI_LOW()     PORTD.OUTCLR = TP_SDI;
#define TP_SDI_HIGH()    PORTD.OUTSET = TP_SDI;

#define TP_CLK_LOW()     PORTD.OUTCLR = TP_CLK;
#define TP_CLK_HIGH()    PORTD.OUTSET = TP_CLK;

#define TOUCH_PRESSED()  !(PORTD.IN & TP_PEN);

#define getTP_SDO()	((PORTD.IN & TP_SDO)>0) ? 1 : 0

#define TC_CR_Y			 0x92 //DFR = 0, PD = 10
#define TC_CR_X			 0xD2 //DFR = 0, PD = 10

#define AD7843_MAX_YVAL	32760
#define AD7843_MAX_XVAL	30752
#define PIXELS_X        240
#define PIXELS_Y        320

extern UTFT utft;

class classAD7843
{
//variables
public:
 uint32_t keyValue;
protected:
private:
//functions
public:
	classAD7843();
	void TCInit(void);
	unsigned int TCGetX(void); //call this after TCRead() to get X value
	unsigned int TCGetY(void); //call this after TCRead() to get Y value
	unsigned char TCIsPenOn(void); //TC_PEN_PIN is 0 when the screen is pressed
	int16_t ad7843_read(uint8_t);
	bool day_key(void);
	bool hours_key(void);
	bool minute_key(void);
	void drawKeys(void);
    uint8_t presetKey(void);
	~classAD7843();
protected:
private:
	classAD7843( const classAD7843 &c );
	classAD7843& operator=( const classAD7843 &c );
	void drawButton(const char * string,uint16_t x, uint16_t y,uint8_t ancho, uint8_t alto);
	uint8_t coll1_decode(void);
	uint8_t coll2_decode(void);
	uint8_t coll3_decode(void);

}; //classAD7843

#endif //__CLASSAD7843_H__
