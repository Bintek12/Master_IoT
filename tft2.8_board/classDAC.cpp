/* 
* classDAC.cpp
*
* Created: 11/03/2015 15:05:04
* Author: Vaio
*/


#include "classDAC.h"

// default constructor
classDAC::classDAC()
{
} //classDAC


// DACB initialization
void classDAC::dacb_init(void)
{
// Operating mode: Single Channel (Ch0)
// Channel 0 triggered by the event system: On
DACB.CTRLB=(DACB.CTRLB & (~(DAC_CHSEL_gm | DAC_CH0TRIG_bm | DAC_CH1TRIG_bm))) |
DAC_CHSEL_SINGLE_gc | DAC_CH0TRIG_bm;

// Reference: AREF on PORTA
// Left adjust value: Off
DACB.CTRLC=(DACB.CTRLC & (~(DAC_REFSEL_gm | DAC_LEFTADJ_bm))) |
DAC_REFSEL_INT1V_gc;

// DACB is triggered by the Event System Channel 0
DACB.EVCTRL=(DACB.EVCTRL & (~DAC_EVSEL_gm)) | DAC_EVSEL_0_gc;

// DACB is enabled
// Low power mode: Off
// Channel 0 output: On
// Channel 1 output: Off
// Internal output connected to the ADCB and Analog Comparator MUX-es: Off
DACB.CTRLA=(DACB.CTRLA & (~(DAC_IDOEN_bm | DAC_CH0EN_bm | DAC_CH1EN_bm | DAC_LPMODE_bm))) |
DAC_CH0EN_bm | DAC_ENABLE_bm;}

// Function used to write data to a DACB channel ch
void classDAC::dacb0_write(unsigned int data)
{
	// Wait for the channel data register to be ready for new data
	while ((DACB.STATUS & DAC_CH0DRE_bm)==0);
	// Write new data to the channel data register
	DACB.CH0DATA=data;
}

// default destructor
classDAC::~classDAC()
{
} //~classDAC
