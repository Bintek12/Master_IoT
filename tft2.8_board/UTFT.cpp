/*
  UTFT.cpp - Arduino/chipKit library support for Color TFT LCD Boards
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

#include "UTFT.h"



UTFT::UTFT()
{
}



void UTFT::LCD_Write_COM(char cmd)  
{   
    TFT_DC_LOW;
    TFT_CS_LOW;
    SPIc.transfer(cmd);
    TFT_CS_HIGH;
}

void UTFT::LCD_Write_DATA(char VH,char VL)
{
    TFT_DC_HIGH;
    TFT_CS_LOW;
    SPIc.transfer(VH);
    SPIc.transfer(VL);
    TFT_CS_HIGH;
	
}

//==============================================================
//write  data word
void  UTFT::LCD_Write_DATAWORD(unsigned int y)
{
	unsigned char m,n;
	m=y>>8;
	n=y;
	LCD_Write_DATA(m,n);

}

void UTFT::LCD_Write_DATA(char data)
{
    TFT_DC_HIGH;
    TFT_CS_LOW;
    SPIc.transfer(data);
    TFT_CS_HIGH;
}

void UTFT::LCD_Write_COM_DATA(char com1,int dat1)
{
     LCD_Write_COM(com1);
     LCD_Write_DATA(dat1>>8,dat1);
}

void UTFT::InitLCD(uint8_t orientation)
{
	
	PORTC.DIRSET = LCD_CS | LCD_RS | LCD_BKL | LCD_SCL | LCD_SDI ;
	PORTC.DIRCLR = LCD_SDO;
	PORTC.OUTSET = LCD_CS | LCD_RS | LCD_BKL | LCD_SCL | LCD_SDI ;
    PORTD.DIRSET = LCD_RST;
	PORTD.OUTSET = LCD_RST;
	 
	orient=orientation;
	setColor(255, 255, 255);
	setBackColor(0, 0, 0);
	setFont(font16x16);
	_transparent = false;
	SPIc.spi_init();
	TFT_CS_HIGH;

	LCD_Write_COM(0x11);
	_delay_ms(120);

	LCD_Write_COM(0xCF);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0xc3);
	LCD_Write_DATA(0X30);

	LCD_Write_COM(0xED);
	LCD_Write_DATA(0x64);
	LCD_Write_DATA(0x03);
	LCD_Write_DATA(0X12);
	LCD_Write_DATA(0X81);

	LCD_Write_COM(0xE8);
	LCD_Write_DATA(0x85);
	LCD_Write_DATA(0x10);
	LCD_Write_DATA(0x79);

	LCD_Write_COM(0xCB);
    LCD_Write_DATA(0x39);
	LCD_Write_DATA(0x2C);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x34);
	LCD_Write_DATA(0x02);

	LCD_Write_COM(0xF7);
	LCD_Write_DATA(0x20);

	LCD_Write_COM(0xEA);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);

	LCD_Write_COM(0xC0);    //Power control
	LCD_Write_DATA(0x22);   //VRH[5:0]

    LCD_Write_COM(0xC1);    //Power control
	LCD_Write_DATA(0x11);   //SAP[2:0];BT[3:0]

	LCD_Write_COM(0xC5);    //VCM control
	LCD_Write_DATA(0x3d);
	//LCD_DataWrite_ILI9341(0x30);
	LCD_Write_DATA(0x20);

	LCD_Write_COM(0xC7);    //VCM control2
	//LCD_DataWrite_ILI9341(0xBD);
	LCD_Write_DATA(0xAA); //0xB0

	LCD_Write_COM(0x36);    // Memory Access Control
	LCD_Write_DATA(0x08);

	LCD_Write_COM(0x3A);
	LCD_Write_DATA(0x55);

	LCD_Write_COM(0xB1);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x13);

	LCD_Write_COM(0xB6);    // Display Function Control
	LCD_Write_DATA(0x0A);
	LCD_Write_DATA(0xA2);

	LCD_Write_COM(0xF6);
	LCD_Write_DATA(0x01);
	LCD_Write_DATA(0x30);

	LCD_Write_COM(0xF2);    // 3Gamma Function Disable
	LCD_Write_DATA(0x00);

	LCD_Write_COM(0x26);    //Gamma curve selected
	LCD_Write_DATA(0x01);

	LCD_Write_COM(0xE0);    //Set Gamma
	LCD_Write_DATA(0x0F);
	LCD_Write_DATA(0x3F);
	LCD_Write_DATA(0x2F);
	LCD_Write_DATA(0x0C);
	LCD_Write_DATA(0x10);
	LCD_Write_DATA(0x0A);
	LCD_Write_DATA(0x53);
	LCD_Write_DATA(0XD5);
	LCD_Write_DATA(0x40);
	LCD_Write_DATA(0x0A);
	LCD_Write_DATA(0x13);
	LCD_Write_DATA(0x03);
	LCD_Write_DATA(0x08);
	LCD_Write_DATA(0x03);
	LCD_Write_DATA(0x00);

	LCD_Write_COM(0XE1);    //Set Gamma
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x10);
	LCD_Write_DATA(0x03);
	LCD_Write_DATA(0x0F);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x2C);
	LCD_Write_DATA(0xA2);
	LCD_Write_DATA(0x3F);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x0E);
	LCD_Write_DATA(0x0C);
	LCD_Write_DATA(0x37);
	LCD_Write_DATA(0x3C);
	LCD_Write_DATA(0x0F);

	LCD_Write_COM(0x11);    //Exit Sleep
	_delay_ms(120);
	LCD_Write_COM(0x29);    //Display on
	_delay_ms(50);
    clrXY();
}

int16_t UTFT::absolute(int16_t value)
{
	if (value < 0) {
		return -value;
	}
	else {
		return value;
	}
}

void UTFT::swap_word(int16_t a,int16_t b)
{
	int16_t temp = a;
	a = b;
	b = temp;
}


void UTFT::setXY(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	if (orient==LANDSCAPE)
	{
		swap_word(x1, y1);
		swap_word(x2, y2);
		y1=disp_y_size-y1;
		y2=disp_y_size-y2;
		swap_word(y1, y2);
	}
	
	LCD_Write_COM(0x2a);
	LCD_Write_DATA(x1>>8);
	LCD_Write_DATA(x1);
	LCD_Write_DATA(x2>>8);
	LCD_Write_DATA(x2);
	LCD_Write_COM(0x2b);
	LCD_Write_DATA(y1>>8);
	LCD_Write_DATA(y1);
	LCD_Write_DATA(y2>>8);
	LCD_Write_DATA(y2);
	LCD_Write_COM(0x2c);

}

void UTFT::clrXY()
{
	if (orient==PORTRAIT)
		setXY(0,0,disp_x_size,disp_y_size);
	else
		setXY(0,0,disp_y_size,disp_x_size);
}

void UTFT::drawRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap_word(x1, x2);
	}
	if (y1>y2)
	{
		swap_word(y1, y2);
	}

	drawHLine(x1, y1, x2-x1);
	drawHLine(x1, y2, x2-x1);
	drawVLine(x1, y1, y2-y1);
	drawVLine(x2, y1, y2-y1);
}

void UTFT::drawRoundRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap_word(x1, x2);
	}
	if (y1>y2)
	{
		swap_word(y1, y2);
	}
	if ((x2-x1)>4 && (y2-y1)>4)
	{
		drawPixel(x1+1,y1+1);
		drawPixel(x2-1,y1+1);
		drawPixel(x1+1,y2-1);
		drawPixel(x2-1,y2-1);
		drawHLine(x1+2, y1, x2-x1-4);
		drawHLine(x1+2, y2, x2-x1-4);
		drawVLine(x1, y1+2, y2-y1-4);
		drawVLine(x2, y1+2, y2-y1-4);
	}
}

void UTFT::fillRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap_word(x1, x2);
	}
	if (y1>y2)
	{
		swap_word(y1, y2);
	}
	if (orient==PORTRAIT)
	{
		for (int i=0; i<((y2-y1)/2)+1; i++)
		{
			drawHLine(x1, y1+i, x2-x1);
			drawHLine(x1, y2-i, x2-x1);
		}
	}
	else
	{
		for (int i=0; i<((x2-x1)/2)+1; i++)
		{
			drawVLine(x1+i, y1, y2-y1);
			drawVLine(x2-i, y1, y2-y1);
		}
	}
}

void UTFT::fillRoundRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap_word(x1, x2);
	}
	if (y1>y2)
	{
		swap_word(y1, y2);
	}

	if ((x2-x1)>4 && (y2-y1)>4)
	{
		for (int i=0; i<((y2-y1)/2)+1; i++)
		{
			switch(i)
			{
			case 0:
				drawHLine(x1+2, y1+i, x2-x1-4);
				drawHLine(x1+2, y2-i, x2-x1-4);
				break;
			case 1:
				drawHLine(x1+1, y1+i, x2-x1-2);
				drawHLine(x1+1, y2-i, x2-x1-2);
				break;
			default:
				drawHLine(x1, y1+i, x2-x1);
				drawHLine(x1, y2-i, x2-x1);
			}
		}
	}
}

void UTFT::drawCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
 
	setXY(x, y + radius, x, y + radius);
	LCD_Write_DATA(fch,fcl);
	setXY(x, y - radius, x, y - radius);
	LCD_Write_DATA(fch,fcl);
	setXY(x + radius, y, x + radius, y);
	LCD_Write_DATA(fch,fcl);
	setXY(x - radius, y, x - radius, y);
	LCD_Write_DATA(fch,fcl);
 
	while(x1 < y1)
	{
		if(f >= 0) 
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;    
		setXY(x + x1, y + y1, x + x1, y + y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - x1, y + y1, x - x1, y + y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x + x1, y - y1, x + x1, y - y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - x1, y - y1, x - x1, y - y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x + y1, y + x1, x + y1, y + x1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - y1, y + x1, x - y1, y + x1);
		LCD_Write_DATA(fch,fcl);
		setXY(x + y1, y - x1, x + y1, y - x1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - y1, y - x1, x - y1, y - x1);
		LCD_Write_DATA(fch,fcl);
	}
	clrXY();
}

void UTFT::fillCircle(int x, int y, int radius)
{
	for(int y1=-radius; y1<=0; y1++) 
		for(int x1=-radius; x1<=0; x1++)
			if(x1*x1+y1*y1 <= radius*radius) 
			{
				drawHLine(x+x1, y+y1, 2*(-x1));
				drawHLine(x+x1, y-y1, 2*(-x1));
				break;
			}
}

void UTFT::clrScr()
{
    LCD_Write_COM(0x2c);                                                  /* start to write to display ra */
    TFT_DC_HIGH;
    TFT_CS_LOW;
    for(uint16_t i=0; i<38400; i++)
    {
	    SPIc.transfer(0);
	    SPIc.transfer(0);
	    SPIc.transfer(0);
	    SPIc.transfer(0);
    }
    TFT_CS_HIGH;
}

