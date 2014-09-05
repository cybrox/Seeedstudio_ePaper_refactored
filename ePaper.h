#ifndef __EPAPER_H__
#define __EPAPER_H__

#include <SD.h>
#include <SPI.h>
#include <EPD.h>

#include "sd_epaper.h"
#include "ePaperDfs.h"

#define EP_DEBUG            1

#if EP_DEBUG
#define print_ep(X)         Serial.print(X)
#define println_ep(X)       Serial.println(X)
#else
#define print_ep(X)
#define println_ep(X)
#endif

class ePaper
{

private:

    unsigned char tMatrix[32];
    
    int DISP_LEN;
    int DISP_WIDTH;
    
    EPD_DIR direction;
    
public:

    EPD_size size;
    
    void begin(EPD_size sz);
    void setDirection(EPD_DIR dir);
    void init_io();
    
    unsigned char display();                // refresh 
    
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
    
    void clear_sd();                         // clear sd card 

    void image_sram(unsigned char *image) {
        EPD.start();
        EPD.image_sram(image);
        EPD.end();
    }
    
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
