/* 
* classClock.cpp
*
* Created: 04/02/2015 11:36:07
* Author: Vaio
*/


#include "classClock.h"

// default constructor
classClock::classClock()
{
} //classClock


void classClock::system_clocks_init(void)
{
	unsigned char n,s;


	// Save interrupts enabled/disabled state
	s=SREG;
	// Disable interrupts
	asm("cli");

	// Internal 32 MHz RC oscillator initialization
	// Enable the internal 32 MHz RC oscillator
	OSC.CTRL|=OSC_RC32MEN_bm;

	// System Clock prescaler A division factor: 1
	// System Clock prescalers B & C division factors: B:1, C:1
	// ClkPer4: 32000,000 kHz
	// ClkPer2: 32000,000 kHz
	// ClkPer:  32000,000 kHz
	// ClkCPU:  32000,000 kHz
	n=(CLK.PSCTRL & (~(CLK_PSADIV_gm | CLK_PSBCDIV1_bm | CLK_PSBCDIV0_bm))) |
	CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc;
	//CCP=CCP_IOREG_gc;
	
	asm("ldi     r18, 0xD8");                 // Load magic CCP value
	asm(" out     52, r18");                  // Start CCP handshake CCP
	asm("ldi     r18, 0x00");                 // Write value to I/O register
	asm(" sts     0x41, r18");                //&PSCTRL = 0x41
	//CLK.PSCTRL=n;

	// Disable the autocalibration of the internal 32 MHz RC oscillator
	DFLLRC32M.CTRL&= ~DFLL_ENABLE_bm;

	// Wait for the internal 32 MHz RC oscillator to stabilize
	while ((OSC.STATUS & OSC_RC32MRDY_bm)==0);

	// Select the system clock source: 32 MHz Internal RC Osc.
	n=(CLK.CTRL & (~CLK_SCLKSEL_gm)) | CLK_SCLKSEL_RC32M_gc;
	//CCP=CCP_IOREG_gc;
	asm("ldi     r18, 0xD8");            // Load magic CCP value
	asm(" out     52, r18");                 // Start CCP handshake CCP
	asm("ldi     r18, 0x01");                 // Write value to I/O register
	asm(" sts     0x40, r18");               //&CTRL = 0x40
	
	//CLK.CTRL=0x01;

	// Disable the unused oscillators: 2 MHz, internal 32 kHz, external clock/crystal oscillator, PLL
	OSC.CTRL&= ~(OSC_RC2MEN_bm | OSC_RC32KEN_bm | OSC_XOSCEN_bm | OSC_PLLEN_bm);

	// Lock the CLK.CTRL and CLK.PSCTRL registers
	n=CLK.LOCK | CLK_LOCK_bm;
	//CCP=CCP_IOREG_gc;
	asm("ldi     r18, 0xD8");            // Load magic CCP value
	asm(" out     52, r18");                 // Start CCP handshake CCP
	asm("ldi     r18, 0x01");                 // Write value to I/O register
	asm(" sts     0x42, r18");                //&LOCK = 0x42

	//CLK.LOCK=0x01;

	// Peripheral Clock output: Disabled
	PORTCFG.CLKEVOUT=(PORTCFG.CLKEVOUT & (~PORTCFG_CLKOUT_gm)) | PORTCFG_CLKOUT_OFF_gc;

	// Restore interrupts enabled/disabled state
	SREG=s;

}


// default destructor
classClock::~classClock()
{
} //~classClock