void UTFT::fillScr(uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t color = ((r&248)<<8 | (g&252)<<3 | (b&248)>>3);
	fillScr(color);
}

void UTFT::fillScr(uint16_t color)
{
	char ch, cl;
	unsigned i,j;
	ch=uint8_t(color>>8);
	cl=uint8_t(color & 0xFF);

	clrXY();
	
	TFT_DC_HIGH;
	TFT_CS_LOW;
	
	for (i=0;i<disp_y_size;i++)
	{
			
		for (j=0;j<disp_x_size;j++)
		{
		  SPIc.transfer(ch);
		  SPIc.transfer(cl);
		}
	}
	TFT_CS_HIGH;

}




void UTFT::setColor(uint8_t r, uint8_t g, uint8_t b)
{
	fch=((r&248)|g>>5);
	fcl=((g&28)<<3|b>>3);
}

void UTFT::setColor(uint16_t color)
{
	fch=uint8_t(color>>8);
	fcl=uint8_t(color & 0xFF);
}

uint16_t UTFT::getColor()
{
	return (fch<<8) | fcl;
}

void UTFT::setBackColor(uint8_t r, uint8_t g, uint8_t b)
{
	bch=((r&248)|g>>5);
	bcl=((g&28)<<3|b>>3);
	_transparent=false;
}

