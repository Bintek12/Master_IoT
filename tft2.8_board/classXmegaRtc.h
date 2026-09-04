/* 
* classXmegaRtc.h
*
* Created: 11/03/2015 17:15:16
* Author: Vaio
*/


#ifndef __CLASSXMEGARTC_H__
#define __CLASSXMEGARTC_H__

#include "avr_compiler.h"

/* Definitions of macros. */

/*! \brief This macro checks the RTC busy flag.
 *
 *  \return Non-Zero if RTC is busy, zero otherwise.
 */
#define RTC_Busy()               ( RTC.STATUS & RTC_SYNCBUSY_bm )

/*! \brief This macro returns the status of the RTC overflow flag.
 *
 *  \return Non-Zero if overflow has occured, zero otherwise.
 */
#define RTC_GetOverflowFlag()    ( RTC.INTFLAGS & RTC_OVFIF_bm )

/*! \brief This macro returns the status of the RTC compare flag.
 *
 *  \return Non-Zero if an RTC compare has occured, zero otherwise.
 */
#define RTC_GetCompareFlag()     ( RTC.INTFLAGS & RTC_COMPIF_bm )

/*! \brief This macro sets a new RTC count value.
 *
 *  This is the count value of the RTC, which will count up to the RTC period.
 *
 *  \note The program code needs to check if the RTC is busy before calling
 *        this function. The RTC_Busy() function can be used to do this.
 *
 *  \param _rtccnt The new count value.
 */
#define RTC_SetCount( _rtccnt )  ( RTC.CNT = (_rtccnt) )

/*! \brief This macro returns the current RTC count value.
 *
 *  This is the count value of the RTC, which will count up to the RTC period.
 *
 *  \return The current RTC count value.
 */
#define RTC_GetCount()           ( RTC.CNT )

/*! \brief This macro sets a new RTC period.
 *
 *  This is the period value of the RTC, which is the top of the count sequence.
 *
 *  \note The program code needs to check if the RTC is busy before calling
 *        this function. The RTC_Busy() function can be used to do this.
 *
 *  \param _rtcper The new RTC period.
 */
#define RTC_SetPeriod( _rtcper ) ( RTC.PER = (_rtcper) )

/*! \brief This macro returns the current RTC period value.
 *
 *  This is the period value of the RTC, which is the top of the count sequence.
 *
 *  \return The current RTC period value.
 */
#define RTC_GetPeriod()          ( RTC.PER )

/*! \brief This macro sets a new RTC compare value.
 *
 *  This is the compare value of the RTC, which will be compared to the count
 *  sequence. The compare value must be less than the RTC period.
 *
 *  \note The program code needs to check if the RTC is busy before calling
 *        this function. The RTC_Busy() function can be used to do this.
 *
 *  \param _cmpVal   The new compare value.
 */
#define RTC_SetCompareValue( _cmpVal ) ( RTC.COMP = (_cmpVal) )

/*! \brief This macro returns the current RTC compare value.
 *
 *  This is the compare value of the RTC, which will be compared to the count
 *  sequence. The compare value must be less than the RTC period.
 *
 *  \return  The current compare value.
 */
#define RTC_GetCompareValue()    ( RTC.COMP )

class classXmegaRtc
{
//variables
public:
protected:
private:

//functions
public:
	classXmegaRtc();
	~classXmegaRtc();
    void RTC_Initialize( uint16_t period,
                         uint16_t count,
                         uint16_t compareValue,
                         RTC_PRESCALER_t prescaler);
    void RTC_SetOverflowIntLevel( RTC_OVFINTLVL_t intLevel );
    void RTC_SetCompareIntLevel( RTC_COMPINTLVL_t intLevel );
    void RTC_SetIntLevels( RTC_OVFINTLVL_t ovfIntLevel,
    RTC_COMPINTLVL_t compIntLevel );
    void RTC_SetAlarm( uint16_t alarmTimeout );
    void RTC_SetPrescaler( RTC_PRESCALER_t prescaler );
    void rtc_init(void);	
protected:
private:
	classXmegaRtc( const classXmegaRtc &c );
	classXmegaRtc& operator=( const classXmegaRtc &c );

}; //classXmegaRtc

#endif //__CLASSXMEGARTC_H__
