/*
 * tft2_8_board.cpp
 *
 * Created: 02/06/2026 11:28:22
 *  Author: Vaio
 * Modificado: Bruno Gonzalez Rojas
 * Envio de imagenes a memoria FLASH W25Q128
 * Lecturas del encoder
 * lecturas de las NTC
 * Envio Digital
 * Envio Analogico
 * Lectura del Displsy resistivo
 * envios RS485
 */ 


#include <avr/io.h>
#include "main.h"
/**********************************************************************************
 * usart0 rx interrupt service routine
 * Recibe un comando de 4 uint8_ts desde la red rs485
 **********************************************************************************/
ISR (USARTE0_RXC_vect)
{
 unsigned char status, sr;
 sr = SREG;
 status = USARTE0.STATUS; //requerido por el hardware (see Atmel datasheet)
 Usart.rx_buffer[Usart.rx_index] = USARTE0.DATA ;
 Usart.rx_index++;

 //TCD1.PER=0xF9FF;//4 ms
 TCD1.INTFLAGS = TCD1.INTFLAGS;//flag clear
 TCD1.INTCTRLA = (TCD1.INTCTRLA & (~(TC1_ERRINTLVL_gm | TC1_OVFINTLVL_gm))) |
 TC_ERRINTLVL_OFF_gc | TC_OVFINTLVL_LO_gc;
 TCD1.CTRLA = 0x02;//clock/2
 TCD1.CNT = 0;
 SREG = sr;
}

/*****************************************************************
* Se ejecuta 4 milisegundos despues del ultimo caracter recibido por Uart
*****************************************************************/
ISR(TCD1_OVF_vect)
{
	 unsigned char sr;
	 sr = SREG;
	 RX_INDEX=Usart.rx_index;
	Usart.rx_index = 0;
	TCD1.INTCTRLA = 0;
	if(Usart.rx_buffer[0] == 1)//dirRs485
	{
		flags.datos_listos = true;
	}
	SREG = sr;

}

/*
*****************************************************************
* PORTC interrupt 0 service routine
* Interrupcion del Touch Pannel
* Ocurre cada vez que se pulsa la pantalla.
****************************************************************/
ISR(PORTC_INT0_vect)
{
	contadores.segundos = 0;
	//flags.leer_AD7843 = true;
}



/*****************************************************************
* Base de tiempo de 10 ms.
* Llamada a los menus
******************************************************************/
 ISR(RTC_OVF_vect)
 {
	unsigned char sr;
	sr = SREG;
	contadores.led_blinking++;
	contadores.timebase_sg++;
	SREG = sr;
 }
 
 /*****************************************************************
 * Frecuencia de la señal analogica de salida
 * Usado por la clase classGenerator
 *****************************************************************/
 ISR(TCE0_OVF_vect)
 {
	//generator.Generate(); 
	PORTB.OUTTGL = OUT2;
 }
 

//DMA Chennel Interrupt
ISR(DMA_CH0_vect)
{
	DMA.CH0.CTRLB|=0x10;
	DMA.CH0.CTRLA|=0x10;
	DMA.CH0.CTRLA|=0x80;
}



int main(void)
{
	clock.system_clocks_init();  //inicializacion del reloj externo 32Mhz del Atxmega32Ua4
	Ports.ports_init();
	SPIc.spi_init();
	Usart.usart0_init();
	Timers.tcd0_init();
	Timers.tcd1_init();          //comunicacion rs485 (perro guardian)
	Timers.tce0_init(1000);
	adc.adca_init();
	dac.dacb_init();
	Rtc.rtc_init();              //inicializacion del RTC
	interrupts_config();
    if(eeprom_read_byte(&eeprom_writed) != 1)
	   set_defaults();
	program_load();  
	utft.disp_x_size = 240;
	utft.disp_y_size = 320;
	utft.InitLCD(PORTRAIT);
	utft.setColor(VGA_WHITE);
	utft.setBackColor(VGA_BLACK);
    utft.setFont(font16x16);
	utft.clrScr();
	m41t00.twi_init(&TWIE,32000000L,100000L);
    m41t00.show_time = true;
	AD7843.TCInit();
	//utft.setColor(VGA_WHITE);
	//utft.fillRoundRect(0,0,240,170);
	//utft.setColor(VGA_BLUE);
	//utft.fillRoundRect(0,60,240,180);
    //utft.drawBitmap(0,0,240,50,0,1); //BinteK.raw
	utft.drawBitmap(0,0,240,320,180224,1); //Solar1.raw
	page = 0;
	m41t00.show_time = true;
	 while(1) {
        if(contadores.led_blinking >14){
	      contadores.led_blinking = 0;
	      PORTD.OUTTGL= LED_bm;		  
        } 
		/*if(PULSADOR_OK)		{
			while(PULSADOR_OK);
			//Encoder.push = true;
			//Mnu.set_time = true;
			//Mnu.set_time_secuence++;
		}*/
		if(m41t00.show_time){
			m41t00.time_date_read();
			Mnu.Display_time();
			T1 = leer_temperatura(0);
			Mnu.Display_temp(T1);
		}
		if(contadores.timebase_sg>=50) {
			contadores.timebase_sg = 0;
			
			//utft.printNumF(T1,2,177,92,'.',3,' ');
			//Timer.cada_segundo = true;
		}	
		
		if(flags.datos_listos){
			flags.datos_listos = false;
			rs485_cmd_decode(Usart.rx_buffer[1]);
		}
		
		///Hid.Loop();
		//Menu.Loop();
		
	}
}