void UTFT::setBackColor(uint16_t color)
{
	if (color==VGA_TRANSPARENT)
		_transparent=true;
	else
	{
		bch=uint8_t(color>>8);
		bcl=uint8_t(color & 0xFF);
		_transparent=false;
	}
}

uint16_t UTFT::getBackColor()
{
	return (bch<<8) | bcl;
}

void UTFT::setPixel(uint16_t color)
{
	LCD_Write_DATA((color>>8),(color&0xFF));	// rrrrrggggggbbbbb
}

void UTFT::drawPixel(int x, int y)
{
	setXY(x, y, x, y);
	setPixel((fch<<8)|fcl);
	clrXY();
}

void UTFT::drawPixel(int x, int y,uint16_t color)
{
	setXY(x, y, x, y);
	setPixel(color);
	clrXY();
}

void UTFT::drawLine(int x1, int y1, int x2, int y2)
{
	if (y1==y2)
		drawHLine(x1, y1, x2-x1);
	else if (x1==x2)
		drawVLine(x1, y1, y2-y1);
	else
	{
		unsigned int	dx = (x2 > x1 ? x2 - x1 : x1 - x2);
		int16_t			xstep =  x2 > x1 ? 1 : -1;
		unsigned int	dy = (y2 > y1 ? y2 - y1 : y1 - y2);
		int16_t			ystep =  y2 > y1 ? 1 : -1;
		int				col = x1, row = y1;

		if (dx < dy)
		{
			int t = - (dy >> 1);
			while (true)
			{
				setXY (col, row, col, row);
				LCD_Write_DATA (fch, fcl);
				if (row == y2)
					return;
				row += ystep;
				t += dx;
				if (t >= 0)
				{
					col += xstep;
					t   -= dy;
				}
			} 
		}
		else
		{
			int t = - (dx >> 1);
			while (true)
			{
				setXY (col, row, col, row);
				LCD_Write_DATA (fch, fcl);
				if (col == x2)
					return;
				col += xstep;
				t += dy;
				if (t >= 0)
				{
					row += ystep;
					t   -= dx;
				}
			} 
		}
	}
	clrXY();
}

