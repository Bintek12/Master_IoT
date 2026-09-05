/* 
* classTimers.cpp
*
* Created: 04/02/2015 12:21:25
* Author: Vaio
*/


#include "classTimers.h"

// default constructor
classTimers::classTimers()
{
} //classTimers

// Disable a Timer/Counter type 0
void classTimers::tc0_disable(TC0_t *ptc)
{
	// Timer/Counter off
	ptc->CTRLA=(ptc->CTRLA & (~TC0_CLKSEL_gm)) | TC_CLKSEL_OFF_gc;
	// Issue a reset command
	ptc->CTRLFSET=TC_CMD_RESET_gc;
}

// Disable a Timer/Counter type 1
void classTimers::tc1_disable(TC1_t *ptc)
{
	// Timer/Counter off
	ptc->CTRLA=(ptc->CTRLA & (~TC1_CLKSEL_gm)) | TC_CLKSEL_OFF_gc;
	// Issue a reset command
	ptc->CTRLFSET=TC_CMD_RESET_gc;
}

// Timer/Counter TCD0 initialization
//Control PWM asociado al Pid
void classTimers::tcd0_init(void)
{
	unsigned char s;

	// Note: the correct PORTD direction for the Compare Channels outputs
	// is configured in the ports_init function



	// Disable and reset the timer/counter just to be sure
	tc0_disable(&TCD0);
	// Clock source: Peripheral Clock/64
	TCD0.CTRLA=(TCD0.CTRLA & (~TC0_CLKSEL_gm)) | TC_CLKSEL_DIV2_gc;
	// Mode: Normal Operation, Overflow Int./Event on TOP
	// Compare/Capture on channel A: Off
	// Compare/Capture on channel B: Off
	// Compare/Capture on channel C: Off
	// Compare/Capture on channel D: Off
	TCD0.CTRLB=(TCD0.CTRLB & (~(TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm | TC0_CCDEN_bm | TC0_WGMODE_gm))) |
	TC_WGMODE_NORMAL_gc;

	// Capture event source: None
	// Capture event action: None
	TCD0.CTRLD=(TCD0.CTRLD & (~(TC0_EVACT_gm | TC0_EVSEL_gm))) |
	TC_EVACT_OFF_gc | TC_EVSEL_OFF_gc;

	// Overflow interrupt: Low Level
	// Error interrupt: Disabled
	TCD0.INTCTRLA=(TCD0.INTCTRLA & (~(TC0_ERRINTLVL_gm | TC0_OVFINTLVL_gm))) |
	TC_ERRINTLVL_OFF_gc | TC_OVFINTLVL_LO_gc;

	// Compare/Capture channel A interrupt: Enabled
	// Compare/Capture channel B interrupt: Disabled
	// Compare/Capture channel C interrupt: Disabled
	// Compare/Capture channel D interrupt: Disabled
	TCD0.INTCTRLB=(TCD0.INTCTRLB & (~(TC0_CCDINTLVL_gm | TC0_CCCINTLVL_gm | TC0_CCBINTLVL_gm | TC0_CCAINTLVL_gm))) |
	TC_CCDINTLVL_OFF_gc | TC_CCCINTLVL_OFF_gc | TC_CCBINTLVL_OFF_gc | TC_CCAINTLVL_LO_gc;
	

	// Clear the interrupt flags
	TCD0.INTFLAGS=TCD0.INTFLAGS;
	// Set counter register
	TCD0.CNT=0x00;
	// Set period register
	TCD0.PER=400;
	// Set channel A Compare/Capture register
	TCD0.CCA=100;
	// Set channel B Compare/Capture register
	TCD0.CCB=3000;
	// Set channel C Compare/Capture register
	TCD0.CCC=0x00;
	// Set channel D Compare/Capture register
	TCD0.CCD=0x00;

}



