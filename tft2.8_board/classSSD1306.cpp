/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/

#include "classSSD1306.h"

classSSD1306::classSSD1306() {
    this->clear();
}

void classSSD1306::OledInit(void) 
{
	//i2c.init(SSD1306_DEFAULT_ADDRESS);

	// Turn display off
	sendCommand(SSD1306_DISPLAYOFF);

	sendCommand(SSD1306_SETDISPLAYCLOCKDIV);
	sendCommand(0x80);

	sendCommand(SSD1306_SETMULTIPLEX);
	sendCommand(0x3F);
	
	sendCommand(SSD1306_SETDISPLAYOFFSET);
	sendCommand(0x00);
	
	sendCommand(SSD1306_SETSTARTLINE | 0x00);
	
	// We use internal charge pump
	sendCommand(SSD1306_CHARGEPUMP);
	sendCommand(0x14);
	
	// Horizontal memory mode
	sendCommand(SSD1306_MEMORYMODE);
	sendCommand(0x00);
	
	sendCommand(SSD1306_SEGREMAP | 0x1);

	sendCommand(SSD1306_COMSCANDEC);

	sendCommand(SSD1306_SETCOMPINS);
	sendCommand(0x12);

	// Max contrast
	sendCommand(SSD1306_SETCONTRAST);
	sendCommand(0xCF);

	sendCommand(SSD1306_SETPRECHARGE);
	sendCommand(0xF1);

	sendCommand(SSD1306_SETVCOMDETECT);
	sendCommand(0x40);

	sendCommand(SSD1306_DISPLAYALLON_RESUME);

	// Non-inverted display
	sendCommand(SSD1306_NORMALDISPLAY);

	// Turn display back on
	sendCommand(SSD1306_DISPLAYON);
}



void classSSD1306::sendCommand(uint8_t command)
{
	//twiname->MASTER.CTRLC = 0;
	TWIE.MASTER.CTRLC = 0;
	TWIE.MASTER.ADDR = 0x78;  // write to Display
	while(!(TWIE.MASTER.STATUS&TWI_MASTER_WIF_bm));
	TWIE.MASTER.DATA = 0x00;       // write word addr
	while(!(TWIE.MASTER.STATUS&TWI_MASTER_WIF_bm));
	//twiname->MASTER.DATA = 0x00;
	//while(!(twiname->MASTER.STATUS & TWI_MASTER_WIF_bm));
	TWIE.MASTER.DATA = command;
	while(!(TWIE.MASTER.STATUS & TWI_MASTER_WIF_bm));
	TWIE.MASTER.CTRLC = 0x07;

}

void classSSD1306::sendData(uint8_t data)
{
	//twiname->MASTER.CTRLC = 0;
	TWIE.MASTER.CTRLC = 0;
	TWIE.MASTER.ADDR = 0x78;  // write to Display
	while(!(TWIE.MASTER.STATUS&TWI_MASTER_WIF_bm));
	TWIE.MASTER.DATA = 0x40;       // write word addr
	while(!(TWIE.MASTER.STATUS&TWI_MASTER_WIF_bm));
	TWIE.MASTER.DATA = data;
	while(!(TWIE.MASTER.STATUS & TWI_MASTER_WIF_bm));
	TWIE.MASTER.CTRLC = 0x07;

}


void classSSD1306::invert(uint8_t inverted) {
	if (inverted) {
		sendCommand(SSD1306_INVERTDISPLAY);
		} else {
		sendCommand(SSD1306_NORMALDISPLAY);
	}
}

