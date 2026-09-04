/* 
* classPorts.cpp
*
* Created: 04/02/2015 12:05:16
* Author: Vaio
*/


#include "classPorts.h"

// default constructor
classPorts::classPorts()
{
} //classPorts

// Ports initialization
void classPorts::ports_init(void)
{
	// PORTA initialization
	// OUT register
	PORTA.OUT=0b00010000;
	// Bit0: Input
	// Bit1: Input
	// Bit2: Input
	// Bit3: Input
	// Bit4: Input
	// Bit5: Input
	// Bit6: Input
	// Bit7: Input
	PORTA.DIR=0b00010000;
	// Bit0 Output/Pull configuration: Totempole/No
	// Bit0 Input/Sense configuration: Sense both edges
	// Bit0 inverted: Off
	// Bit0 slew rate limitation: Off
	PORTA.PIN0CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	// Bit1 Output/Pull configuration: Totempole/No
	// Bit1 Input/Sense configuration: Sense both edges
	// Bit1 inverted: Off
	// Bit1 slew rate limitation: Off
	PORTA.PIN1CTRL=PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
	// Bit2 Output/Pull configuration: Totempole/No
	// Bit2 Input/Sense configuration: Sense both edges
	// Bit2 inverted: Off
	// Bit2 slew rate limitation: Off
	PORTA.PIN2CTRL=PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
	// Bit3 Output/Pull configuration: Totempole/No
	// Bit3 Input/Sense configuration: Sense both edges
	// Bit3 inverted: Off
	// Bit3 slew rate limitation: Off
	PORTA.PIN3CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	// Bit4 Output/Pull configuration: Totempole/No
	// Bit4 Input/Sense configuration: Sense rising edge
	// Bit4 inverted: Off
	// Bit4 slew rate limitation: Off
	PORTA.PIN4CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_RISING_gc;
	// Bit5 Output/Pull configuration: Totempole/Pull-up (on input)
	// Bit5 Input/Sense configuration: Sense both edges
	// Bit5 inverted: Off
	// Bit5 slew rate limitation: Off
	PORTA.PIN5CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	// Bit6 Output/Pull configuration: Totempole/Pull-up (on input)
	// Bit6 Input/Sense configuration: Sense faling
	// Bit6 inverted: Off
	// Bit6 slew rate limitation: Off
	PORTA.PIN6CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_FALLING_gc;
	// Bit7 Output/Pull configuration: Totempole/Pull-up (on input)
	// Bit7 Input/Sense configuration: Sense both edges
	// Bit7 inverted: Off
	// Bit7 slew rate limitation: Off
	PORTA.PIN7CTRL= PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
	// Interrupt 0 level: Enabled Low Level
	// Interrupt 1 level: Disabled
	PORTA.INTCTRL=(PORTA.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |
	PORT_INT1LVL_OFF_gc | PORT_INT0LVL_LO_gc;
	// Bit0 pin change interrupt 0: Off
	// Bit1 pin change interrupt 0: Off
	// Bit2 pin change interrupt 0: Off
	// Bit3 pin change interrupt 0: Off
	// Bit4 pin change interrupt 0: On
	// Bit5 pin change interrupt 0: Off
	// Bit6 pin change interrupt 0: Off
	// Bit7 pin change interrupt 0: Off
	PORTA.INT0MASK=0b00100000;//pin6
	// Bit0 pin change interrupt 1: Off
	// Bit1 pin change interrupt 1: Off
	// Bit2 pin change interrupt 1: Off
	// Bit3 pin change interrupt 1: Off
	// Bit4 pin change interrupt 1: Off
	// Bit5 pin change interrupt 1: Off
	// Bit6 pin change interrupt 1: On
	// Bit7 pin change interrupt 1: Off
	PORTA.INT1MASK=0x00; //0x40

	// PORTB initialization
	// OUT register
	PORTB.OUT=0x00;
	// Bit0: Output
	// Bit1: Output
	// Bit2: Output
	// Bit3: Output
	PORTB.DIR=0x0F;
	// Bit0 Output/Pull configuration: Totempole/No
	// Bit0 Input/Sense configuration: Sense both edges
	// Bit0 inverted: Off
	// Bit0 slew rate limitation: Off
	PORTB.PIN0CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	// Bit1 Output/Pull configuration: Totempole/No
	// Bit1 Input/Sense configuration: Sense both edges
	// Bit1 inverted: Off
	// Bit1 slew rate limitation: Off
	PORTB.PIN1CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	// Bit2 Output/Pull configuration: Totempole/No
	// Bit2 Input/Sense configuration: Sense both edges
	// Bit2 inverted: Off
	// Bit2 slew rate limitation: Off
	PORTB.PIN2CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	// Bit3 Output/Pull configuration: Totempole/No
	// Bit3 Input/Sense configuration: Sense both edges
	// Bit3 inverted: Off
	// Bit3 slew rate limitation: Off
	PORTB.PIN3CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	// Interrupt 0 level: Disabled
	// Interrupt 1 level: Disabled
	PORTB.INTCTRL=(PORTB.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |
	PORT_INT1LVL_OFF_gc | PORT_INT0LVL_OFF_gc;
	// Bit0 pin change interrupt 0: Off
	// Bit1 pin change interrupt 0: Off
	// Bit2 pin change interrupt 0: Off
	// Bit3 pin change interrupt 0: Off
	PORTB.INT0MASK=0x00;
	// Bit0 pin change interrupt 1: Off
	// Bit1 pin change interrupt 1: Off
	// Bit2 pin change interrupt 1: Off
	// Bit3 pin change interrupt 1: Off
	PORTB.INT1MASK=0x00;


  
	// PORTD initialization
	PORTD.DIR=0xD7;
	// OUT register
	PORTD.OUT=0x18;
	// Pin0: Output
	// Pin1: Output
	// Pin2: Output
	// Pin3: Input
	// Pin4: Output
	// Pin5: Input
	// Pin6: Output
	// Pin7: Output
	
	// Pin0 Output/Pull configuration: Totempole/No
	// Pin0 Input/Sense configuration: Sense both edges
	// Pin0 Inverted: Off
	// Pin0 Slew Rate Limitation: Off
	PORTD.PIN0CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	// Pin1 Output/Pull configuration: Totempole/No
	// Pin1 Input/Sense configuration: Sense both edges
	// Pin1 Inverted: Off
	// Pin1 Slew Rate Limitation: Off
	PORTD.PIN1CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	// Pin2 Output/Pull configuration: Totempole/No
	// Pin2 Input/Sense configuration: Sense both edges
	// Pin2 Inverted: Off
	// Pin2 Slew Rate Limitation: Off
	PORTD.PIN2CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	// Pin3 Output/Pull configuration: Totempole/Pull-up (on input)
	// Pin3 Input/Sense configuration: Sense falling edge
	// Pin3 Inverted: Off
	// Pin3 Slew Rate Limitation: Off
	PORTD.PIN3CTRL=PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	// Pin4 Output/Pull configuration: Totempole/No
	// Pin4 Input/Sense configuration: Sense both edges
	// Pin4 Inverted: Off
	// Pin4 Slew Rate Limitation: Off
	PORTD.PIN4CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	// Pin5 Output/Pull configuration: Totempole/No
	// Pin5 Input/Sense configuration: Sense both edges
	// Pin5 Inverted: Off
	// Pin5 Slew Rate Limitation: Off
	PORTD.PIN5CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	// Pin6 Output/Pull configuration: Totempole/No
	// Pin6 Input/Sense configuration: Sense both edges
	// Pin6 Inverted: Off
	// Pin6 Slew Rate Limitation: Off
	PORTD.PIN6CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	// Pin7 Output/Pull configuration: Totempole/No
	// Pin7 Input/Sense configuration: Sense both edges
	// Pin7 Inverted: Off
	// Pin7 Slew Rate Limitation: Off
	PORTD.PIN7CTRL=PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	// Interrupt 0 level: Disabled
	// Interrupt 1 level: Disabled
	PORTD.INTCTRL=(PORTD.INTCTRL & (~(PORT_INT1LVL_gm | PORT_INT0LVL_gm))) |
	PORT_INT1LVL_OFF_gc | PORT_INT0LVL_OFF_gc;
	// Pin0 Pin Change interrupt 0: Off
	// Pin1 Pin Change interrupt 0: Off
	// Pin2 Pin Change interrupt 0: Off
	// Pin3 Pin Change interrupt 0: Off
	// Pin4 Pin Change interrupt 0: Off
	// Pin5 Pin Change interrupt 0: Off
	// Pin6 Pin Change interrupt 0: Off
	// Pin7 Pin Change interrupt 0: Off
	PORTD.INT0MASK=0x00;
	// Pin0 Pin Change interrupt 1: Off
	// Pin1 Pin Change interrupt 1: Off
	// Pin2 Pin Change interrupt 1: Off
	// Pin3 Pin Change interrupt 1: Off
	// Pin4 Pin Change interrupt 1: Off
	// Pin5 Pin Change interrupt 1: Off
	// Pin6 Pin Change interrupt 1: Off
	// Pin7 Pin Change interrupt 1: Off
	PORTD.INT1MASK=0x00;
 

     //PORTD.DIRSET = 0xFF;
     //PORTD.DIRCLR = 0x28;
     //PORTD.PIN3CTRL=PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;


	// PORTE initialization
	// OUT register
	PORTE.OUT=0xff;
	// Bit0: Output
	// Bit1: Output
	// Bit2: Input
	// Bit3: Output
	PORTE.DIR=0x0B;
	// Bit0 Output/Pull configuration: Totempole/No
	// Bit0 Input/Sense configuration: Sense both edges
	// Bit0 inverted: Off
	// Bit0 slew rate limitation: Off
}


// default destructor
classPorts::~classPorts()
{
} //~classPorts