// Timer/Counter TCD1 initialization
//Perro guardián de la comunicación rs485
void classTimers::tcd1_init(void)
{
	unsigned char s;

	// Note: the correct PORTD direction for the Compare Channels outputs
	// is configured in the ports_init function

	// Save interrupts enabled/disabled state
	s=SREG;
	// Disable interrupts
	asm("cli");

	// Disable and reset the timer/counter just to be sure
	tc1_disable(&TCD1);
	// Clock source: Peripheral Clock/2
	TCD1.CTRLA=(TCD1.CTRLA & (~TC1_CLKSEL_gm)) | TC_CLKSEL_DIV4_gc;
	// Mode: Normal Operation, Overflow Int./Event on TOP
	// Compare/Capture on channel A: Off
	// Compare/Capture on channel B: Off
	TCD1.CTRLB=(TCD1.CTRLB & (~(TC1_CCAEN_bm | TC1_CCBEN_bm | TC1_WGMODE_gm))) |
	TC_WGMODE_NORMAL_gc;

	// Capture event source: None
	// Capture event action: None
	TCD1.CTRLD=(TCD1.CTRLD & (~(TC1_EVACT_gm | TC1_EVSEL_gm))) |
	TC_EVACT_OFF_gc | TC_EVSEL_OFF_gc;

	// Overflow interrupt: Disabled
	// Error interrupt: Disabled
	TCD1.INTCTRLA=0x00;

	// Compare/Capture channel A interrupt: Disabled
	// Compare/Capture channel B interrupt: Disabled
	TCD1.INTCTRLB=(TCD1.INTCTRLB & (~(TC1_CCBINTLVL_gm | TC1_CCAINTLVL_gm))) |
	TC_CCBINTLVL_OFF_gc | TC_CCAINTLVL_OFF_gc;

	
	// Clear the interrupt flags
	TCD1.INTFLAGS=TCD1.INTFLAGS;
	// Set counter register
	TCD1.CNT=0x0000;
	// Set period register
	TCD1.PER=0xF9FF;
	// Set channel A Compare/Capture register
	TCD1.CCA=0x0000;
	// Set channel B Compare/Capture register
	TCD1.CCB=0x0000;

	// Restore interrupts enabled/disabled state
	SREG=s;
}

//Frecuencia de la señal analógica

// Timer/Counter TCE0 initialization
void classTimers::tce0_init(uint16_t per)
{
	unsigned char s;

	// Note: the correct PORTE direction for the Compare Channels outputs
	// is configured in the ports_init function

	// Save interrupts enabled/disabled state
	s=SREG;
	// Disable interrupts
	asm("cli");

	// Clock source: Peripheral Clock
	TCE0.CTRLA=(TCE0.CTRLA & (~TC0_CLKSEL_gm)) | TC_CLKSEL_DIV1_gc;
	// Mode: Normal Operation, Overflow Int./Event on TOP
	// Compare/Capture on channel A: Off
	// Compare/Capture on channel B: Off
	// Compare/Capture on channel C: Off
	// Compare/Capture on channel D: Off
	TCE0.CTRLB=(TCE0.CTRLB & (~(TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm | TC0_CCDEN_bm | TC0_WGMODE_gm))) |
	TC_WGMODE_NORMAL_gc;

	// Capture event source: None
	// Capture event action: None
	TCE0.CTRLD=(TCE0.CTRLD & (~(TC0_EVACT_gm | TC0_EVSEL_gm))) |
	TC_EVACT_OFF_gc | TC_EVSEL_OFF_gc;

	// Overflow interrupt: Enable
	// Error interrupt: Disabled
	TCE0.INTCTRLA=(TCE0.INTCTRLA & (~(TC0_ERRINTLVL_gm | TC0_OVFINTLVL_gm))) |
	TC_ERRINTLVL_OFF_gc | TC_OVFINTLVL_LO_gc;
    //TCE0.INTCTRLA = 0; //???????????????????????????????????????????????borrar
	// Compare/Capture channel A interrupt: Low Level
	// Compare/Capture channel B interrupt: Low Level
	// Compare/Capture channel C interrupt: Disabled
	// Compare/Capture channel D interrupt: Disabled
	TCE0.INTCTRLB=(TCE0.INTCTRLB & (~(TC0_CCDINTLVL_gm | TC0_CCCINTLVL_gm | TC0_CCBINTLVL_gm | TC0_CCAINTLVL_gm))) |
	TC_CCDINTLVL_OFF_gc | TC_CCCINTLVL_OFF_gc | TC_CCDINTLVL_OFF_gc | TC_CCDINTLVL_OFF_gc;


	// Clear the interrupt flags
	TCE0.INTFLAGS=TCE0.INTFLAGS;
	// Set counter register
	TCE0.CNT=0x0000;
	// Set period register
	TCE0.PER=per;
	// Set channel A Compare/Capture register
	TCE0.CCA=0x0000;
	// Set channel B Compare/Capture register
	TCE0.CCB=0x0000;
	// Set channel C Compare/Capture register
	TCE0.CCC=0x0000;
	// Set channel D Compare/Capture register
	TCE0.CCD=0x0000;

	// Restore interrupts enabled/disabled state
	SREG=s;
}


// default destructor
classTimers::~classTimers()
{
} //~classTimers
