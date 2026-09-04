/* 
* classSPI.h
*
* Created: 04/02/2015 11:24:30
* Author: Vaio
*/


#ifndef __CLASSSPI_H__
#define __CLASSSPI_H__

// I/O Registers definitions
#include <avr/io.h>


class classSPI
{
//variables
public:
    SPI_t *spi;
protected:
private:

//functions
public:
	classSPI(SPI_t *spi);
	~classSPI();
	void spi_init(void);
	unsigned char transfer(unsigned char c);

protected:
private:
	classSPI( const classSPI &c );
	classSPI& operator=( const classSPI &c );

}; //classSPI


#endif //__CLASSSPI_H__
