/* 
* classUsart.h
*
* Created: 04/02/2015 12:15:04
* Author: Vaio
*/


#ifndef __CLASSUSART_H__
#define __CLASSUSART_H__

#include <avr/io.h>
#define USART_BSCALE_bp 4 // Baud Rate Scale group position
#define RX_BUFFER_SIZE         264
#define TX_BUFFER_SIZE         4
#define RS485_BUS_DELAY        10            //uSegundos


class classUsart
{
//variables
public:
     USART_t *current_usart;
   	 unsigned int rx_index;
   	 unsigned char tx_index;
   	 unsigned char rx_buffer[RX_BUFFER_SIZE];
   	 unsigned char tx_buffer[TX_BUFFER_SIZE];
   	 unsigned char dirRs485;
   	 unsigned char last_rx_cmd;

protected:
private:

//functions
public:
	classUsart(USART_t *usart);
	void usart0_init();
	
	~classUsart();
protected:
private:
	classUsart( const classUsart &c );
	classUsart& operator=( const classUsart &c );

}; //classUsart

#endif //__CLASSUSART_H__
