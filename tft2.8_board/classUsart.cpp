/* 
* classUsart.cpp
*
* Created: 04/02/2015 12:15:04
* Author: Vaio
*/


#include "classUsart.h"

// default constructor
classUsart::classUsart(USART_t *usart)
{
	current_usart = usart;
} //classUsart

// USARTD0 initialization
void classUsart::usart0_init()
{
    PORTE.DIRCLR = PIN2_bm;//rx
	PORTE.DIRSET = PIN3_bm;//tx
	PORTD.DIRSET = PIN1_bm;//DE
	PORTD.OUTCLR = PIN1_bm;
	// Communication mode: Asynchronous USART
	// Data bits: 8
	// Stop bits: 1
	// Parity: Disabled
	current_usart->CTRLC=USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;

	// Receive complete interrupt: High Level
	// Transmit complete interrupt: Disabled
	// Data register empty interrupt: Disabled
	current_usart->CTRLA=(current_usart->CTRLA & (~(USART_RXCINTLVL_gm | USART_TXCINTLVL_gm | USART_DREINTLVL_gm))) |
	USART_RXCINTLVL_MED_gc | USART_TXCINTLVL_OFF_gc | USART_DREINTLVL_OFF_gc;

	// Required Baud rate: 38400
	// Real Baud Rate: 38403,8 (x1 Mode), Error: 0,0 %
	current_usart->BAUDCTRLA=0xC5;
	current_usart->BAUDCTRLB=((0x0A << USART_BSCALE_bp) & USART_BSCALE_gm) | 0x0C;

	// Receiver: On
	// Transmitter: On
	// Double transmission speed mode: Off
	// Multi-processor communication mode: Off
	current_usart->CTRLB=(current_usart->CTRLB & (~(USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm | USART_MPCM_bm | USART_TXB8_bm))) |
	USART_RXEN_bm | USART_TXEN_bm;
}






// default destructor
classUsart::~classUsart()
{
} //~classUsart
