/* 
* classI2C.cpp
*
* Created: 16/02/2015 11:49:03
* Author: Vaio
*/


#include "classI2C.h"

// default constructor
classI2C::classI2C()
{
} //classI2C


/****************************************************/

void classI2C::twi_init(TWI_t * twiname,long F_syst,long F_i2c)
{
	//TWIE
	PORTE.DIRCLR = PIN0_bm |PIN1_bm;
	
	twiname->MASTER.CTRLB = TWI_MASTER_SMEN_bm;
	twiname->MASTER.BAUD = ((F_syst / (2 * F_i2c)) - 5);
	twiname->MASTER.CTRLA = TWI_MASTER_ENABLE_bm;
	twiname->MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}

/****************************************************/

void classI2C::twi_write_byte(TWI_t *twiname,uint8_t i2c_address, uint8_t byte)
{

	twiname->MASTER.ADDR = i2c_address<<1;  // write to slave
	while(!(twiname->MASTER.STATUS&TWI_MASTER_WIF_bm));
	twiname->MASTER.DATA = byte;       // write word addr
	while(!(twiname->MASTER.STATUS&TWI_MASTER_WIF_bm));

}



/****************************************************/

uint8_t classI2C::twi_read_byte(TWI_t *twiname, uint8_t i2c_address)
{
	// read from i2c slave
	twiname->MASTER.ADDR = i2c_address<<1 | 0x01;//Read from slave
	while(!(twiname->MASTER.STATUS&TWI_MASTER_WIF_bm));
	twiname->MASTER.DATA = 0x00;       // write word addrpointer first
	while(!(twiname->MASTER.STATUS&TWI_MASTER_RIF_bm));
    return twiname->MASTER.DATA;

}




/****************************************************/
void classI2C::twi_write_rtc(TWI_t *twiname)
{

	uint8_t i;
	uint8_t *writeData;
	writeData =(unsigned char *) &time;
	twiname->MASTER.CTRLC = 0;
	twiname->MASTER.ADDR = RTC_SLAVE_ADDRESS;  // write to RTC
	while(!(twiname->MASTER.STATUS&TWI_MASTER_WIF_bm));
	twiname->MASTER.DATA = 0x00;       // write word addr
	while(!(twiname->MASTER.STATUS&TWI_MASTER_WIF_bm));
	for(i=0;i<8;i++)
	{                  // write date and time
		twiname->MASTER.DATA =*writeData++;
		while(!(twiname->MASTER.STATUS & TWI_MASTER_WIF_bm));
	}
    twiname->MASTER.CTRLC = 0x07;

}

/****************************************************/

void classI2C::twi_read_rtc(TWI_t *twiname)
{
	// read from RTC
	uint8_t i;
	uint8_t *readData;
	readData =(unsigned char *) &time;
	uint8_t address = RTC_SLAVE_ADDRESS;
	address |= 0x01;
	twiname->MASTER.CTRLC = 0;
	twiname->MASTER.ADDR = RTC_SLAVE_ADDRESS;
	while(!(twiname->MASTER.STATUS & TWI_MASTER_WIF_bm));
	
	twiname->MASTER.DATA = 0x00;       // write word addrpointer first
	twiname->MASTER.ADDR = address;    // send read command
	//twiname->MASTER.CTRLC = TWI_MASTER_ACKACT_bm | 0x02;//??
	for(i=0;i<8;i++)
	{   // read date and time
		while(!(twiname->MASTER.STATUS&TWI_MASTER_RIF_bm));
		//twiname->MASTER.CTRLC = 2;
		*readData++ = twiname->MASTER.DATA;
	}
	
	twiname->MASTER.CTRLC = 7;
	
	decoded_time.sec = m41t00_bcd_to_byte(time.sec);
	decoded_time.min = m41t00_bcd_to_byte(time.min & 0x7F);
	decoded_time.hour = m41t00_bcd_to_byte(time.hour & 0x3F) ;
	decoded_time.day_of_week = m41t00_bcd_to_byte(time.day_of_week & 0x07) ;
    decoded_time.day_of_month =m41t00_bcd_to_byte(time.day_of_month);
	decoded_time.month =m41t00_bcd_to_byte(time.month);
    decoded_time.year =m41t00_bcd_to_byte(time.year);
	
    //twiname->MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}


uint8_t classI2C::m41t00_bcd_to_byte(uint8_t bcd_val)
{
	return (bcd_val&0xf) + ((bcd_val&0xf0) >> 4)*10;
}

uint8_t classI2C::m41t00_byte_to_bcd(uint8_t byte_val)
{
	return (byte_val%10) + ((byte_val/10) << 4);
}

void classI2C::time_date_read(void)
{
	twi_read_rtc(&TWIE);
}

// default destructor
classI2C::~classI2C()
{
} //~classI2C
