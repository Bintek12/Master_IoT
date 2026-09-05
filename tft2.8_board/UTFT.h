/*
  UTFT.h - Arduino/chipKit library support for Color TFT LCD Boards
  Copyright (C)2010-2014 Henning Karlsen. All right reserved
  
  This library is the continuation of my ITDB02_Graph, ITDB02_Graph16
  and RGB_GLCD libraries for Arduino and chipKit. As the number of 
  supported display modules and controllers started to increase I felt 
  it was time to make a single, universal library as it will be much 
  easier to maintain in the future.

  Basic functionality of this library was origianlly based on the 
  demo-code provided by ITead studio (for the ITDB02 modules) and 
  NKC Electronics (for the RGB GLCD module/shield).

  This library supports a number of 8bit, 16bit and serial graphic 
  displays, and will work with both Arduino and chipKit boards. For a 
  full list of tested display modules and controllers, see the 
  document UTFT_Supported_display_modules_&_controllers.pdf.

  When using 8bit and 16bit display modules there are some 
  requirements you must adhere to. These requirements can be found 
  in the document UTFT_Requirements.pdf.
  There are no special requirements when using serial displays.

  You can always find the latest version of the library at 
  http://electronics.henningkarlsen.com/

  If you make any modifications or improvements to the code, I would 
  appreciate that you share the code with me so that I might include 
  it in the next release. I can be contacted through 
  http://electronics.henningkarlsen.com/contact.php.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/

#ifndef UTFT_h
#define UTFT_h

#include "classSPI.h"
#include "classW25Qflash.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/portpins.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define F_CPU (32000000UL)
#include <util/delay.h>
#include "fonts.h"

//#define OUT0        PIN0_bm           //PORTB

//#define SET_OUT0     PORTB.OUTSET = OUT0;
//#define CLR_OUT0     PORTB.OUTCLR = OUT0;


#define UTFT_VERSION	279

#define LEFT 0
#define RIGHT 9999
#define CENTER 9998

#define PORTRAIT 0
#define LANDSCAPE 1

#define HX8347A			0
#define ILI9327			1
#define SSD1289			2
#define ILI9325C		3
#define ILI9325D_8		4
#define ILI9325D_16		5
#define HX8340B_8		6
#define HX8340B_S		7
#define HX8352A			8
#define ST7735			9
#define PCF8833			10
#define S1D19122		11
#define SSD1963_480		12
#define SSD1963_800		13
#define S6D1121_8		14
#define S6D1121_16		15
#define	SSD1289LATCHED	16
//#define NOT_IN_USE	17
//#define NOT_IN_USE	18
#define SSD1289_8		19
#define	SSD1963_800ALT	20
#define ILI9481			21
#define ILI9325D_16ALT	22
#define S6D0164			23
#define ST7735S			24
#define ILI9341_S5P		25
#define ILI9341_S4P		26
#define R61581			27
#define ILI9486			28
#define CPLD			29
#define HX8353C			30
#define ST7735_ALT		31

#define ITDB32			0	// HX8347-A (16bit)
#define ITDB32WC		1	// ILI9327  (16bit)
#define TFT01_32W		1	// ILI9327	(16bit)
#define ITDB32S			2	// SSD1289  (16bit)
#define TFT01_32		2	// SSD1289  (16bit)
#define CTE32			2	// SSD1289  (16bit)
#define ITDB24			3	// ILI9325C (8bit)
#define ITDB24D			4	// ILI9325D (8bit)
#define ITDB24DWOT		4	// ILI9325D (8bit)
#define ITDB28			4	// ILI9325D (8bit)
#define TFT01_24_8		4	// ILI9325D (8bit)
#define DMTFT24104      4   // ILI9325D (8bit)
#define DMTFT28103      4   // ILI9325D (8bit)
#define TFT01_24_16		5	// ILI9325D (16bit)
#define ITDB22			6	// HX8340-B (8bit)
#define ITDB22SP		7	// HX8340-B (Serial 4Pin)
#define ITDB32WD		8	// HX8352-A (16bit)
#define TFT01_32WD		8	// HX8352-A	(16bit)
#define CTE32W			8	// HX8352-A	(16bit)
#define ITDB18SP		9	// ST7735   (Serial 5Pin)
#define LPH9135			10	// PCF8833	(Serial 5Pin)
#define ITDB25H			11	// S1D19122	(16bit)
#define ITDB43			12	// SSD1963	(16bit) 480x272
#define TFT01_43		12	// SSD1963	(16bit) 480x272
#define ITDB50			13	// SSD1963	(16bit) 800x480
#define TFT01_50		13	// SSD1963	(16bit) 800x480
#define CTE50			13	// SSD1963	(16bit) 800x480
#define EHOUSE50		13	// SSD1963	(16bit) 800x480
#define ITDB24E_8		14	// S6D1121	(8bit)
#define TFT01_24R2		14	// S6D1121	(8bit)
#define ITDB24E_16		15	// S6D1121	(16bit)
#define INFINIT32		16	// SSD1289	(Latched 16bit) -- Legacy, will be removed later
#define ELEE32_REVA		16	// SSD1289	(Latched 16bit)
//#define NOT_IN_USE	17	
//#define NOT_IN_USE	18	
#define ELEE32_REVB		19	// SSD1289	(8bit)
#define TFT01_70		20	// SSD1963	(16bit) 800x480 Alternative Init
#define CTE70			20	// SSD1963	(16bit) 800x480 Alternative Init
#define EHOUSE70		20	// SSD1963	(16bit) 800x480 Alternative Init
#define CTE32HR			21	// ILI9481	(16bit)
#define CTE28			22	// ILI9325D (16bit) Alternative Init
#define TFT01_28		22	// ILI9325D (16bit) Alternative Init
#define CTE22			23	// S6D0164	(8bit)
#define TFT01_22		23	// S6D0164	(8bit)
#define DMTFT22102      23  // S6D0164  (8bit)
#define TFT01_18SP		24	// ST7735S  (Serial 5Pin)
#define TFT01_22SP		25	// ILI9341	(Serial 5Pin)
#define TFT01_24SP		25	// ILI9341	(Serial 5Pin)
#define TFT22SHLD		25	// ILI9341	(Serial 5Pin)
#define DMTFT28105      25  // ILI9341  (Serial 5Pin)
#define MI0283QT9		26  // ILI9341	(Serial 4Pin)
#define CTE35IPS		27	// R61581	(16bit)
#define CTE40			28	// ILI9486	(16bit)
#define EHOUSE50CPLD	29	// CPLD		(16bit)
#define CTE50CPLD		29	// CPLD		(16bit)
#define CTE70CPLD		29	// CPLD		(16bit)
#define DMTFT18101      30  // HX8353C  (Serial 5Pin)
#define TFT18SHLD		31	// ST7735	(Serial 5Pin) Alternative Init

#define SERIAL_4PIN		4
#define SERIAL_5PIN		5
#define LATCHED_16		17

#define NOTINUSE		255

//*********************************
// COLORS
//*********************************
// VGA color palette
#define VGA_BLACK		0x0000
#define VGA_WHITE		0xFFFF
#define VGA_RED			0xF800
#define VGA_GREEN		0x0400
#define VGA_BLUE		0x001F
#define VGA_SILVER		0xC618
#define VGA_GRAY		0x8410
#define VGA_MAROON		0x8000
#define VGA_YELLOW		0xFFE0
#define VGA_OLIVE		0x8400
#define VGA_LIME		0x07E0
#define VGA_AQUA		0x07FF
#define VGA_TEAL		0x0410
#define VGA_NAVY		0x0010
#define VGA_FUCHSIA		0xF81F
#define VGA_PURPLE		0x8010
#define VGA_TRANSPARENT	0xFFFFFFFF

#define FONT_SPACE 6
#define FONT_X 8
#define FONT_Y 8

#ifndef INT8U
#define INT8U unsigned char
#endif
#ifndef INT16U
#define INT16U unsigned int
#endif

#define LCD_RST     PIN2_bm           //PORTD
#define LCD_CS      PIN3_bm           //PORTC
#define LCD_RS      PIN2_bm           //PORTC
#define LCD_BKL     PIN1_bm           //PORTC

#define LCD_SCL     PIN7_bm           //PORTC
#define LCD_SDI     PIN5_bm           //PORTC
#define LCD_SDO     PIN6_bm           //PORTC
//===================================================
// Definicion de macros

#define TFT_CS_LOW   PORTC.OUTCLR = LCD_CS;
#define TFT_CS_HIGH  PORTC.OUTSET = LCD_CS;
#define TFT_DC_LOW   PORTC.OUTCLR = LCD_RS;
#define TFT_DC_HIGH  PORTC.OUTSET = LCD_RS;
#define TFT_BKL_ON   PORTC.OUTSET = LCD_BKL;
#define TFT_BKL_OFF  PORTC.OUTCLR = LCD_BKL;
#define TFT_RST_OFF  PORTD.OUTSET = LCD_RST;
#define TFT_RST_ON   PORTD.OUTCLR = LCD_RST;

typedef enum
{
	font8x12,
	font16x16,
	font7seg
	
	}FONTYPE;

struct _current_font
{
	uint8_t* font;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t offset;
	uint8_t numchars;
};


typedef struct bcd
{
	unsigned char dec_miles;
	unsigned char miles;
	unsigned char centenas;
	unsigned char decenas;
	unsigned char unidades;
}bcd;

extern classSPI SPIc;
extern classW25Qflash wflash;


class UTFT
{
	public:
		UTFT();
		void	InitLCD(uint8_t orientation=PORTRAIT);
		void	clrScr();
		void	drawPixel(int x, int y);
		void    drawPixel(int x, int y,uint16_t color);
		void	drawLine(int x1, int y1, int x2, int y2);
		void	fillScr(uint8_t r, uint8_t g, uint8_t b);
		void	fillScr(uint16_t color);
		void	drawRect(int x1, int y1, int x2, int y2);
		void	drawRoundRect(int x1, int y1, int x2, int y2);
		void	fillRect(int x1, int y1, int x2, int y2);
		void	fillRoundRect(int x1, int y1, int x2, int y2);
		void	drawCircle(int x, int y, int radius);
		void	fillCircle(int x, int y, int radius);
		void	setColor(uint8_t r, uint8_t g, uint8_t b);
		void	setColor(uint16_t color);
		uint16_t	getColor();
		void	setBackColor(uint8_t r, uint8_t g, uint8_t b);
		void	setBackColor(uint16_t color);
		uint16_t	getBackColor();
		void	print(const char *st, uint16_t x, uint16_t y, int deg=0);//char *st
		void	print(uint8_t st, int x, int y, int deg=0);//String st
		void	printNumI(long num, int x, int y, int length=0, char filler=' ');
		void	printNumF(double num, uint8_t dec, int x, int y, char divider='.', int length=0, char filler=' ');
		void    drawChar( INT8U ascii, INT16U poX, INT16U poY,INT16U size, INT16U fgcolor);
		void    drawString(char *string,INT16U poX, INT16U poY, INT16U size,INT16U fgcolor);
		void    drawBitmap(int x, int y, int sx, int sy,uint32_t p, int scale);
		void	setFont(FONTYPE fontype);
		uint8_t* getFont();
		uint8_t	getFontXsize();
		uint8_t	getFontYsize();
		void int_to_bcd(unsigned int value);
		//void	drawBitmap(int x, int y, int sx, int sy, uint8_t data[], int scale=1);
		//void	drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy);
		void	lcdOff();
		void	lcdOn();
		void	setContrast(char c);
		int		getDisplayXSize();
		int		getDisplayYSize();
		void	setBrightness(uint8_t br);
		void	setDisplayPage(uint8_t page);
		void	setWritePage(uint8_t page);
/*
	The functions and variables below should not normally be used.
	They have been left publicly available for use in add-on libraries
	that might need access to the lower level functions of UTFT.

	Please note that these functions and variables are not documented
	and I do not provide support on how to use them.
*/
		uint8_t			fch, fcl, bch, bcl;
		uint8_t			orient;
		uint16_t			disp_x_size, disp_y_size;
		FONTYPE	Fontype;
		_current_font cfont;
		bool			_transparent;
		bcd digitos;

		//void LCD_Writ_Bus(char VH,char VL, uint8_t mode);
		void LCD_Write_COM(char VL);
		void LCD_Write_DATA(char VH,char VL);
		void LCD_Write_DATA(char VL);
		void LCD_Write_DATAWORD(unsigned int y);
		void LCD_Write_COM_DATA(char com1,int dat1);
		void _hw_special_init();
		void setPixel(uint16_t color);
		void drawHLine(int x, int y, int l);
		void drawVLine(int x, int y, int l);
		void printChar(uint8_t c, int x, int y);
		void setXY(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
		//void setXY(int16_t x0, int16_t x1, int16_t y0, int16_t y1);
		void clrXY();
		void rotateChar(uint8_t c, int x, int y, int pos, int deg);
		void _set_direction_registers(uint8_t mode);
		void _convert_float(char *buf, double num, int width, uint8_t prec);
	private:
		int16_t absolute(int16_t);
		void swap_word(int16_t a,int16_t b);

};

#endif