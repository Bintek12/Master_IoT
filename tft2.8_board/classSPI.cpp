/* 
* classSPI.cpp
*
* Created: 04/02/2015 11:24:30
* Author: Vaio
*/


#include "classSPI.h"

// default constructor
classSPI::classSPI(SPI_t *spi_)
{
	spi = spi_;
} //classSPI

// SPIC initialization
void classSPI::spi_init()
{
// SPIC is enabled
// SPI mode: 0
// Operating as: Master
// Data order: MSB First
// SCK clock prescaler: 4
// SCK clock doubled: On
// SCK clock frequency: 8000,000 kHz
SPIC.CTRL=SPI_ENABLE_bm | SPI_MODE_0_gc | SPI_MASTER_bm |
SPI_PRESCALER_DIV4_gc ;//| SPI_CLK2X_bm;
	// SPID interrupt: Disabled
	spi->INTCTRL=(spi->INTCTRL & (~SPI_INTLVL_gm)) | SPI_INTLVL_OFF_gc;
	
	// Note: the MOSI (PORTX Bit 5), SCK (PORTX Bit 7) and
	// /SS (PORTX Bit 4) signals are configured as outputs in the ports_init function
}

// SPID transmit/receive function in Master mode
// c - data to be transmitted
// Returns the received data
unsigned char classSPI::transfer(unsigned char c)
{
	// Transmit data in Master mode
	spi->DATA=c;
	// Wait for the data to be transmitted/received
	while ((spi->STATUS & SPI_IF_bm)==0);
	// Return the received data
	return spi->DATA;
}

// default destructor
classSPI::~classSPI()
{
} //~classSPI