void UTFT::drawHLine(int x, int y, int l)
{
	if (l<0)
	{
		l = -l;
		x -= l;
	}
	setXY(x, y, x+l, y);
	{
		for (int i=0; i<l+1; i++)
		{
			LCD_Write_DATA(fch, fcl);
		}
	}
	//sbi(P_CS, B_CS);
	clrXY();
}

void UTFT::drawVLine(int x, int y, int l)
{
	if (l<0)
	{
		l = -l;
		y -= l;
	}
	//cbi(P_CS, B_CS);
	setXY(x, y, x, y+l);
	{
		for (int i=0; i<l+1; i++)
		{
			LCD_Write_DATA(fch, fcl);
		}
	}
	//sbi(P_CS, B_CS);
	clrXY();
}


void UTFT::drawChar( INT8U ascii, INT16U poX, INT16U poY,INT16U size, INT16U fgcolor)
{
	if((ascii>=32)&&(ascii<=127))
	{
		;
	}
	else
	{
		ascii = '?'-32;
	}
	for (int i =0; i<FONT_X; i++ ) {
		INT8U temp = pgm_read_byte(&simpleFont[ascii-0x20][i]);
		for(INT8U f=0;f<8;f++)
		{
			if((temp>>f)&0x01)
			{
				//fillRectangle(poX+i*size, poY+f*size, size, size, fgcolor);
				fillRect(poX+i*size, poY+f*size, size + (poX+i*size), size + (poY+f*size));
				
			}

		}

	}
}

void UTFT::drawString(char *string,INT16U poX, INT16U poY, INT16U size,INT16U fgcolor)
{
	while(*string)
	{
		drawChar(*string, poX, poY, size, fgcolor);
		*string++;

		if(poX < disp_x_size)
		{
			poX += FONT_SPACE*size;                                     /* Move cursor right            */
		}
	}
}


