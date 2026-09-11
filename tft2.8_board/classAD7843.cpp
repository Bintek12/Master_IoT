/* 
* classAD7843.cpp
*
* Created: 06/02/2015 21:05:21
* Author: Vaio
*/


#include "classAD7843.h"

// default constructor
classAD7843::classAD7843()
{
} //classAD7843


void classAD7843::TCInit(void)
{
	
    PORTD.DIRSET = TP_CS | TP_SDI | TP_CLK;
	PORTD.DIRCLR = TP_PEN | TP_SDO;
    PORTD.OUTSET = TP_CS ;
	PORTD.PIN3CTRL=PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	TP_CS_HIGH();
	TP_CLK_LOW();
	TP_SDI_LOW();
}


// ads_rd():
// A bit-banged implementation of the SPI access for AD7843...
// Slow and steady gets the job done!
//

int16_t   classAD7843::ad7843_read(uint8_t ads_ctl)
{
	uint16_t bit, val;

	TP_CLK_LOW();
	TP_CS_LOW();

	for(bit = 0x80;bit != 0;bit >>= 1) {
		if (ads_ctl & bit)
		{
			TP_SDI_HIGH();
		}
		else
		{
			TP_SDI_LOW();
		
		}

		TP_CLK_HIGH();
		_delay_us(1);
		TP_CLK_LOW();
		_delay_us(1);//clk a 500 kHz
	}

	val = 0;
	for(bit = 0x8000;bit != 0;bit >>= 1)
	 {
		TP_CLK_HIGH();
		if (getTP_SDO())
		val |= bit;
		_delay_us(1);
		TP_CLK_LOW();
		_delay_us(1);
	}

	TP_CLK_LOW();
	_delay_us(1);
	TP_CS_HIGH();
	return(val);
}








unsigned int classAD7843::TCGetX(void) //call this after TCRead() to get X value
{
	uint16_t temp;
	temp = ad7843_read(0xD2);
	return temp /(AD7843_MAX_XVAL / PIXELS_X);
}

unsigned int classAD7843::TCGetY(void) //call this after TCRead() to get Y value
{
	uint16_t temp;
	temp = ad7843_read(0x92);
	return temp /(AD7843_MAX_YVAL / PIXELS_Y);
}

unsigned char classAD7843::TCIsPenOn(void) //TC_PEN_PIN is 0 when the screen is pressed
{
	return ((PORTD.IN & TP_PEN)>0) ? 0 : 1;
}

			

bool classAD7843::day_key(void)
{
	return (TCGetY()>0 && TCGetY()<50) ? true:false;
}

bool classAD7843::Boton_agua(void)
{
	if(TCGetX()>0 && TCGetX()<240)
	  if(TCGetY()>10 && TCGetY()<71)
	    return true;
	return false;	
}

bool classAD7843::Boton_energia(void)
{
	if(TCGetX()>0 && TCGetX()<240)
		if(TCGetY()>85 && TCGetY()<141)
			 return true;
	return false;
}
bool classAD7843::Boton_menu(void)
{
	if(TCGetX()>0 && TCGetX()<20)
		if(TCGetY()>310 && TCGetY()<320)
			return true;
	return false;
}
bool classAD7843::Boton_reloj(void)
{
	if(TCGetX()>0 && TCGetX()<240)
		if(TCGetY()>160 && TCGetY()<310)
			return true;
	return false;
}

void classAD7843::drawKeys(void)
{
	utft.clrScr();
	drawButton("1",10,50,60,60);
	drawButton("2",90,50,60,60);
	drawButton("3",170,50,60,60);
	drawButton("4",10,120,60,60);
	drawButton("5",90,120,60,60);
	drawButton("6",170,120,60,60);
	drawButton("7",10,190,60,60);
	drawButton("8",90,190,60,60);
	drawButton("9",170,190,60,60);
	drawButton("Del",10,270,60,60);
	drawButton("0",90,270,60,60);
	drawButton("Ok",170,270,60,60);
	utft.drawRect(1,1,239,319);

}

void classAD7843::drawButton(const char * string,uint16_t x, uint16_t y,uint8_t ancho, uint8_t alto)
{
	utft.fillRect(x,y,x+ancho,y+alto);
	utft.print(string,x,y,0);
	
}

uint8_t classAD7843::presetKey(void)
{
	if(TCGetX()<100)
		return coll1_decode();
	if(TCGetX()> 100 && TCGetX()<180)
		return coll2_decode();
	if(TCGetX()> 180)
		return coll3_decode();
	 return 0; // valor por defecto si no se cumple ningún caso
}

uint8_t classAD7843::coll1_decode(void)
{
	if(TCGetY()<100)
		return 1;
	 
	if(TCGetY()>100 && TCGetY()< 180)
		return 4;
	 
	if(TCGetY()>180 && TCGetY()< 250)
		return 7;

	if(TCGetY()>250)
		return 10;
	return 0; // valor por defecto si no se cumple ningún caso 
}

uint8_t classAD7843::coll2_decode(void)
{
	if(TCGetY()<100)
	return 2;
	
	if(TCGetY()>100 && TCGetY()< 180)
	return 5;
	
	if(TCGetY()>180 && TCGetY()< 250)
	return 8;
	
	if(TCGetY()>250)
	 return 0;
   return 0; // valor por defecto si no se cumple ningún caso 
}

uint8_t classAD7843::coll3_decode(void)
{
	if(TCGetY()<100)
		return 3;
	
	if(TCGetY()>100 && TCGetY()< 180)
		return 6;
	
	if(TCGetY()>180 && TCGetY()< 250)
		return 9;

	if(TCGetY()>250)
		return 12;
	return 0; // valor por defecto si no se cumple ningún caso
}



// default destructor
classAD7843::~classAD7843()
{
} //~classAD7843