void classSSD1306::OLED_write(void)
{
	unsigned x;
	sendCommand(SSD1306_COLUMNADDR);
	sendCommand(0x00);
	sendCommand(0x7F);

	sendCommand(SSD1306_PAGEADDR);
	sendCommand(0x00);
	sendCommand(0x07);

	TWIE.MASTER.CTRLC = 0;
	TWIE.MASTER.ADDR = 0x78;  // write to Display
	while(!(TWIE.MASTER.STATUS&TWI_MASTER_WIF_bm));
	TWIE.MASTER.DATA = 0x40;       // write word addr
	while(!(TWIE.MASTER.STATUS&TWI_MASTER_WIF_bm));
	
	for(x = 0; x < (128 * 64 / 8); x++)//*32
	{
		TWIE.MASTER.DATA = buffer[x];
		while(!(TWIE.MASTER.STATUS & TWI_MASTER_WIF_bm));
	}
	TWIE.MASTER.CTRLC = 0x07;
}

void classSSD1306::sendFramebuffer(uint8_t *buffer)
{
	sendCommand(SSD1306_COLUMNADDR);
	sendCommand(0x00);
	sendCommand(0x7F);

	sendCommand(SSD1306_PAGEADDR);
	sendCommand(0x00);
	sendCommand(0x07);

	// We have to send the buffer as 16 bytes packets
	// Our buffer is 1024 bytes long, 1024/16 = 64
	// We have to send 64 packets
	
	TWIE.MASTER.CTRLC = 0;
	TWIE.MASTER.ADDR = 0x78;  // write to Display
	while(!(TWIE.MASTER.STATUS&TWI_MASTER_WIF_bm));
	TWIE.MASTER.DATA = 0x40;       // write word addr
	while(!(TWIE.MASTER.STATUS&TWI_MASTER_WIF_bm));
	for (uint8_t packet = 0; packet < 64; packet++)
	{
		for (uint8_t packet_byte = 0; packet_byte < 16; ++packet_byte)
		{
			TWIE.MASTER.DATA = buffer[packet*16+packet_byte];
			while(!(TWIE.MASTER.STATUS & TWI_MASTER_WIF_bm));
		}
	}
	TWIE.MASTER.CTRLC = 0x07;
	
}

void classSSD1306::OLED_lscroll(char start, char stop) 
{
	sendCommand(0x27);
	sendCommand(0X00);
	sendCommand(start);
	sendCommand(0X00);
	sendCommand(stop);
	sendCommand(0X00);
	sendCommand(0XFF);
	sendCommand(0x2F); //Activate scroll
}


void classSSD1306::OLED_rscroll(char start, char stop)
{
	sendCommand(0x26);
	sendCommand(0X00);
	sendCommand(start);
	sendCommand(0X00);
	sendCommand(stop);
	sendCommand(0X00);
	sendCommand(0XFF);
	sendCommand(0x2F); //Activate scroll
}

void classSSD1306::OLED_stopscroll() 
{
	sendCommand(0x2E);
}


// Draw a PROGMEM-resident 1-bit image at the specified (x,y) position,
// using the specified foreground color (unset bits are transparent).
void classSSD1306::drawBitmap(int16_t x, int16_t y,
const uint8_t bitmap[], int16_t w, int16_t h) 
{

	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t byte = 0;

	for(int16_t j=0; j<h; j++, y++) 
	{
		for(int16_t i=0; i<w; i++) 
		{
			if(i & 7) byte <<= 1;
			else      byte   = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
			if(byte & 0x80) drawPixel(x+i, y);
		}
	}
}

// Draw PROGMEM-resident XBitMap Files (*.xbm), exported from GIMP,
// Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
// C Array can be directly used with this function.
// There is no RAM-resident version of this function; if generating bitmaps
// in RAM, use the format defined by drawBitmap() and call that instead.
void classSSD1306::drawXBitmap(int16_t x, int16_t y,
const uint8_t bitmap[], int16_t w, int16_t h) 
{

	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t byte = 0;

	for(int16_t j=0; j<h; j++, y++) 
	{
		for(int16_t i=0; i<w; i++ ) 
		{
			if(i & 7) byte >>= 1;
			else      byte   = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
			// Nearly identical to drawBitmap(), only the bit order
			// is reversed here (left-to-right = LSB to MSB):
			if(byte & 0x01) drawPixel(x+i, y);
		}
	}
}



