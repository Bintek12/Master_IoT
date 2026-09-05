/*
 * main.h
 *
 * Created: 30/01/2015 10:35:43
 *  Author: Vaio
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include "tipos.h"
#include <avr/io.h>
#include <avr/portpins.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <math.h>
#define F_CPU (32000000UL)
#include <util/delay.h>
#include "classSPI.h"
#include "classClock.h"
#include "classPorts.h"
#include "classUsart.h"
#include "classTimers.h"
#include "UTFT.h"
#include "classAD7843.h"
#include "classW25Qflash.h"
#include "classI2C.h"
#include "classDAC.h"
#include "classADC.h"
#include "classXmegaRtc.h"
#include "classMnu.h"
#include "fixedP.h"
//#include "classSSD1306.h"




/******************************************************************************
**  Variables globales
******************************************************************************/

EEMEM uint8_t eeprom_writed;

EEMEM PROGRAMA prog_eep;
PROGRAMA prog_ram;

FLAGS flags;
CONTADORES contadores;

uint16_t page;
uint16_t RX_INDEX;
uint16_t x,y,ancho,alto;
uint32_t address;
uint16_t Ta,Tb;
float V;
int16_t setpoint;
bool ciclo;

classClock clock;
classSPI SPIc(&SPIC);
classPorts Ports;
classUsart Usart(&USARTE0);
classTimers Timers;
classADC adc;
classDAC dac;
classXmegaRtc Rtc;
UTFT utft;
classAD7843 AD7843;
classW25Qflash wflash;
classI2C m41t00;
classMnu Mnu;
//classSSD1306 Oled96;

//Prototipos
void interrupts_config(void);
void event_system_init(void);
void set_defaults(void);
void program_load(void);
void rs485_cmd_decode(uint8_t);
uint16_t leer_temperatura(uint8_t ntc);
float leer_voltaje(uint8_t channel);
void fillTriangle(uint8_t x,uint8_t y,uint8_t l,uint8_t h);
#endif /* MAIN_H_ */