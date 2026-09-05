/* 
* classMnu.cpp
*
* Created: 06/04/2015 16:07:30
* Author: Vaio
*/


#include "classMnu.h"

// default constructor
classMnu::classMnu()
{
} //classMnu

// default destructor
classMnu::~classMnu()
{
} //~classMnu



void classMnu::Display_time(void)
{
	utft.int_to_bcd(m41t00.decoded_time.sec);
	unidades_s = utft.digitos.unidades;
	decenas_s = utft.digitos.decenas;
	utft.int_to_bcd(m41t00.decoded_time.min);
	unidades_m = utft.digitos.unidades;
	decenas_m = utft.digitos.decenas;
	utft.int_to_bcd(m41t00.decoded_time.hour);
	unidades_h = utft.digitos.unidades;
	decenas_h = utft.digitos.decenas;

	utft.setColor(VGA_WHITE);
	utft.setBackColor(VGA_BLUE);
	utft.setFont(font8x12);
	char txt;
	switch(m41t00.decoded_time.day_of_week)
	{
		case 1:
		utft.print("DOMINGO",124,137);
		break;
		case 2:
		utft.print("LUNES",124,37);
		break;
		case 3:
		utft.print("MARTES",124,37);
		break;
		case 4:
		utft.print("MIERCOLES",124,37);
		break;
		case 5:
		utft.print("JUEVES",124,37);
		break;
		case 6:
		utft.print("VIERNES",124,37);
		break;
		case 7:
		utft.print("SABADO",124,37);
		break;
		default:
		utft.print("UNKNOWN  ",15,60);
		break;		
	}
	utft.setFont(font8x12);
	utft.printChar(decenas_h,47,37);
	utft.printChar(unidades_h,56,37);
	
	utft.setFont(font8x12);
	utft.printChar(decenas_m,68,37);
	utft.printChar(unidades_m,78,37);
}


void classMnu::Display_temp(int16_t temp)
{

	utft.setColor(VGA_YELLOW);
	utft.setBackColor(VGA_BLACK);
	utft.setFont(font16x16);
	float ftemp = (float)temp;
	//utft.print("Temperatura:",20,240);
	//utft.printNumF(ftemp,175,74,270,'.',0);   // esquina inferior
	utft.printNumF(ftemp,1,126,57,'.',0);  // esquina superior
	//utft.print("oC",150,270);

}






