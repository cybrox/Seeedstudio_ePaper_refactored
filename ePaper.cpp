#include <Arduino.h>
#include <SPI.h>
#include "GT20L16_drive.h"
#include "ePaper.h"


static void spi_on(){
    SPI.begin();
}

void ePaper::begin(EPD_size sz){
    size = sz;
    direction = DIRNORMAL;

    DISP_LEN    = 200;
    DISP_WIDTH  = 96;
    
    EPD.begin(size);
    init_io();
}

void ePaper::start(){
    EPD.start();
}

void ePaper::end(){
    EPD.end();
}

void ePaper::init_io(){
    pinMode(Pin_BUSY, INPUT);
    pinMode(Pin_RESET, OUTPUT);
    pinMode(Pin_PANEL_ON, OUTPUT);
    pinMode(Pin_DISCHARGE, OUTPUT);
    pinMode(Pin_BORDER, OUTPUT);
    pinMode(Pin_EPD_CS, OUTPUT);
    pinMode(Pin_SD_CS, OUTPUT);
    
    pinMode(9, OUTPUT);
    digitalWrite(9, HIGH);

    digitalWrite(Pin_RESET, LOW);
    digitalWrite(Pin_PANEL_ON, LOW);
    digitalWrite(Pin_DISCHARGE, LOW);
    digitalWrite(Pin_BORDER, LOW);
    digitalWrite(Pin_EPD_CS, HIGH);
    digitalWrite(Pin_SD_CS, HIGH);
    
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
}


int ePaper::drawUnicode(unsigned int uniCode, int x, int y) {
   

    int dtaLen = GT20L16.getMatrixUnicode(uniCode, tMatrix);


    int pX      = 0;
    int pY      = 0;
    int color   = 0;

    for(int k = 0; k<2; k++)
    {
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<(dtaLen/2); j++)
            {

                if(tMatrix[j+(dtaLen/2)*k] & (0x01<<(7-i)))
                {  
                    color = 1;
                }
                else
                {
                    color = 0;
                }
                
                pX = x + j;
                pY = y + k*8+i;
                
                
                drawPixel(pX, pY, color);
            }
        }
    }

    return dtaLen/2;        // x +
}

int ePaper::drawUnicode(unsigned char *matrix, int x, int y)
{

    int dtaLen  = 32;
    int pX      = 0;
    int pY      = 0;
    int color   = 0;
    //spi_on();
    for(int k = 0; k<2; k++)
    {
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<(dtaLen/2); j++)
            {

                if(matrix[j+(dtaLen/2)*k] & (0x01<<(7-i)))
                {  
                    color = 1;
                }
                else
                {
                    color = 0;
                }
                
                pX = x + j;
                pY = y + k*8+i;
                
                drawPixel(pX, pY, color);
            }
        }
    }

    return dtaLen/2;        // x +
}


int ePaper::drawUnicodeString(unsigned int *uniCode, int len, int x, int y)
{
    int xPlus = 0;
    int xSum  = 0;
    
    for(int i=0; i<len; i++)
    {
        xPlus = drawUnicode(uniCode[i], x, y);
        x += xPlus;
        xSum += xPlus;
    }
    return xSum;
}


int ePaper::drawChar(char c, int x, int y)
{
    return drawUnicode(c, x, y);
}


int ePaper::drawString(char *string, int poX, int poY)
{
    int sumX = 0;

    while(*string)
    {
        
        int xPlus = drawChar(*string, poX, poY);
        sumX += xPlus;
        *string++;
        
        if(poX < 200)
        {
            poX += xPlus;                                     /* Move cursor right            */
        }
    }
    
    return sumX;
}


int ePaper::drawNumber(long long_num,int poX, int poY)
{
    char tmp[10];
    sprintf(tmp, "%d", long_num);
    return drawString(tmp, poX, poY);

}