void interrupts_config(void){
	// Low level interrupt: On
	// Round-robin scheduling for low level interrupt: On
	// Medium level interrupt: On
	// High level interrupt: On
	// The interrupt vectors will be placed at the start of the Application FLASH section
	unsigned char n;
	n=(PMIC.CTRL & (~(PMIC_RREN_bm | PMIC_IVSEL_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm))) |
	PMIC_LOLVLEN_bm | PMIC_RREN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
	CCP=CCP_IOREG_gc;
	PMIC.CTRL=n;
	// Set the default priority for round-robin scheduling
	PMIC.INTPRI=0x00;
	asm("sei");
}


void rs485_cmd_decode(uint8_t cmd){
 switch(cmd){
	case 1: {//external flash prog
		address = (((uint32_t)(Usart.rx_buffer[2])<<16)) |((uint16_t) (Usart.rx_buffer[3])<<8) | Usart.rx_buffer[4];
		page = ((uint16_t)(Usart.rx_buffer[5])<<8) | Usart.rx_buffer[6];	
		for(int i=0;i<256;i++)
			wflash.temp_buffer[i] = Usart.rx_buffer[i+7];	
		uint32_t target_addr = address + ((uint32_t)page << 8);
		if (RX_INDEX >= 7 + 256) {
			char texto[] = "Page";
			utft.print(texto, 20, 190, 0);
			//utft.print("Page",20,190,0);
			utft.int_to_bcd(page+1);
			utft.printChar(utft.digitos.centenas,100,190);
			utft.printChar(utft.digitos.decenas,120,190);
			utft.printChar(utft.digitos.unidades,140,190);
			// Escribir página completa (256 bytes)
			wflash.IO_Write_nBytes(target_addr, 256);		
		}	
		break;
	}
	case 2:{//external flash erase
		wflash.IO_Erase_Chip();
		wflash.IO_Erase_Chip();
		break;
	}
	case 4: // external flash erase
	{
		//uint8_t erase_type = Usart.rx_buffer[2]; // tipo de borrado
		uint32_t addr = (((uint32_t)(Usart.rx_buffer[2])<<16)) |
		((uint16_t)(Usart.rx_buffer[3])<<8) |
		Usart.rx_buffer[4];
		wflash.IO_Erase_Sector(addr);
		break;
	}
	case 3:{
		address = (((uint32_t)(Usart.rx_buffer[2])<<16)) |((uint16_t) (Usart.rx_buffer[3])<<8) | Usart.rx_buffer[4];
		x = ((uint16_t)(Usart.rx_buffer[5])<<8) | Usart.rx_buffer[6];
		y = ((uint16_t)(Usart.rx_buffer[7])<<8) | Usart.rx_buffer[8];
		ancho = ((uint16_t)(Usart.rx_buffer[9])<<8) | Usart.rx_buffer[10];
		alto = ((uint16_t)(Usart.rx_buffer[11])<<8) | Usart.rx_buffer[12];
		utft.drawBitmap(x,y,ancho,alto,address,1);
		break;
	}
	case 5:
	    m41t00.time.sec  = decToBcd(Usart.rx_buffer[2]);
		m41t00.time.min  = decToBcd(Usart.rx_buffer[3]);
		m41t00.time.hour    = decToBcd(Usart.rx_buffer[4]);
		m41t00.time.day_of_week  = decToBcd(Usart.rx_buffer[5]);
		m41t00.time.day_of_month = decToBcd(Usart.rx_buffer[6]);
		m41t00.time.month    = decToBcd(Usart.rx_buffer[7]);
		m41t00.time.year     = decToBcd(Usart.rx_buffer[8]);// 2026 ? 26
		//classI2C.time.control  = 0x00;          // sin flags

	m41t00.twi_write_rtc(&TWIC);  // ejemplo usando TWIC como bus I2C

	break;
	case 6:
	break;
	case 10:{
		PORTB.OUTSET = OUT0;
		break;
	}
	case 11:{
		PORTB.OUTCLR = OUT0;
		break;
	}
	case 12:{
		PORTB.OUTSET = OUT1;
		break;
	}
	case 13:{
		PORTB.OUTCLR = OUT1;
		break;
	}
	case 14:{
		PORTB.OUTSET = OUT2;
		break;
	}
	case 15:{
		PORTB.OUTCLR = OUT2;
		break;
	}
	case 30:{
	    //page=(Usart.rx_buffer[3] << 8) | Usart.rx_buffer[2];
		//utft.print("Salida 3: 0",20,210,0);
		//utft.int_to_bcd(page);
		//utft.printChar(utft.digitos.centenas,100,210);
		//utft.printChar(utft.digitos.decenas,120,210);
		//utft.printChar(utft.digitos.unidades,140,210);
		//dac.dacb0_write((Usart.rx_buffer[3] << 8) | Usart.rx_buffer[2]);		
	break;
	}
	}
}

