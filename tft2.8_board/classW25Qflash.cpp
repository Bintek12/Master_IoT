/* 
* classW25Qflash.cpp
*
* Created: 02/03/2015 15:12:18
* Author: Vaio
*/


#include "classW25Qflash.h"

// default constructor
classW25Qflash::classW25Qflash()
{
	PORTC.DIRSET = W_CS | W_WP | W_DI | W_CLK;
	PORTC.DIRCLR = W_DO;
	PORTC.OUTSET = W_CS | W_WP | W_DI | W_CLK;

} //classW25Qflash

// default destructor
classW25Qflash::~classW25Qflash()
{
} //~classW25Qflash



void classW25Qflash::IO_Send_Byte(uint8_t byte)
{
	chip_select();
	SPIc.transfer(byte);
}



uint8_t classW25Qflash::IO_Get_Byte()
{
	uint8_t im; 
	chip_select();
	im = SPIc.transfer(0x00);
	return im;
}

void classW25Qflash::IO_Wait_Busy()
{
	/*  waste time until not busy WEL & Busy bit all be 1 (0x03). */
	while (IO_Read_StatusReg() == 0x03)
	IO_Read_StatusReg();
}



uint8_t classW25Qflash::IO_Read_StatusReg()
{
	uint8_t byte = 0;
	chip_select();   /* enable device */
	IO_Send_Byte(W25P_ReadStatusReg); /* send Read Status Register command */
	byte = IO_Get_Byte();   /* receive byte */
	chip_deselect();    /* disable device */
	return byte;
}




void classW25Qflash::IO_Write_StatusReg(uint8_t byte)
{
	chip_select();    /* enable device */
	IO_Send_Byte(W25P_WriteStatusReg); /* select write to status register */
	IO_Send_Byte(byte); /* data that will change the status(only bits 2,3,7 can be written) */
	chip_deselect();    /* disable the device */
}

void classW25Qflash::IO_Write_Enable()
{ 
	chip_unprotect();
	chip_select();   /* enable device */
	IO_Send_Byte(W25P_WriteEnable); /* send W25P_Write_Enable command */
	chip_deselect();   /* disable device */
}




void classW25Qflash::IO_PowerDown()
{
	chip_select();    /* enable device */
	IO_Send_Byte(W25P_PowerDown);  /* send W25P_PowerDown command 0xB9 */
	chip_deselect();    /* disable device */
	_delay_us(3);    /* remain CS high for tPD = 3uS */
}




void classW25Qflash::IO_ReleasePowerDown()
{
	chip_select();   /* enable device */
	IO_Send_Byte(W25P_ReleasePowerDown); /* send W25P_PowerDown command 0xAB */
	chip_deselect();    /* disable device */
	_delay_us(3);    /* remain CS high for tPD = 3uS */
}


uint8_t classW25Qflash::IO_Read_ID1()
{
	uint8_t byte;
	chip_select();  /* enable device */
	IO_Send_Byte(W25P_DeviceID); /* send read device ID command (ABh) */
	IO_Send_Byte(0);  /* send address */
	IO_Send_Byte(0);  /* send address */
	IO_Send_Byte(0);  /* send 3_Dummy address */
	byte = IO_Get_Byte();  /* receive Device ID byte */
	chip_deselect();   /* disable device */
	_delay_us(2);   /* remain CS high for tRES2 = 1.8uS */
	return byte;
}




uint16_t classW25Qflash::IO_Read_ID2(uint8_t ID_Addr)
{
	uint16_t IData16;
	chip_select();   /* enable device */
	IO_Send_Byte(W25P_ManufactDeviceID); /* send read ID command (90h) */
	IO_Send_Byte(0x00);   /* send address */
	IO_Send_Byte(0x00);   /* send address */
	IO_Send_Byte(ID_Addr);   /* send W25Pxx selectable ID address 00H or 01H */
	IData16 = IO_Get_Byte()<<8;  /* receive Manufature or Device ID byte */
	IData16 |= IO_Get_Byte();  /* receive Device or Manufacture ID byte */
	chip_deselect();    /* disable device */
	return IData16;
}


uint16_t classW25Qflash::IO_Read_ID3()
{
	uint16_t IData16;
	chip_select();   /* enable device */
	IO_Send_Byte(R_JEDEC_ID); /* send read ID command (9Fh) */
	IData16 = IO_Get_Byte()<<8;  /* receive Manufature or Device ID byte */
	IData16 |= IO_Get_Byte();  /* receive Device or Manufacture ID byte */
	tx_buff[2] = IO_Get_Byte();
	chip_deselect();    /* disable device */
	return IData16;
}




uint8_t classW25Qflash::IO_Read_Byte(unsigned long Dst_Addr)
{
	uint8_t byte = 0;
	chip_select();     /* enable device */
	IO_Send_Byte(W25P_ReadData);    /* read command */
	IO_Send_Byte(((Dst_Addr & 0xFFFFFF) >> 16)); /* send 3 address bytes */
	IO_Send_Byte(((Dst_Addr & 0xFFFF) >> 8));
	IO_Send_Byte(Dst_Addr & 0xFF);
	byte = IO_Get_Byte();
	chip_deselect();   /* disable device */
	return byte;   /* return one byte read */
}

void classW25Qflash::IO_Read_nBytes(unsigned long Dst_Addr, uint8_t nBytes_128)
{
	unsigned long i = 0;
	
	chip_select();     /* enable device */
	IO_Send_Byte(W25P_ReadData);    /* read command */
	IO_Send_Byte(((Dst_Addr & 0xFFFFFF) >> 16));  /* send 3 address bytes */
	IO_Send_Byte(((Dst_Addr & 0xFFFF) >> 8));
	IO_Send_Byte(Dst_Addr & 0xFF);
	for (i = 0; i < nBytes_128; i++)  /* read until no_bytes is reached */
	{
		temp_buffer[i] = IO_Get_Byte(); /* receive byte and store at address 80H - FFH */
	}
	
	chip_deselect();    /* disable device */
	
}