void classSSD1306::drawBuffer(const uint8_t *progmem_buffer) 
{
	uint8_t current_byte;

	for (uint8_t y_pos = 0; y_pos < 64; y_pos++) 
	{
		for (uint8_t x_pos = 0; x_pos < 128; x_pos++) 
		{
		   current_byte = pgm_read_byte(progmem_buffer + y_pos*16 + x_pos/8);
           if (current_byte & (128 >> (x_pos&7))) 
		   {
	          this->drawPixel(x_pos,y_pos);
	       } 
	    }
	}
}







void classSSD1306::drawPixel(uint8_t pos_x, uint8_t pos_y) 
{
    if (pos_x >= SSD1306_WIDTH || pos_y >= SSD1306_HEIGHT) 
	{
        return;
    }

    this->buffer[pos_x+(pos_y/8)*SSD1306_WIDTH] |= (1 << (pos_y&7));
}



void classSSD1306::drawVLine(uint8_t x, uint8_t y, uint8_t length) {
    for (uint8_t i = 0; i < length; ++i) 
	{
        this->drawPixel(x,i+y);
    }
}

void classSSD1306::drawHLine(uint8_t x, uint8_t y, uint8_t length) {
    for (uint8_t i = 0; i < length; ++i) {
        this->drawPixel(i+x,y);
    }
}

void classSSD1306::drawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    uint8_t length = x2 - x1 + 1;
    uint8_t height = y2 - y1;

    this->drawHLine(x1,y1,length);
    this->drawHLine(x1,y2,length);
    this->drawVLine(x1,y1,height);
    this->drawVLine(x2,y1,height);
}

void classSSD1306::drawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t fill) {
    if (!fill) {
        this->drawRectangle(x1,y1,x2,y2);
    } else {
        uint8_t length = x2 - x1 + 1;
        uint8_t height = y2 - y1;

        for (int x = 0; x < length; ++x) {
            for (int y = 0; y <= height; ++y) {
                this->drawPixel(x1+x,y+y1);
            }
        }
    }
}

void classSSD1306::clear(void) 
{
    for (uint16_t buffer_location = 0; buffer_location < SSD1306_BUFFERSIZE; buffer_location++) 
	{
        this->buffer[buffer_location] = 0x00;
    }
}




void classSSD1306::drawChar(uint8_t character, int16_t x, int16_t y) 
{
	int16_t table_offset = (character - 0x20) * 8;
	  
	 for(int8_t i=0; i<8; i++ ) 
	 { // Char bitmap = 8 columns
		 uint8_t line = pgm_read_byte(&Terminal8x8[i+table_offset]);
		 for(int8_t j=0; j<8; j++, line >>= 1) 
		 {
			 if(line & 1) 
			 {
				 drawPixel(x+i, y+j);
			 }
		 }
	 }  
}




void classSSD1306::drawString(char * str, int16_t x, int16_t y) 
{
	int16_t pos = 0;
	char character = str[pos++];
	int16_t startx = x;
	int16_t starty = y;
	while(character != '\0')
	{
		drawChar(character, startx, starty);
		if(startx >= 127) starty++; //wrap around
		startx += 8;
		character = str[pos++];
	}
}










//------------------------------------------------------------------------------
void classSSD1306::setRow(uint8_t row) 
{
	if (row >= SSD1306_HEIGHT / 8) return;
	m_row = row;
	sendCommand(SSD1306_SETSTARTPAGE | row);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void classSSD1306::setCol(uint8_t col) 
{
	if (col >= SSD1306_WIDTH) return;
	m_col = col;
	col += m_colOffset;
	sendCommand(SSD1306_SETLOWCOLUMN | (col & 0XF));
	sendCommand(SSD1306_SETHIGHCOLUMN | (col >> 4));
}

//------------------------------------------------------------------------------
void classSSD1306::setCursor(uint8_t col, uint8_t row)
{
	setCol(col);
	setRow(row);
}






void classSSD1306::show() 
{
    this->sendFramebuffer(this->buffer);
	//this->OLED_write();
}