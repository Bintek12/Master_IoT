/* 
* classADC.cpp
*
* Created: 11/03/2015 15:19:27
* Author: Vaio
*/


#include "classADC.h"

// default constructor
classADC::classADC()
{
} //classADC


unsigned char classADC::SP_ReadCalibrationByte( unsigned char index )
{
	unsigned char result;

	/* Load the NVM Command register to read the calibration row. */
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
	result = pgm_read_byte(index);

	/* Clean up NVM Command register. */
	NVM_CMD = NVM_CMD_NO_OPERATION_gc;

	return result;
}

// ADCA initialization

void classADC::adca_init(void)
{

	// ADCA is enabled
	// Resolution: 12 Bits
	// Load the calibration value for 12 Bit resolution
	// from the signature row
	ADCA.CALL=SP_ReadCalibrationByte(PROD_SIGNATURES_START+ADCACAL0_offset);
	ADCA.CALH=SP_ReadCalibrationByte(PROD_SIGNATURES_START+ADCACAL1_offset);

	// Free Running mode: Off
	// Gain stage impedance mode: High-impedance sources
	// Current consumption: No limit
	// Conversion mode: signed
	ADCA.CTRLB=(ADCA.CTRLB & (~(ADC_IMPMODE_bm | ADC_CURRLIMIT_gm | ADC_CONMODE_bm | ADC_FREERUN_bm | ADC_RESOLUTION_gm))) |
	ADC_CURRLIMIT_NO_gc | ADC_RESOLUTION_12BIT_gc;

	// Clock frequency: 62,500 kHz
	ADCA.PRESCALER=(ADCA.PRESCALER & (~ADC_PRESCALER_gm)) | ADC_PRESCALER_DIV64_gc;

	// Reference: VCC
	// Temperature reference: Off
	ADCA.REFCTRL=(ADCA.REFCTRL & ((~(ADC_REFSEL_gm | ADC_TEMPREF_bm)) | ADC_BANDGAP_bm)) |
	ADC_REFSEL_INTVCC_gc;

	// Initialize the ADC Compare register
	ADCA.CMPL=0x00;
	ADCA.CMPH=0x00;

	// AD conversion is started by software
	ADCA.EVCTRL=ADC_EVACT_NONE_gc;

	// Channel 0 interrupt: Disabled
	ADCA.CH0.INTCTRL=(ADCA.CH0.INTCTRL & (~(ADC_CH_INTMODE_gm | ADC_CH_INTLVL_gm))) |
	ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_OFF_gc;
	// Channel 1 interrupt: Disabled
	ADCA.CH1.INTCTRL=(ADCA.CH1.INTCTRL & (~(ADC_CH_INTMODE_gm | ADC_CH_INTLVL_gm))) |
	ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_OFF_gc;
	// Channel 2 interrupt: Disabled
	ADCA.CH2.INTCTRL=(ADCA.CH2.INTCTRL & (~(ADC_CH_INTMODE_gm | ADC_CH_INTLVL_gm))) |
	ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_OFF_gc;
	// Channel 3 interrupt: Disabled
	ADCA.CH3.INTCTRL=(ADCA.CH3.INTCTRL & (~(ADC_CH_INTMODE_gm | ADC_CH_INTLVL_gm))) |
	ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_OFF_gc;
	
	
		// ADC channel 0 gain: 1
	// ADC channel 0 input mode: Differential input signal
	ADCA.CH0.CTRL=(ADCA.CH0.CTRL & (~(ADC_CH_START_bm | ADC_CH_GAIN_gm | ADC_CH_INPUTMODE_gm))) |
	ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;

	// ADC channel 0 positive input: ADC0 pin
	// ADC channel 0 negative input: PAD_GND
	ADCA.CH0.MUXCTRL=(ADCA.CH0.MUXCTRL & (~(ADC_CH_MUXPOS_gm | ADC_CH_MUXNEG_gm))) |
	ADC_CH_MUXPOS_PIN0_gc ; 

	// ADC channel 1 gain: 1
	// ADC channel 1 input mode: Differential input signal
	ADCA.CH1.CTRL=(ADCA.CH1.CTRL & (~(ADC_CH_START_bm | ADC_CH_GAIN_gm | ADC_CH_INPUTMODE_gm))) |
	ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;

	// ADC channel 1 positive input: ADC1 pin
	// ADC channel 1 negative input: PAD_GND
	ADCA.CH1.MUXCTRL=(ADCA.CH1.MUXCTRL & (~(ADC_CH_MUXPOS_gm | ADC_CH_MUXNEG_gm))) |
	ADC_CH_MUXPOS_PIN1_gc;

	// ADC channel 2 gain: 1
	// ADC channel 2 input mode: Differential input signal
	ADCA.CH2.CTRL=(ADCA.CH2.CTRL & (~(ADC_CH_START_bm | ADC_CH_GAIN_gm | ADC_CH_INPUTMODE_gm))) |
	ADC_CH_GAIN_1X_gc |ADC_CH_INPUTMODE_SINGLEENDED_gc;

	// ADC channel 2 positive input: ADC2 pin
	// ADC channel 2 negative input: PAD_GND
	ADCA.CH2.MUXCTRL=(ADCA.CH2.MUXCTRL & (~(ADC_CH_MUXPOS_gm | ADC_CH_MUXNEG_gm))) |
	ADC_CH_MUXPOS_PIN2_gc ;

	// Enable the ADC
	ADCA.CTRLA|=ADC_ENABLE_bm;
	// Insert a delay to allow the ADC common mode voltage to stabilize
	_delay_us(2);
}


// ADCA channel data read function using polled mode
unsigned int classADC::adca_read(unsigned char channel)
{
	ADC_CH_t *pch=&ADCA.CH0+channel;
	signed int data;

	// Start the AD conversion
	pch->CTRL|=ADC_CH_START_bm;
	// Wait for the AD conversion to complete
	while ((pch->INTFLAGS & ADC_CH_CHIF_bm)==0);
	// Clear the interrupt flag
	pch->INTFLAGS=ADC_CH_CHIF_bm;
	// Read the AD conversion result
	((unsigned char *) &data)[0]=pch->RESL;
	((unsigned char *) &data)[1]=pch->RESH;
	return data ;
}




// default destructor
classADC::~classADC()
{
} //~classADC
