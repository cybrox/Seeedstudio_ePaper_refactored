#ifndef __EPAPER_H__
#define __EPAPER_H__

#include <SD.h>
#include <SPI.h>
#include <EPD.h>

#include "ePaperDfs.h"

////////////////////////////////////////////////////////////////////////////////
// Small ePaper shield refactored config.                                     //
// You might want to adjust these values in order to use different            //
// ePaper shields. This supports the 3 default sized from seeedstudio.        //
//                                                                            //
// Choose one of the folloqing display sizes:                                 //
// EPD_1_44 (128 x 96)                                                        //
// EPD_2_0 (200 x 96)                                                         //
// EPD_2_7 (264 x 176)                                                        //
  #define DISPLAYSIZE EPD_1_44                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



// calculate buffer size
#if DISPLAYSIZE == EPD_1_44
#define BUFFERSIZE 1536   // (128 x 96) / 8
#elif DISPLAYSIZE == EPD_2_0
#define BUFFERSIZE 2400   // (200 x 96) / 8
#else
#define BUFFERSIZE 5808   // (264 * 176) / 8
#endif

class ePaper {

  private:
    int DISPLAY_SIZE_X;
    int DISPLAY_SIZE_Y;
    int DISPLAY_LINE_B;
    EPD_DIR DISPLAY_DIRECT;


    unsigned char _buffer[BUFFERSIZE];


    int matrix_pin_cs;
    unsigned char matrix_character[32];
    unsigned long matrix_get_unicode_address(unsigned int uniCode);
    unsigned long matrix_read(unsigned long Address);
    

  public:
    void begin();
    void display();
    void hardware_begin();


    void buffer_clear();
    void buffer_write(int x, int y, bool fill);


    void matrix_begin();
    int matrix_get_unicode(unsigned int uniCode, unsigned char *matrix);


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
