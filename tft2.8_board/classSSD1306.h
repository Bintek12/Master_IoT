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

#include <avr/io.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include "ssd1306_fonts.h"
#include <string.h>
#define SSD1306_DEFAULT_ADDRESS 0x78
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_SWITCHCAPVCC 0x2
#define SSD1306_NOP 0xE3
#define SSD1306_SETSTARTPAGE 0XB0

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_BUFFERSIZE (SSD1306_WIDTH * SSD1306_HEIGHT) / 8

//------------------------------------------------------------------------------
// Values for writeDisplay() mode parameter.
/** Write to Command register. */
#define SSD1306_MODE_CMD     0
/** Write one byte to display RAM. */
#define SSD1306_MODE_RAM     1
/** Write to display RAM with possible buffering. */
#define SSD1306_MODE_RAM_BUF 2




class classSSD1306 {
public:
    classSSD1306();
	void OledInit(void) ;
	void sendFramebuffer(uint8_t *buffer);
	void OLED_write(void);
	void invert(uint8_t inverted);

    void drawBuffer(const uint8_t *buffer);
    void drawPixel(uint8_t pos_x, uint8_t pos_y);
    void drawVLine(uint8_t x, uint8_t y, uint8_t length);
    void drawHLine(uint8_t pos_y, uint8_t start, uint8_t length);
    void drawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
    void drawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t fill);
	void drawChar(uint8_t character, int16_t x, int16_t y);
	void drawString(char * str, int16_t x, int16_t y);
	void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);

	void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);
    void clear();
    void show();
	void OLED_lscroll(char start, char stop);
	void OLED_rscroll(char start, char stop);
	void OLED_stopscroll();
private:
    uint8_t m_col;
	uint8_t m_row;
	uint8_t m_colOffset;
	uint8_t buffer_index;
    uint8_t buffer[SSD1306_BUFFERSIZE];
	void sendCommand(uint8_t command);
	void sendData(uint8_t data);
	void setRow(uint8_t row);
	void setCol(uint8_t col);
	void setCursor(uint8_t col, uint8_t row);

};