int ePaper::drawFloat(float floatNumber, int decimal, int poX, int poY)
{
    unsigned long temp=0;
    float decy=0.0;
    float rounding = 0.5;
    unsigned char f=0;
    
    float eep = 0.000001;
    
    int sumX    = 0;
    int xPlus   = 0;
    
    if(floatNumber-0.0 < eep)       // floatNumber < 0
    {
        xPlus = drawChar('-',poX, poY);
        floatNumber = -floatNumber;

        poX  += xPlus; 
        sumX += xPlus;
    }
    
    for (unsigned char i=0; i<decimal; ++i)
    {
        rounding /= 10.0;
    }
    
    floatNumber += rounding;

    temp = (long)floatNumber;
    
    
    xPlus = drawNumber(temp,poX, poY);

    poX  += xPlus; 
    sumX += xPlus;

    if(decimal>0)
    {
        xPlus = drawChar('.',poX, poY);
        poX += xPlus;                                       /* Move cursor right            */
        sumX += xPlus;
    }
    else
    {
        return sumX;
    }
    
    decy = floatNumber - temp;
    for(unsigned char i=0; i<decimal; i++)                                      
    {
        decy *= 10;                                                      /* for the next decimal         */
        temp = decy;                                                    /* get the decimal              */
        xPlus = drawNumber(temp,poX, poY);
        
        poX += xPlus;                                       /* Move cursor right            */
        sumX += xPlus;
        decy -= temp;
    }
    return sumX;
}


void ePaper::drawLine(int x0, int y0, int x1, int y1)
{

    init_io();
    
    int x = x1-x0;
    int y = y1-y0;
    int dx = abs(x), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2;                                              
    for (;;)
    {                                                          
        drawPixel(x0,y0,1);
        e2 = 2*err;
        if (e2 >= dy) 
        {                                                
            if (x0 == x1) break;
            err += dy; x0 += sx;
        }
        if (e2 <= dx) 
        {                                                
            if (y0 == y1) break;
            err += dx; y0 += sy;
        }
    }
}


void ePaper::clear_sd()
{
    
    init_io();
    
    for(int i=0; i<DISP_WIDTH; i++)
    {
        for(int j=0; j<DISP_LEN; j++)
        {
            drawPixel(j, i, 0);
            
        }
    }
}


void ePaper::drawCircle(int poX, int poY, int r)
{

    init_io();
    int x = -r, y = 0, err = 2-2*r, e2;
    do {
        drawPixel(poX-x, poY+y, 1);
        drawPixel(poX+x, poY+y, 1);
        drawPixel(poX+x, poY-y, 1);
        drawPixel(poX-x, poY-y, 1);
        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);
}


void ePaper::drawHorizontalLine( int poX, int poY, int len)
{
    drawLine(poX, poY, poX+len, poY);
}


void ePaper::drawVerticalLine( int poX, int poY, int len)
{
    drawLine(poX, poY, poX, poY+len);
}


void ePaper::drawRectangle(int poX, int poY, int len, int width)
{
    drawHorizontalLine(poX, poY, len);
    drawHorizontalLine(poX, poY+width, len);
    drawVerticalLine(poX, poY, width);
    drawVerticalLine(poX + len, poY, width);
}


void ePaper::fillCircle(int poX, int poY, int r)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    
    do {

        drawVerticalLine(poX-x, poY-y, 2*y);
        drawVerticalLine(poX+x, poY-y, 2*y);

        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);

}


void ePaper::drawTraingle( int poX1, int poY1, int poX2, int poY2, int poX3, int poY3)
{
    drawLine(poX1, poY1, poX2, poY2);
    drawLine(poX1, poY1, poX3, poY3);
    drawLine(poX2, poY2, poX3, poY3);
}


void ePaper::fillRectangle(int poX, int poY, int len, int width)
{
    for(int i=0; i<width; i++)
    {
        drawHorizontalLine(poX, poY+i, len);
    }
}


unsigned char ePaper::display()
{
    start();
    EPD.image_sram(eSD.sram_image);
    end();
}

ePaper EPAPER;
