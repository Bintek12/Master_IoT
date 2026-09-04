/* 
* classW25Qflash.h
*
* Created: 02/03/2015 15:12:19
* Author: Vaio
*/


#ifndef __CLASSW25QFLASH_H__
#define __CLASSW25QFLASH_H__

#include <inttypes.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <math.h>
#define F_CPU (32000000UL)
#include <util/delay.h>
#include "classSPI.h"

#define W25P_WriteEnable 0x06
#define W25P_WriteDisable 0x04
#define W25P_ReadStatusReg 0x05
#define W25P_WriteStatusReg 0x01
#define W25P_ReadData  0x03
#define W25P_FastReadData 0x0B
#define W25P_PageProgram 0x02
#define W25P_SectorErase 0xD8 // 64KB
#define W25P_ChipErase  0xC7
#define W25P_PowerDown  0xB9
#define W25P_ReleasePowerDown 0xAB
#define W25P_DeviceID  0xAB
#define W25P_ManufactDeviceID 0x90
#define R_JEDEC_ID	0x9f	//read JEDEC ID = Manuf+ID (suggested)

#define W_CS              PIN4_bm           //PORTC
#define W_WP              PIN0_bm           //PORTC
#define W_CLK             PIN7_bm           //PORTC
#define W_DI              PIN5_bm           //PORTC
#define W_DO              PIN6_bm           //PORTC

#define chip_select()     {PORTC.OUTCLR = W_CS;}
#define chip_deselect()   {PORTC.OUTSET = W_CS;}
#define chip_protect()    {PORTC.OUTCLR = W_WP;}
#define chip_unprotect()  {PORTC.OUTSET = W_WP;}

  enum partNumber 
  {
	  W25Q80 = 1,
	  W25Q16 = 2,
	  W25Q32 = 4,
	  W25Q64 = 8,
	  W25Q128 = 16
  };


typedef struct
{
	partNumber pn;       // Identificador del chip
	uint16_t id;         // Device ID
	uint32_t bytes;      // Tamaño total en bytes
	uint32_t pages;      // Número de páginas
	uint32_t sectors;    // Número de sectores
	uint32_t blocks;     // Número de bloques
} pnListType;

static const pnListType pnList[] PROGMEM =
{
	{ W25Q80,  0x4014,  1048576,   4096,   256,   16  },  // 1 MB
	{ W25Q16,  0x4015,  2097152,   8192,   512,   32  },  // 2 MB
	{ W25Q32,  0x4016,  4194304,  16384,  1024,   64  },  // 4 MB
	{ W25Q64,  0x4017,  8388608,  32768,  2048,  128  },  // 8 MB
	{ W25Q128, 0x4018, 16777216,  65536,  4096,  256  }   // 16 MB
};



extern classSPI SPIc;


class classW25Qflash
{
//variables
public:
    uint8_t tx_buff[2];
    uint8_t  temp_buffer[256];
protected:
private:

//functions
public:
	classW25Qflash();
	void IO_Send_Byte(uint8_t out);
	uint8_t IO_Get_Byte();
	void IO_Wait_Busy();
	uint8_t IO_Read_StatusReg();  //Reads the status register of the serial flash
	void IO_Write_StatusReg(uint8_t byte);//Performs a write to the status register
	void IO_Write_Enable();	//Write enables the serial flash
	void IO_PowerDown();
	void IO_ReleasePowerDown();
	uint8_t IO_Read_ID1();//Reads the device ID using the instruction 0xAB
	uint16_t IO_Read_ID2(uint8_t ID_Addr);//Reads the manufacturer ID and device ID with 0x90
	uint8_t IO_Read_Byte(unsigned long Dst_Addr);//Reads one byte from the serial flash and returns byte(max of 40 MHz CLK frequency)
	void IO_Read_nBytes(unsigned long Dst_Addr, uint8_t nBytes_128);//Reads multiple bytes(max of 40 MHz CLK frequency)
	uint16_t IO_Read_word(unsigned long Dst_Addr);
	uint8_t IO_FastRead_Byte(unsigned long Dst_Addr);//Reads one byte from the serial flash and returns byte(max of 80 MHz CLK frequency)
	void IO_FastRead_nBytes(unsigned long Dst_Addr, uint8_t nBytes_128);//Reads multiple bytes(max of 80 MHz CLK frequency)
	void IO_Write_Byte(unsigned long Dst_Addr, uint8_t byte);//Program one byte to the serial flash
	void IO_Write_nBytes(unsigned long Dst_Addr, uint16_t nBytes_128);//Program n bytes to the serial flash, n<=256
	void IO_Erase_Chip();//Erases entire serial flash
	void IO_Erase_Sector(unsigned long Dst_Addr);//Erases one sector (64 KB) of the serial flash
	void Verify(uint8_t byte, uint8_t cor_byte);
	void IO_Write_Disable();//Write disables the serial flash
	uint16_t IO_Read_ID3();
	~classW25Qflash();
protected:
private:
	classW25Qflash( const classW25Qflash &c );
	classW25Qflash& operator=( const classW25Qflash &c );

}; //classW25Qflash

#endif //__CLASSW25QFLASH_H__
