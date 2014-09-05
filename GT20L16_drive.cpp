#include <SPI.h>
#include <Arduino.h>

#include "GT20L16_drive.h"
#include "ePaper.h"


void GT20L16_drive::begin(int pinSelect) {
  pinCS = pinSelect;
  pinMode(pinCS, OUTPUT);
  digitalWrite(pinCS, HIGH);
}


int GT20L16_drive::getMatrixUnicode(unsigned int uniCode, unsigned char *matrix) {
  if(matrix == NULL)return 0;
  
  EPAPER.init_io();
  begin();

  unsigned char i;
  unsigned char tempdata;
  unsigned long Add=0;
  
  unsigned char dtaLen = 0;
  
  if(uniCode <= 45632 ) dtaLen = 16;
  else dtaLen = 32;

  Add=getAddrFromUnicode(uniCode);
  delayMicroseconds(10);
  
  digitalWrite(pinCS, LOW);
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
  digitalWrite(pinCS, HIGH);
  
  return dtaLen;
}


unsigned long GT20L16_drive::getAddrFromUnicode(unsigned int uniCode) {
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
  
  // Removed china stuff for minimized driver
}


unsigned long GT20L16_drive::GTRead(unsigned long Address) {
  unsigned char i;
  unsigned char buffer[2]={0};
  unsigned int data;
  delayMicroseconds(10);
  digitalWrite(pinCS, LOW);
  SPI.transfer(0x03);
  SPI.transfer(Address>>16);
  SPI.transfer(Address>>8);
  SPI.transfer(Address);
  for(i=0;i<2;i++)
  {
    buffer[i]=SPI.transfer(0x00);
  }
  digitalWrite(pinCS, HIGH);
  data=buffer[0]*256+buffer[1];
  return data;
}

GT20L16_drive GT20L16;
