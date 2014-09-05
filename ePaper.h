#ifndef __EPAPER_H__
#define __EPAPER_H__

#include <SD.h>
#include <SPI.h>
#include <EPD.h>

#include "ePaperDfs.h"

class ePaper {

  private:
    unsigned char tMatrix[32];
    
    int DISP_LEN;
    int DISP_WIDTH;
    
    EPD_DIR direction;

    /* Buffer */
    unsigned char _buffer[5808];
    
  public:

    EPD_size size;
    
    void begin(EPD_size sz);
    void setDirection(EPD_DIR dir);
    void init_io();
    
    unsigned char display();
    
    /* REDIRECTING FUNCTIONS /FOR NOW/ */
    void image_flash(PROGMEM const unsigned char *image) {
      EPD.start();
      EPD.image(image);
      EPD.end();
    } 
 
    void clear() {
      EPD.start();
      EPD.clear();
      EPD.end();
    } 

    void image_sram(unsigned char *image) {
      EPD.start();
      EPD.image_sram(image);
      EPD.end();
    }
    

    /**
     * Buffer methods, merged from old sd_epaper
     */
    void buffer_clear();
    void buffer_write(int x, int y, bool fill);


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
