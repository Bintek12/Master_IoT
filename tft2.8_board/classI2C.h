/* 
* classI2C.h
*
* Created: 16/02/2015 11:49:04
* Author: Vaio
*/


#ifndef __CLASSI2C_H__
#define __CLASSI2C_H__

// I/O Registers definitions
#include <avr/io.h>
#include "UTFT.h"
#include "classAD7843.h"
#define RTC_SLAVE_ADDRESS           0xD0; // already shifted !!!!

/// Structure to store time and date
typedef struct
{
	uint8_t sec;            ///< Seconds:       0  to 59
	uint8_t min;            ///< Minutes:       0  to 59
	uint8_t hour;           ///< Hours:         0  to 23
	uint8_t day_of_week;    ///< Day of Week:   1  to 7
	uint8_t day_of_month;   ///< Days of Month: 1  to 31 (depending on month)
	uint8_t month;          ///< Months:        1  to 12
	uint8_t year;           ///< Years:         00 to 99
	uint8_t creg;
} m41t00_time_t;



extern UTFT utft;
extern classAD7843 AD7843;

class classI2C
{
//variables
public:
	m41t00_time_t decoded_time;
	m41t00_time_t time;
	bool show_time;
protected:
private:
	//m41t00_time_t time;
    char segundos;
    char minutos;
    char horas;
    char dia;
	bool set_time;
	bool set_minutos;
	bool set_hora;
	bool set_dia;
    uint16_t time_set_delay_counter;
	uint8_t time_set_timer;
//functions
public:
	classI2C();
	void twi_init(TWI_t * twiname,long F_syst,long F_i2c);
	void twi_write_byte(TWI_t *twiname,uint8_t i2c_address, uint8_t byte);
	uint8_t twi_read_byte(TWI_t *twiname, uint8_t i2c_address);
	void twi_write_rtc(TWI_t *twiname);
	void twi_read_rtc(TWI_t *twiname);
	void time_date_read(void);
	void  setTime_keys(void);
	~classI2C();
protected:
private:
    uint8_t m41t00_bcd_to_byte(uint8_t bcd_val);
	uint8_t m41t00_byte_to_bcd(uint8_t byte_val);
	void  minute_adjust();
	void  hour_adjust();
	void  day_adjust();
	void  Time_adjust(void);
	classI2C( const classI2C &c );
	classI2C& operator=( const classI2C &c );

}; //classI2C

#endif //__CLASSI2C_H__
