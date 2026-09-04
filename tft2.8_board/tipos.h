
#ifndef TIPOS_H_
#define TIPOS_H_

/* INPUT PIN DEFINITION******************************************************/
#define TA          PIN0_bm           //PORTA
#define TB          PIN1_bm           //PORTA
#define ANA2        PIN2_bm           //PORTA
#define DIO4        PIN4_bm           //PORTA
#define DIO5        PIN5_bm           //PORTA
#define DIO6        PIN6_bm           //PORTA
#define DIO7        PIN7_bm           //PORTA

/**************************************************************************
* INPUT PINS DEFINITIONS
***************************************************************************/
//#define TP_PEN                     !(PORTF.IN & PIN3_bm)    //PINF
#define PULSADOR_OK                !(PORTA.IN & PIN7_bm)    //PINA

/* OUTPUT PIN DEFINITION******************************************************/

#define LED_bm      PIN0_bm           //PORTD
#define DE          PIN1_bm           //PORTD
#define OUT0        PIN0_bm           //PORTB
#define OUT1        PIN1_bm           //PORTB
#define OUT2        PIN3_bm           //PORTB
#define DAC         PIN2_bm           //PORTB
#define BOMBA       OUT0


#define ADC_REF     2.05F
#define ADC_OFFSET  170
#define ADC_TOP     4096



typedef struct FLAGS
{
	volatile bool datos_listos;                    //comando recibido por RS 485
	volatile bool leer_AD7843;
	
}FLAGS;
extern FLAGS flags;// __attribute__((__section__(".noinit")));

typedef struct CONTADORES
{
	volatile unsigned int led_blinking;            //parpadeo del led
	volatile unsigned int timebase_sg;             //base de tiempo segundos
	volatile unsigned int segundos;               //segundos que transcurren durante el calentamiento
	volatile unsigned int time_set_delay_counter;
}CONTADORES;
extern CONTADORES contadores;// __attribute__((__section__(".noinit"))) ;



#endif /* TIPOS_H_ */