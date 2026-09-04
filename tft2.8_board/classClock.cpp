/* 
* classClock.cpp
*
* Created: 04/02/2015 11:36:07
* Author: Vaio
*/


#include "classClock.h"

// default constructor
classClock::classClock()
{
} //classClock


void classClock::system_clocks_init(void)
{
	uint8_t s = SREG;   // guardar estado de interrupciones
	asm("cli");         // desactivar interrupciones globales

	// Habilitar oscilador interno de 32 MHz
	OSC.CTRL |= OSC_RC32MEN_bm;

	// Desactivar autocalibración DFLL
	DFLLRC32M.CTRL &= ~DFLL_ENABLE_bm;

	// Esperar estabilización del oscilador
	while ((OSC.STATUS & OSC_RC32MRDY_bm) == 0);

	// Configurar prescalers A=1, B=1, C=1
	CCP = CCP_IOREG_gc;             // handshake para desbloquear
	CLK.PSCTRL = CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc;

	// Seleccionar fuente de reloj = RC32M
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;

	// Desactivar osciladores no usados
	OSC.CTRL &= ~(OSC_RC2MEN_bm | OSC_RC32KEN_bm | OSC_XOSCEN_bm | OSC_PLLEN_bm);

	// Bloquear registros de reloj
	CCP = CCP_IOREG_gc;
	CLK.LOCK = CLK_LOCK_bm;

	// Desactivar salida de reloj a pines
	PORTCFG.CLKEVOUT = PORTCFG_CLKOUT_OFF_gc;

	// Restaurar estado de interrupciones
	SREG = s;
}


// default destructor
classClock::~classClock()
{
} //~classClock
