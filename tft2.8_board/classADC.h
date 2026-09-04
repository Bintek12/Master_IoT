/* 
* classADC.h
*
* Created: 11/03/2015 15:19:27
* Author: Vaio
*/


#ifndef __CLASSADC_H__
#define __CLASSADC_H__

// I/O Registers definitions
#include <avr/io.h>
#include <avr/pgmspace.h>
#define F_CPU (32000000UL)
#include <util/delay.h>
#define ADCACAL0_offset 0x20
#define ADCACAL1_offset 0x21
#define ADCBCAL0_offset 0x24
#define ADCBCAL1_offset 0x25
#define TEMPSENSE0_offset      0x2E
#define TEMPSENSE1_offset      0x2F



class classADC
{
//variables
public:
protected:
private:
unsigned int TempRefCall_85L;
unsigned int TempRefCall_85H;

//functions
public:
	classADC();
	~classADC();
	void adca_init(void);
	unsigned int adca_read(unsigned char channel);
protected:
private:
	classADC( const classADC &c );
	classADC& operator=( const classADC &c );
    unsigned char SP_ReadCalibrationByte( unsigned char index );

}; //classADC

#endif //__CLASSADC_H__