void UTFT::printChar(uint8_t c, int x, int y)
{
	uint8_t i,ch;
	unsigned j;
	uint16_t temp; 

  
	if (!_transparent)
	{
		if (orient==PORTRAIT)
		{
			setXY(x,y,x+cfont.x_size-1,y+cfont.y_size-1);
			temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
			for(j=0;j<((cfont.x_size/8)*cfont.y_size);j++)
			{
				ch=pgm_read_byte(&cfont.font[temp]);//&cfont.font[temp]
				for(i=0;i<8;i++)
				{  
					if((ch&(1<<(7-i)))!=0)   
					{
						setPixel((fch<<8)|fcl);
					} 
					else
					{
						setPixel((bch<<8)|bcl);
					} 
				}
				temp++;
			}
		}
		else
		{
			temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;

			for(j=0;j<((cfont.x_size/8)*cfont.y_size);j+=(cfont.x_size/8))
			{
				setXY(x,y+(j/(cfont.x_size/8)),x+cfont.x_size-1,y+(j/(cfont.x_size/8)));
				for (int zz=(cfont.x_size/8)-1; zz>=0; zz--)
				{
					ch=pgm_read_byte(&cfont.font[temp+zz]);//&cfont.font[temp+zz]
					for(i=0;i<8;i++)
					{   
						if((ch&(1<<i))!=0)   
						{
							setPixel((fch<<8)|fcl);
						} 
						else
						{
							setPixel((bch<<8)|bcl);
						}   
					}
				}
				temp+=(cfont.x_size/8);
			}
		}
	}
	else
	{
		temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
		for(j=0;j<cfont.y_size;j++) 
		{
			for (int zz=0; zz<(cfont.x_size/8); zz++)
			{
				ch=pgm_read_byte(&cfont.font[temp+zz]); 
				for(i=0;i<8;i++)
				{   
					setXY(x+i+(zz*8),y+j,x+i+(zz*8)+1,y+j+1);
				
					if((ch&(1<<(7-i)))!=0)   
					{
						setPixel((fch<<8)|fcl);
					} 
				}
			}
			temp+=(cfont.x_size/8);
		}
	}

	//clrXY();
}

void UTFT::rotateChar(uint8_t c, int x, int y, int pos, int deg)
{
	uint8_t i,j,ch;
	uint16_t temp; 
	int newx,newy;
	double radian;
	radian=deg*0.0175;  

	//cbi(P_CS, B_CS);

	temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
	for(j=0;j<cfont.y_size;j++) 
	{
		for (int zz=0; zz<(cfont.x_size/8); zz++)
		{
			ch=pgm_read_byte(&cfont.font[temp+zz]); 
			for(i=0;i<8;i++)
			{   
				newx=x+(((i+(zz*8)+(pos*cfont.x_size))*cos(radian))-((j)*sin(radian)));
				newy=y+(((j)*cos(radian))+((i+(zz*8)+(pos*cfont.x_size))*sin(radian)));

				setXY(newx,newy,newx+1,newy+1);
				
				if((ch&(1<<(7-i)))!=0)   
				{
					setPixel((fch<<8)|fcl);
				} 
				else  
				{
					if (!_transparent)
						setPixel((bch<<8)|bcl);
				}   
			}
		}
		temp+=(cfont.x_size/8);
	}
	//sbi(P_CS, B_CS);
	clrXY();
}

void UTFT::print(const char *st, uint16_t x, uint16_t y, int deg)
{
	int stl, i;
	stl = strlen(st);
	if (orient==PORTRAIT)
	{
	if (x==RIGHT)
		x=(disp_x_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_x_size+1)-(stl*cfont.x_size))/2;
	}
	else
	{
	if (x==RIGHT)
		x=(disp_y_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_y_size+1)-(stl*cfont.x_size))/2;
	}

	for (i=0; i<stl; i++)
		if (deg==0)
			printChar(*st++, x + (i*(cfont.x_size)), y);
		else
			rotateChar(*st++, x, y, i, deg);
}

void UTFT::print(uint8_t st, int x, int y, int deg)//string st
{
	char buf[sizeof(st)+1];

	//st.toCharArray(buf, st.length()+1);
	print(buf, x, y, deg);
}

void UTFT::printNumI(long num, int x, int y, int length, char filler)
{
	char buf[25];
	char st[27];
	bool neg=false;
	int c=0, f=0;
  
	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-1); c++)
				st[c]=filler;
			st[c]=48;
			st[c+1]=0;
		}
		else
		{
			st[0]=48;
			st[1]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=true;
			num=-num;
		}
	  
		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;
	  
		if (neg)
		{
			st[0]=45;
		}
	  
		if (length>(c+neg))
		{
			for (int i=0; i<(length-c-neg); i++)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		for (int i=0; i<c; i++)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	print(st,x,y);
}

