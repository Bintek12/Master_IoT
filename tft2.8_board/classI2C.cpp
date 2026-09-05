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
	uint8_t *writeData = (uint8_t*)&time;

	// Iniciar transacción: dirección del esclavo + bit de escritura (0)
	twiname->MASTER.ADDR = (RTC_SLAVE_ADDRESS << 1) | 0;

	// Esperar a que se complete la transmisión de la dirección
	while (!(twiname->MASTER.STATUS & TWI_MASTER_WIF_bm)) {
		if (twiname->MASTER.STATUS & TWI_MASTER_RXACK_bm) {
			// El esclavo no respondió ? abortar
			twiname->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
			return;
		}
	}

	// Enviar dirección inicial de registro (0x00)
	twiname->MASTER.DATA = 0x00;
	while (!(twiname->MASTER.STATUS & TWI_MASTER_WIF_bm));

	// Escribir los 8 bytes de la estructura time
	for (i = 0; i < 8; i++) {
		twiname->MASTER.DATA = *writeData++;
		while (!(twiname->MASTER.STATUS & TWI_MASTER_WIF_bm));
	}

	// STOP para finalizar la transacción
	twiname->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
}

// Formato: act(dia, mes, año, hora, minuto, segundo, diaSemana)
void classI2C::act(uint8_t dia, uint8_t mes, uint8_t anio,
uint8_t hora, uint8_t minuto, uint8_t segundo,
uint8_t diaSemana)
{
	uint8_t buffer[8];

	buffer[0] = decToBcd(segundo);
	buffer[1] = decToBcd(minuto);
	buffer[2] = decToBcd(hora);
	buffer[3] = decToBcd(diaSemana);
	buffer[4] = decToBcd(dia);
	buffer[5] = decToBcd(mes);
	buffer[6] = decToBcd(anio);
	buffer[7] = 0x00; // control sin flags

	// Iniciar transacción con dirección 0xD0 (escritura)
	twiname->MASTER.ADDR = RTC_SLAVE_ADDRESS; // ya definido como 0xD0
	while (!(twiname->MASTER.STATUS & TWI_MASTER_WIF_bm));

	// Dirección inicial de registro
	twiname->MASTER.DATA = 0x00;
	while (!(twiname->MASTER.STATUS & TWI_MASTER_WIF_bm));

	// Escribir los 8 bytes
	for (uint8_t i = 0; i < 8; i++) {
		twiname->MASTER.DATA = buffer[i];
		while (!(twiname->MASTER.STATUS & TWI_MASTER_WIF_bm));
	}

	// STOP
	twiname->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
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
	if(set_time)
	Time_adjust();

	twi_read_rtc(&TWIE);
	
}

void classI2C::Time_adjust(void)
{
	if(set_minutos && AD7843.TCIsPenOn())
	minute_adjust();
	
	if(set_hora && AD7843.TCIsPenOn())
	hour_adjust();
	
	if(set_dia && AD7843.TCIsPenOn())
	day_adjust();
	time_set_timer++;
	if(time_set_timer>100)
	{
		time_set_timer = 0;
		set_time = false;
	}
}

void  classI2C::minute_adjust()
{
	set_dia = false;
	set_hora = false;
	minutos = m41t00_bcd_to_byte(time.min);
	minutos++;
	if(minutos>59)
	minutos = 0;
	time.min = m41t00_byte_to_bcd(minutos);
	twi_write_rtc(&TWIE);
	time_set_delay_counter = 0;
	
}

void  classI2C::hour_adjust()
{
	set_minutos = false;
	set_dia = false;
	horas = m41t00_bcd_to_byte(time.hour);
	horas++;
	if(horas>23)
	horas = 0;
	time.hour = m41t00_byte_to_bcd(horas);
	twi_write_rtc(&TWIE);
	time_set_delay_counter = 0;
	
}


void  classI2C::day_adjust()
{
	set_hora = false;
	set_minutos = false;
	dia = m41t00_bcd_to_byte(time.day_of_week);
	dia++;
	if(dia>7)
	dia = 1;
	time.day_of_week = m41t00_byte_to_bcd(dia);
	twi_write_rtc(&TWIE);
	time_set_delay_counter = 0;
}


void classI2C::setTime_keys(void)
{
	if(AD7843.day_key())
	{
		set_dia = true;
		set_minutos = false;
		set_hora = false;
		//m41t00.set_time = true;
		time_set_delay_counter++;
		if(time_set_delay_counter>9000)
		set_time = true;
	}
	
	if(AD7843.hours_key())
	{
		set_dia = false;
		set_minutos = false;
		set_hora = true;
		//m41t00.set_time = true;
		time_set_delay_counter++;
		if(time_set_delay_counter>9000)
		set_time = true;
		
	}
	
	if(AD7843.minute_key())
	{
		set_dia = false;
		set_minutos = true;
		set_hora = false;
		//m41t00.set_time = true;
		time_set_delay_counter++;
		if(time_set_delay_counter>9000)
		set_time = true;
		
	}
	
}







// default destructor
classI2C::~classI2C()
{
} //~classI2C
