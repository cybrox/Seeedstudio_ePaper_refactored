#include <Arduino.h>
#include <SPI.h>
#include "ePaper.h"


void ePaper::begin(EPD_size sz){
  DISPLAY_DIRECT = DIRNORMAL;
  DISPLAY_SIZE_X = 200; // Display size (in pixels) on x axis
  DISPLAY_SIZE_Y = 96;  // Display size (in pixels) on y axis
  DISPLAY_LINE_B = 25;  // Display bytes per line (x size / 8)
  
  EPD.begin(sz);
  init_io();
  matrix_begin();
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



/****************************************************************************************************************************************************/
/***** Buffer methods merged from sd_epaper                                                                                                     *****/
/****************************************************************************************************************************************************/
void ePaper::buffer_write(int x, int y, bool fill){
  if(x > DISPLAY_SIZE_X || y > DISPLAY_SIZE_Y)return;
  
  int bit = x & 0x07;
  int byte = (x>>3) + y * DISPLAY_LINE_B;
  
  int mask = 0x01 << bit;

  if (fill) _buffer[byte] |= mask;
  else _buffer[byte] &= ~mask;
}


void ePaper::buffer_clear() {
  memset(_buffer, 0x00, 5808);
}



/****************************************************************************************************************************************************/
/***** Matrix unicode methods merged from GT20L16                                                                                               *****/
/****************************************************************************************************************************************************/
void ePaper::matrix_begin() {
  matrix_pin_cs = 9;    // Static CS pin
  pinMode(matrix_pin_cs, OUTPUT);
  digitalWrite(matrix_pin_cs, HIGH);
}


int ePaper::matrix_get_unicode(unsigned int uniCode, unsigned char *matrix) {
  if(matrix == NULL)return 0;
  
  init_io();
  matrix_begin();

  unsigned char i;
  unsigned char tempdata;
  unsigned long Add=0;
  
  unsigned char dtaLen = 0;
  
  if(uniCode <= 45632 ) dtaLen = 16;
  else dtaLen = 32;

  Add=matrix_get_unicode_address(uniCode);
  delayMicroseconds(10);
  
  digitalWrite(matrix_pin_cs, LOW);
  SPI.transfer(0x03);
  SPI.transfer(Add>>16);
  SPI.transfer(Add>>8);
  SPI.transfer(Add);

  SPI.setBitOrder(LSBFIRST);

  for(i=0;i<dtaLen;i++) {
    tempdata=SPI.transfer(0x00);
    matrix[i]=(tempdata);
    delay(5);
  }

  SPI.setBitOrder(MSBFIRST);
  digitalWrite(matrix_pin_cs, HIGH);
  
  return dtaLen;
}


unsigned long ePaper::matrix_get_unicode_address(unsigned int uniCode) {
  if (uniCode <= 45632) {
    unsigned int BaseAdd=0;
    unsigned long Address;
    if(uniCode>=0x20&&uniCode<=0x7f)
    Address=16*(uniCode-0x20)+BaseAdd;
    else if(uniCode>=0xa0&&uniCode<=0xff)
    Address=16*(96+uniCode-0xa0)+BaseAdd;
    else if(uniCode>=0x100&&uniCode<=0x17f)
    Address=16*(192+uniCode-0x100)+BaseAdd;
    else if(uniCode>=0x1a0&&uniCode<=0x1cf)
    Address=16*(320+uniCode-0x1a0)+BaseAdd;
    else if(uniCode>=0x1f0&&uniCode<=0x1ff)
    Address=16*(368+uniCode-0x1f0)+BaseAdd;
    else if(uniCode>=0x210&&uniCode<=0x21f)
    Address=16*(384+uniCode-0x210)+BaseAdd;
    else if(uniCode>=0x1ea0&&uniCode<=0x1eff)
    Address=16*(400+uniCode-0x1ea0)+BaseAdd;
    else if(uniCode>=0x370&&uniCode<=0x3cf)
    Address=16*(496+uniCode-0x370)+BaseAdd;
    else if(uniCode>=0x400&&uniCode<=0x45f)
    Address=16*(592+uniCode-0x400)+BaseAdd;
    else if(uniCode>=0x490&&uniCode<=0x4ff)
    Address=16*(688+uniCode-0x490)+BaseAdd;
    else if(uniCode>=0x590&&uniCode<=0x5ff)
    Address=16*(800+uniCode-0x100)+BaseAdd;
    else if(uniCode>=0xe00&&uniCode<=0xe7f)
    Address=16*(912+uniCode-0xe00)+BaseAdd;
    else  Address=BaseAdd;
    return Address;
  }
}


unsigned long ePaper::matrix_read(unsigned long Address) {
  unsigned char i;
  unsigned char buffer[2]={0};
  unsigned int data;
  delayMicroseconds(10);
  digitalWrite(matrix_pin_cs, LOW);
  SPI.transfer(0x03);
  SPI.transfer(Address>>16);
  SPI.transfer(Address>>8);
  SPI.transfer(Address);
  for(i=0;i<2;i++)
  {
    buffer[i]=SPI.transfer(0x00);
  }
  digitalWrite(matrix_pin_cs, HIGH);
  data=buffer[0]*256+buffer[1];
  return data;
}



/****************************************************************************************************************************************************/
/***** User accessible methods for drawing                                                                                                      *****/
/****************************************************************************************************************************************************/
int ePaper::drawUnicode(unsigned int uniCode, int x, int y) {
  int dtaLen = matrix_get_unicode(uniCode, matrix_character);


  int pX    = 0;
  int pY    = 0;
  int color   = 0;

  for(int k = 0; k<2; k++) {
    for(int i=0; i<8; i++) {
      for(int j=0; j<(dtaLen/2); j++) {
        if(matrix_character[j+(dtaLen/2)*k] & (0x01<<(7-i))) {  
          color = 1;
        } else {
          color = 0;
        }
        
        pX = x + j;
        pY = y + k*8+i;
        
        
        buffer_write(pX, pY, color);
      }
    }
  }

  return dtaLen/2;    // x +
}


int ePaper::drawUnicode(unsigned char *matrix, int x, int y)
{

  int dtaLen  = 32;
  int pX    = 0;
  int pY    = 0;
  int color   = 0;

  for(int k = 0; k<2; k++) {
    for(int i=0; i<8; i++) {
      for(int j=0; j<(dtaLen/2); j++) {
        if(matrix[j+(dtaLen/2)*k] & (0x01<<(7-i))) {  
          color = 1;
        } else {
          color = 0;
        }
        
        pX = x + j;
        pY = y + k*8+i;
        
        buffer_write(pX, pY, color);
      }
    }
  }

  return dtaLen/2;    // x +
}


int ePaper::drawUnicodeString(unsigned int *uniCode, int len, int x, int y) {
  int xPlus = 0;
  int xSum  = 0;
  
  for(int i=0; i<len; i++) {
    xPlus = drawUnicode(uniCode[i], x, y);
    x += xPlus;
    xSum += xPlus;
  }
  return xSum;
}


int ePaper::drawChar(char c, int x, int y) {
  return drawUnicode(c, x, y);
}


int ePaper::drawString(char *string, int poX, int poY) {
  int sumX = 0;

  while(*string) {  
    int xPlus = drawChar(*string, poX, poY);
    sumX += xPlus;
    *string++;
    
    if(poX < 200) poX += xPlus;
  }
  
  return sumX;
}


int ePaper::drawNumber(long long_num,int poX, int poY) {
  char tmp[10];
  sprintf(tmp, "%d", long_num);
  return drawString(tmp, poX, poY);
}


int ePaper::drawFloat(float floatNumber, int decimal, int poX, int poY) {
  unsigned long temp=0;
  float decy=0.0;
  float rounding = 0.5;
  unsigned char f=0;
  
  float eep = 0.000001;
  
  int sumX  = 0;
  int xPlus   = 0;
  
  if(floatNumber-0.0 < eep) {
    xPlus = drawChar('-',poX, poY);
    floatNumber = -floatNumber;

    poX  += xPlus; 
    sumX += xPlus;
  }
  
  for (unsigned char i=0; i<decimal; ++i) {
    rounding /= 10.0;
  }
  
  floatNumber += rounding;

  temp = (long)floatNumber;
  
  
  xPlus = drawNumber(temp,poX, poY);

  poX  += xPlus; 
  sumX += xPlus;

  if(decimal>0) {
    xPlus = drawChar('.',poX, poY);
    poX += xPlus;                     /* Move cursor right      */
    sumX += xPlus;
  } else {
    return sumX;
  }
  
  decy = floatNumber - temp;
  for(unsigned char i=0; i<decimal; i++) {
    decy *= 10;                            /* for the next decimal     */
    temp = decy;                          /* get the decimal        */
    xPlus = drawNumber(temp,poX, poY);
    
    poX += xPlus;                     /* Move cursor right      */
    sumX += xPlus;
    decy -= temp;
  }
  return sumX;
}


void ePaper::drawLine(int x0, int y0, int x1, int y1) {
  init_io();
  
  int x = x1-x0;
  int y = y1-y0;
  int dx = abs(x), sx = x0<x1 ? 1 : -1;
  int dy = -abs(y), sy = y0<y1 ? 1 : -1;
  int err = dx+dy, e2;                        
  for (;;) {                              
    buffer_write(x0,y0,1);
    e2 = 2*err;
    if (e2 >= dy) {                        
      if (x0 == x1) break;
      err += dy; x0 += sx;
    }
    if (e2 <= dx) {                        
      if (y0 == y1) break;
      err += dx; y0 += sy;
    }
  }
}


void ePaper::drawCircle(int poX, int poY, int r) {
  init_io();
  int x = -r, y = 0, err = 2-2*r, e2;
  do {
    buffer_write(poX-x, poY+y, 1);
    buffer_write(poX+x, poY+y, 1);
    buffer_write(poX+x, poY-y, 1);
    buffer_write(poX-x, poY-y, 1);
    e2 = err;
    if (e2 <= y) {
      err += ++y*2+1;
      if (-x == y && e2 <= x) e2 = 0;
    }
    if (e2 > x) err += ++x*2+1;
  } while (x <= 0);
}


void ePaper::drawHorizontalLine( int poX, int poY, int len) {
  drawLine(poX, poY, poX+len, poY);
}


void ePaper::drawVerticalLine( int poX, int poY, int len) {
  drawLine(poX, poY, poX, poY+len);
}


void ePaper::drawRectangle(int poX, int poY, int len, int width) {
  drawHorizontalLine(poX, poY, len);
  drawHorizontalLine(poX, poY+width, len);
  drawVerticalLine(poX, poY, width);
  drawVerticalLine(poX + len, poY, width);
}


void ePaper::fillCircle(int poX, int poY, int r) {
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


void ePaper::drawTraingle( int poX1, int poY1, int poX2, int poY2, int poX3, int poY3) {
  drawLine(poX1, poY1, poX2, poY2);
  drawLine(poX1, poY1, poX3, poY3);
  drawLine(poX2, poY2, poX3, poY3);
}


void ePaper::fillRectangle(int poX, int poY, int len, int width) {
  for(int i=0; i<width; i++) drawHorizontalLine(poX, poY+i, len);
}


unsigned char ePaper::display() {
  EPD.start();
  EPD.image_sram(_buffer);
  EPD.end();
}


ePaper EPAPER;
