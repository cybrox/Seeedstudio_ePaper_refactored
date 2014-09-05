#ifndef __EPAPER_H__
#define __EPAPER_H__

#include <SD.h>
#include <SPI.h>
#include <EPD.h>

#include "ePaperDfs.h"

class ePaper {

  private:
    int DISPLAY_SIZE_X;
    int DISPLAY_SIZE_Y;
    int DISPLAY_LINE_B;
    EPD_DIR DISPLAY_DIRECT;

    /* Buffer */
    unsigned char _buffer[5808];

    /* Matrix */
    int matrix_pin_cs;
    unsigned char matrix_character[32];
    unsigned long matrix_get_unicode_address(unsigned int uniCode);
    unsigned long matrix_read(unsigned long Address);
    

  public:
    void begin();
    void begin_io();
    void display();
 
    void clear() {
      EPD.start();
      EPD.clear();
      EPD.end();
    } 
    

    /**
     * Buffer methods, merged from old sd_epaper
     */
    void buffer_clear();
    void buffer_write(int x, int y, bool fill);


    /**
     * Matrix method to get unicode chars from display chip
     */
    void matrix_begin();
    int matrix_get_unicode(unsigned int uniCode, unsigned char *matrix);


    /**
     * Draw methods that allow the user to actually do
     * something with the display's content before rendering
     */
    int drawChar(char c, int x, int y);
    int drawString(char *string, int poX, int poY);
    int drawNumber(long long_num,int poX, int poY);
    int drawFloat(float floatNumber,int decimal,int poX, int poY);
    
    int drawUnicode(unsigned int uniCode, int x, int y);
    int drawUnicode(unsigned char *matrix, int x, int y);
    
    int drawUnicodeString(unsigned int *uniCode, int len, int x, int y);
    
    void drawLine(int x0, int y0, int x1, int y1);
    void drawCircle(int poX, int poY, int r);
    void drawHorizontalLine( int poX, int poY, int len);
    void drawVerticalLine( int poX, int poY, int len);
    void drawRectangle(int poX, int poY, int len, int width);
    void fillRectangle(int poX, int poY, int len, int width);
    void fillCircle(int poX, int poY, int r);
    void drawTraingle( int poX1, int poY1, int poX2, int poY2, int poX3, int poY3);
};

extern ePaper EPAPER;

#endif