void UTFT::printNumF(double num, uint8_t dec, int x, int y, char divider, int length, char filler)
{
	char st[27];
	bool neg=false;

	if (dec<1)
		dec=1;
	else if (dec>5)
		dec=5;

	if (num<0)
		neg = true;

	_convert_float(st, num, length, dec);

	if (divider != '.')
	{
		for (int i=0; i<sizeof(st); i++)
			if (st[i]=='.')
				st[i]=divider;
	}

	if (filler != ' ')
	{
		if (neg)
		{
			st[0]='-';
			for (int i=1; i<sizeof(st); i++)
				if ((st[i]==' ') || (st[i]=='-'))
					st[i]=filler;
		}
		else
		{
			for (int i=0; i<sizeof(st); i++)
				if (st[i]==' ')
					st[i]=filler;
		}
	}

	print(st,x,y);
}

void UTFT::_convert_float(char *buf, double num, int width, uint8_t prec)
{
	dtostrf(num, width, prec, buf);
}


void UTFT::setFont(FONTYPE fontype)
{
	switch(fontype)
	{
		case font8x12:
		cfont.font =(unsigned char*) SmallFont;
	    cfont.x_size=pgm_read_byte(&SmallFont[0]);
	    cfont.y_size=pgm_read_byte(&SmallFont[1]);
	    cfont.offset=pgm_read_byte(&SmallFont[2]);
	    cfont.numchars=pgm_read_byte(&SmallFont[3]);
		break;
		case font16x16:
		cfont.font =(unsigned char*) BigFont;
		cfont.x_size=pgm_read_byte(&BigFont[0]);
		cfont.y_size=pgm_read_byte(&BigFont[1]);
		cfont.offset=pgm_read_byte(&BigFont[2]);
		cfont.numchars=pgm_read_byte(&BigFont[3]);
		break;
		case font7seg:
		cfont.font = (unsigned char*) SevenSegNumFont;
		cfont.x_size=pgm_read_byte(&SevenSegNumFont[0]);
		cfont.y_size=pgm_read_byte(&SevenSegNumFont[1]);
		cfont.offset=pgm_read_byte(&SevenSegNumFont[2]);
		cfont.numchars=pgm_read_byte(&SevenSegNumFont[3]);
		break;
		
	}
}

uint8_t* UTFT::getFont()
{
	return cfont.font;
}

uint8_t UTFT::getFontXsize()
{
	return cfont.x_size;
}

uint8_t UTFT::getFontYsize()
{
	return cfont.y_size;
}



//Representa un bitmap almacenado en la flash externa 25Q128
//x coordenada izquierda del bitmap
//y coordenada superior del bitmap
//sx ancho del bitmap
//sy alto del bitmap
//p apunta al inicio de datos den la flash externa
void UTFT::drawBitmap(int x, int y, int sx, int sy,uint32_t p, int scale)
{
	unsigned int col;
	int tx, ty, tc, tsx, tsy;
    uint32_t pointer=0;
	if (scale==1)
	{
		for (ty=0; ty<sy; ty++) // espejo vertical
		 //for (ty=sy-1; ty>=0; ty--)
		{
			setXY(x, y+ty, x+sx, y+ty);
			for (tx=sx-1; tx>=0; tx--)//espejo for (tx=0; tx<sx; tx++)
			//for (tx=0; tx<sx; tx++)
			{
				//col=wflash.IO_Read_word(p + (2*ty*sx)+2*tx);
				col=wflash.IO_Read_word(p + pointer);//p +pointer
				LCD_Write_DATA(col>>8,col & 0xff);
				pointer +=2;
			}
		}
	}
	else
	{
		for (ty=0; ty<sy; ty++)
		{
			for (tsy=0; tsy<scale; tsy++)
			{
				setXY(x, y+(ty*scale)+tsy, x+((sx*scale)-1), y+(ty*scale)+tsy);
				for (tx=0; tx<sx; tx++)
				{
					//col=wflash.IO_Read_word(p + (ty*sx)+tx);
					col=wflash.IO_Read_word(p + pointer);//p +pointer
					for (tsx=0; tsx<scale; tsx++)
					LCD_Write_DATA(col>>8,col & 0xff);
					pointer +=2;
				}
			}
		}
	}
	clrXY();
}