void set_defaults(void){
	/*
	eeprom_write_byte(&prog_eep.periodo,120);
	eeprom_write_byte(&prog_eep.pulse,20);
	eeprom_write_byte(&prog_eep.dia_inicio,1);
	eeprom_write_byte(&prog_eep.dia_fin,5);
	eeprom_write_byte(&prog_eep.hora_inicio,8);
	eeprom_write_byte(&prog_eep.hora_fin,18);
	eeprom_write_byte(&eeprom_writed,1);
	*/
}
void readParams_from_eep(void)
{  /*
	Timer.Event.dia_inicio = eeprom_read_byte(&Event_eep.dia_inicio);
	Timer.Event.hora_inicio = eeprom_read_byte(&Event_eep.hora_inicio);
	Timer.Event.minuto_inicio = eeprom_read_byte(&Event_eep.minuto_inicio);
	Timer.Event.segundo_inicio = eeprom_read_byte(&Event_eep.segundo_inicio);
	Timer.Event.dia_fin = eeprom_read_byte(&Event_eep.dia_fin);
	Timer.Event.hora_fin = eeprom_read_byte(&Event_eep.hora_fin);
	Timer.Event.minuto_fin = eeprom_read_byte(&Event_eep.minuto_fin);
	Timer.Event.segundo_fin = eeprom_read_byte(&Event_eep.segundo_fin);
	Timer.Event.enable = (bool) eeprom_read_byte((uint8_t*)&Event_eep.enable);
	*/
}


void program_load(void){
	/*
	prog_ram.periodo = eeprom_read_byte(&prog_eep.periodo);
	prog_ram.dia_inicio = eeprom_read_byte(&prog_eep.dia_inicio);
	prog_ram.hora_fin = eeprom_read_byte(&prog_eep.dia_fin);
	prog_ram.hora_inicio = eeprom_read_byte(&prog_eep.hora_inicio);
	prog_ram.hora_fin = eeprom_read_byte(&prog_eep.hora_fin);
	prog_ram.pulse = eeprom_read_byte(&prog_eep.pulse);
*/
}

//devuelve la temperatura como un entero x 10ºC
uint16_t leer_temperatura(uint8_t ntc){
	Tb = (adc.adca_read(ntc) - ADC_OFFSET);
	float v = Tb * ADC_REF / 4096.0;
	v = (-0.509 * v * v + 41.46 * v - 1.52) * 10.0;
	return round(v);	
}

void rs485_tx(){
	int i;
	//unsigned int cksum = 0;
	Usart.tx_buffer[0] = 1;
	Usart.tx_buffer[1] = x;
	Usart.tx_buffer[2] = y;
	Usart.tx_buffer[3] = ancho;
	Usart.tx_buffer[4] = alto;

	PORTE.OUTSET = DE; //transmit enable
	_delay_us(10);
	for (i=0;i<TX_BUFFER_SIZE;i++)	{
		// Wait for empty transmit buffer /
		while ( !(USARTE0.STATUS & (1<<USART_DREIF_bp )));
		// Put data into buffer, sends the data /
		USARTE0.DATA = Usart.tx_buffer[i];
	}
	//espera a que se transmitan todos los bits
	while ( !( USARTE0.STATUS & (1<<USART_TXCIF_bp ) ));
	USARTE0.STATUS |= (1<<USART_TXCIF_bp ); //flag clear
	PORTE.OUTCLR = DE;                   // pone modo escucha, en red rs485	
} // end rs485_tx()


