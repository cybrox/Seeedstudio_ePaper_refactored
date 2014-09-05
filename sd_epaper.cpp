#include <SD.h>
#include <SPI.h>

#include "sd_epaper.h"
#include "ePaperDfs.h"


void sd_epaper::begin(EPD_size sz) {
  DISP_LEN  = 200;
  DISP_WIDTH  = 96;
  LINE_BYTE = 200/8;
  clear();
}


void sd_epaper::putPixel(int x, int y, unsigned char pixel) {
  int x1 = x;
  int y1 = y;
  
  if(x>DISP_LEN || y>DISP_WIDTH)return;
  
  int bit = x & 0x07;
  int byte = (x>>3) + y * LINE_BYTE;
  
  int mask = 0x01 << bit;

  if (BLACK == pixel) sram_image[byte] |= mask;
  else sram_image[byte] &= ~mask;
}


unsigned char sd_epaper::clear() {
  memset(sram_image, 0x00, 5808);
}

sd_epaper eSD;