/*

void UTFT::drawBitmap(int x, int y, int sx, int sy, uint32_t p, int scale)
{
	unsigned int col;
	//int tx, ty, tc, tsx, tsy;
    int32_t tx, ty, tc, tsx, tsy;
	if (scale==1)
	{
		if (orient==PORTRAIT)
		{
			setXY(x, y, x+sx-1, y+sy-1);
			for (tc=0; tc<(sx*sy); tc++)
			{
				col=wflash.IO_Read_word(p+2*tc);
				LCD_Write_DATA(col>>8,col & 0xff);
			}
		}
		else
		{
			for (ty=0; ty<sy; ty++)
			{
				setXY(x, y+ty, x+sx-1, y+ty);
				for (tx=sx-1; tx>=0; tx--)
				{
					col=wflash.IO_Read_word(p+(2*ty*sx)+2*tx);
					LCD_Write_DATA(col>>8,col & 0xff);
				}
			}
		}
	}
	else
	{
		if (orient==PORTRAIT)
		{
			for (ty=0; ty<sy; ty++)
			{
				setXY(x, y+(ty*scale), x+((sx*scale)-1), y+(ty*scale)+scale);
				for (tsy=0; tsy<scale; tsy++)
				for (tx=0; tx<sx; tx++)
				{
					col=wflash.IO_Read_word(p+(2*ty*sx)+2*tx);
					for (tsx=0; tsx<scale; tsx++)
					LCD_Write_DATA(col>>8,col & 0xff);
				}
			}
		}
		else
		{
			for (ty=0; ty<sy; ty++)
			{
				for (tsy=0; tsy<scale; tsy++)
				{
					setXY(x, y+(ty*scale)+tsy, x+((sx*scale)-1), y+(ty*scale)+tsy);
					for (tx=sx-1; tx>=0; tx--)
					{
						col=wflash.IO_Read_word(p+(2*ty*sx)+2*tx);
						for (tsx=0; tsx<scale; tsx++)
						LCD_Write_DATA(col>>8,col & 0xff);
					}
				}
			}
		}
	}
	clrXY();
}

*/


/*

void UTFT::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy)
{
	unsigned int col;
	int tx, ty, newx, newy;
	double radian;
	radian=deg*0.0175;  

	if (deg==0)
		drawBitmap(x, y, sx, sy, data);
	else
	{
		//cbi(P_CS, B_CS);
		for (ty=0; ty<sy; ty++)
			for (tx=0; tx<sx; tx++)
			{
				col=pgm_read_word(&data[(ty*sx)+tx]);

				newx=x+rox+(((tx-rox)*cos(radian))-((ty-roy)*sin(radian)));
				newy=y+roy+(((ty-roy)*cos(radian))+((tx-rox)*sin(radian)));

				setXY(newx, newy, newx, newy);
				LCD_Write_DATA(col>>8,col & 0xff);
			}
		//sbi(P_CS, B_CS);
	}
	clrXY();
}

*/








void UTFT::lcdOff()
{
}

void UTFT::lcdOn()
{
}

void UTFT::setContrast(char c)
{
}

int UTFT::getDisplayXSize()
{
	if (orient==PORTRAIT)
		return disp_x_size+1;
	else
		return disp_y_size+1;
}

int UTFT::getDisplayYSize()
{
	if (orient==PORTRAIT)
		return disp_y_size+1;
	else
		return disp_x_size+1;
}

void UTFT::setBrightness(uint8_t br)
{
}

void UTFT::setDisplayPage(uint8_t page)
{
	if (page>7) page=7;
	LCD_Write_COM_DATA(0x04,page);
	LCD_Write_COM(0x0F);   
}

void UTFT::setWritePage(uint8_t page)
{
	if (page>7) page=7;
	LCD_Write_COM_DATA(0x05,page);
	LCD_Write_COM(0x0F);   
}

 //Conversión a bcd para visualización

 void UTFT::int_to_bcd(unsigned int value)
 {
	 unsigned int resto;
	 digitos.dec_miles = 48 +  value / 10000;
	 resto = value % 10000;
	 digitos.miles = 48 + resto / 1000;
	 resto=resto % 1000;
	 digitos.centenas = 48 + resto / 100;
	 resto=resto % 100;
	 digitos.decenas = 48 + resto / 10;
	 resto=resto % 10;
	 digitos.unidades = 48 + resto;
 }
 
 
 
 
 

 