/* 
* classXmegaRtc.cpp
*
* Created: 11/03/2015 17:15:16
* Author: Vaio
*/


#include "classXmegaRtc.h"

// default constructor
classXmegaRtc::classXmegaRtc()
{
} //classXmegaRtc

// default destructor
classXmegaRtc::~classXmegaRtc()
{
} //~classXmegaRtc




/*! \brief This function initializes the RTC with period, initial count,
 *         compare value and clock prescaler.
 *
 *  All the synchronized registers are written at the same time to save time.
 *  The program code need to check if the RTC is busy before calling this
 *  function. The RTC_Busy() function can be used to do this.
 *
 *  \param period         RTC period. Topvalue = Period - 1.
 *  \param count          Initial RTC count.
 *  \param compareValue   Compare value.
 *  \param prescaler      Clock prescaler setting.
 */
void classXmegaRtc::RTC_Initialize( uint16_t period,
                     uint16_t count,
                     uint16_t compareValue,
                     RTC_PRESCALER_t prescaler )
{
	RTC.PER = period - 1;
	RTC.CNT = count;
	RTC.COMP = compareValue;
	RTC.CTRL = ( RTC.CTRL & ~RTC_PRESCALER_gm ) | prescaler;
}


/*! \brief This function sets the RTC overflow interrupt level.
 *
 *  \param intLevel The overflow interrupt level.
 */
void classXmegaRtc::RTC_SetOverflowIntLevel( RTC_OVFINTLVL_t intLevel )
{
	RTC.INTCTRL = ( RTC.INTCTRL & ~RTC_OVFINTLVL_gm ) | intLevel;
}


/*! \brief This function sets the RTC compare interrupt level.
 *
 *  \param intLevel The compare interrupt level.
 */
void classXmegaRtc::RTC_SetCompareIntLevel( RTC_COMPINTLVL_t intLevel )
{
	RTC.INTCTRL = ( RTC.INTCTRL & ~RTC_COMPINTLVL_gm ) | intLevel;
}


/*! \brief This function sets both compare and overflow interrupt levels in
 *         one go.
 *
 *  \param ovfIntLevel  The overflow interrupt level.
 *  \param compIntLevel The compare interrupt level.
 */
void classXmegaRtc::RTC_SetIntLevels( RTC_OVFINTLVL_t ovfIntLevel,
                       RTC_COMPINTLVL_t compIntLevel )
{
	RTC.INTCTRL = ( RTC.INTCTRL &
	              ~( RTC_COMPINTLVL_gm | RTC_OVFINTLVL_gm ) ) |
	              ovfIntLevel |
	              compIntLevel;
}


/*! \brief This function sets a timeout alarm.
 *
 *  This function sets a timeout alarm by adding the timeout to the current
 *  count value. If the resulting alarm value is larger than the RTC period
 *  value, it will wrap around. An RTC compare interrupt will be triggered¨
 *  after the specified timeout.
 *
 *  \note The program code needs to check if the RTC is busy before calling
 *        this function. The RTC_Busy() function can be used to do this.
 *
 *  \note The timeout must be less than the timer period. The timeout should not
 *        be set too low, as the timeout may be missed, depending on CPU speed.
 *
 *  \param alarmTimeout Timeout time in RTC clock cycles (scaled).
 */
void classXmegaRtc::RTC_SetAlarm( uint16_t alarmTimeout )
{
	/* Calculate compare time. */
	uint16_t compareValue = RTC.CNT + alarmTimeout;

	/* Wrap on period. */
	if (compareValue > RTC.PER){
		compareValue -= RTC.PER;
	}

	/* Add the timeout value to get the absolute time of the alarm. */
	RTC.COMP = compareValue;
}


/*! \brief This function sets a new RTC clock prescaler value.
 *
 *  \note The program code need to check if the RTC is busy before calling
 *        this function. The RTC_Busy() function can be used to do this.
 *
 *  \param prescaler Clock prescaler setting.
 */
void classXmegaRtc::RTC_SetPrescaler( RTC_PRESCALER_t prescaler )
{
	RTC.CTRL = ( RTC.CTRL & ~RTC_PRESCALER_gm ) | prescaler;
}


void classXmegaRtc::rtc_init(void)
{
	/* Turn on internal 32kHz. */
	OSC.CTRL |= OSC_RC32KEN_bm;

	do {
		/* Wait for the 32kHz oscillator to stabilize. */
	} while ( ( OSC.STATUS & OSC_RC32KRDY_bm ) == 0);


	/* Set internal 32kHz oscillator as clock source for RTC. */
	CLK.RTCCTRL = CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm;



	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );

	/* Configure RTC period to 10 milisecond. */
	RTC_Initialize( 10, 0, 0, RTC_PRESCALER_DIV1_gc );

	/* Enable overflow interrupt. */
	RTC_SetIntLevels( RTC_OVFINTLVL_LO_gc, RTC_COMPINTLVL_OFF_gc );
}

