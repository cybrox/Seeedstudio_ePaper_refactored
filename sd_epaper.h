#ifndef __SD_EPAPER_H__
#define __SD_EPAPER_H__

#include "ePaperDfs.h"
#include "EPD.h"

#define BLACK   1
#define WHITE   0

class sd_epaper {

  private:
    int LINE_BYTE;
    int DISP_LEN;
    int DISP_WIDTH;


  public:
    void begin(EPD_size sz);
    void putPixel(int x, int y, unsigned char pixel);
    unsigned char clear();
    unsigned char sram_image[5808];

};

extern sd_epaper eSD;

#endif