uint8_t classW25Qflash::IO_FastRead_Byte(unsigned long Dst_Addr)
{
	uint8_t byte = 0;
	chip_select();     /* enable device */
	IO_Send_Byte(W25P_FastReadData);   /* fast read command */
	IO_Send_Byte(((Dst_Addr & 0xFFFFFF) >> 16)); /* send 3 address bytes */
	IO_Send_Byte(((Dst_Addr & 0xFFFF) >> 8));
	IO_Send_Byte(Dst_Addr & 0xFF);
	IO_Send_Byte(0xFF);  /*dummy byte*/
	byte = IO_Get_Byte();
	chip_deselect();   /* disable device */
	
	return byte;   /* return one byte read */
}


void classW25Qflash::IO_FastRead_nBytes(uint32_t Dst_Addr, uint8_t nBytes_128)
{
	uint8_t i = 0;
	
	chip_select();     /* enable device */
	IO_Send_Byte(W25P_FastReadData);   /* read command */
	IO_Send_Byte(((Dst_Addr & 0xFFFFFF) >> 16));  /* send 3 address bytes */
	IO_Send_Byte(((Dst_Addr & 0xFFFF) >> 8));
	IO_Send_Byte(Dst_Addr & 0xFF);
	IO_Send_Byte(0xFF);   /*dummy byte*/
	for (i = 0; i < nBytes_128; i++) /* read until no_bytes is reached */
	{
		temp_buffer[i] = IO_Get_Byte(); /* receive byte and store at address 80H - FFH */
	}
	chip_deselect();    /* disable device */

}


void classW25Qflash::IO_Write_Byte(uint32_t Dst_Addr, uint8_t byte)
{
	chip_select();    /* enable device */
	IO_Write_Enable();    /* set WEL */
	IO_Wait_Busy();
	
	chip_select();
	IO_Send_Byte(W25P_PageProgram);   /* send Byte Program command */
	IO_Send_Byte(((Dst_Addr & 0xFFFFFF) >> 16)); /* send 3 address bytes */
	IO_Send_Byte(((Dst_Addr & 0xFFFF) >> 8));
	IO_Send_Byte(Dst_Addr & 0xFF);
	IO_Send_Byte(byte);   /* send byte to be programmed */
	chip_deselect();    /* disable device */
}




void classW25Qflash::IO_Write_nBytes(uint32_t Dst_Addr, uint16_t nBytes_128)
{
	uint16_t i;
	uint8_t byte;
	chip_select();     /* enable device */
	IO_Write_Enable();    /* set WEL */
	chip_select();
	IO_Send_Byte(W25P_PageProgram);   /* send Byte Program command */
	IO_Send_Byte(((Dst_Addr & 0xFFFFFF) >> 16)); /* send 3 address bytes */
	IO_Send_Byte(((Dst_Addr & 0xFFFF) >> 8));
	IO_Send_Byte(Dst_Addr & 0xFF);
	
	for (i = 0; i < nBytes_128; i++)
	{
		byte = temp_buffer[i];
		IO_Send_Byte(byte);  /* send byte to be programmed */
	}
	chip_deselect();   /* disable device */
	
	//printf("\nPage program (%d nBytes)! please waiting....\n");
}




void classW25Qflash::IO_Erase_Chip()
{
	chip_select();    /* enable device */
	IO_Write_Enable();    /* set WEL */
	chip_select();
	IO_Wait_Busy();
	chip_select();
	IO_Send_Byte(W25P_ChipErase);  /* send Chip Erase command */
	chip_deselect();   /* disable device */
	IO_Wait_Busy();
}


void classW25Qflash::IO_Erase_Sector(uint32_t Dst_Addr)
{
	chip_select();     /* enable device */
	IO_Write_Enable();    /* set WEL */
	chip_select();
	IO_Send_Byte(W25P_SectorErase);   /* send Sector Erase command */
	IO_Send_Byte(((Dst_Addr & 0xFFFFFF) >> 16));  /* send 3 address bytes */
	IO_Send_Byte(((Dst_Addr & 0xFFFF) >> 8));
	IO_Send_Byte(Dst_Addr & 0xFF);
	chip_deselect();     /* disable device */
	IO_Wait_Busy();
}




void classW25Qflash::Verify(uint8_t byte, uint8_t cor_byte)
{
	if (byte != cor_byte)
	{
		while(1);
		//LED_Error = 0; /* display to view error on LED. */
	}
}




void classW25Qflash::IO_Write_Disable()
{
	chip_protect();
	chip_select();    /* enable device */
	IO_Send_Byte(W25P_WriteDisable); /* send W25P_Write_Disable command */
	chip_deselect();    /* disable device */
}

uint16_t classW25Qflash::IO_Read_word(unsigned long Dst_Addr)
{
	//unsigned long i = 0;
	uint8_t bl, bh;
	chip_select();     /* enable device */
	IO_Send_Byte(W25P_ReadData);    /* read command */
	IO_Send_Byte(((Dst_Addr & 0xFFFFFF) >> 16));  /* send 3 address bytes */
	IO_Send_Byte(((Dst_Addr & 0xFFFF) >> 8));
	IO_Send_Byte(Dst_Addr & 0xFF);
	bl = IO_Get_Byte(); /* receive byte and store at address 80H - FFH */
	bh = IO_Get_Byte(); 
	chip_deselect();    /* disable device */
	return bh << 8 | bl;
}









