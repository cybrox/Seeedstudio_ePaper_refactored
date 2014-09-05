/*
  sd_paper.cpp
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Modified by Loovee
  www.seeedstudio.com
  2013-7-7

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <SD.h>
#include <SPI.h>

#include "sd_epaper.h"
#include "ePaperDfs.h"


static void spi_on()
{
    SPI.begin();
    //SPI.setClockDivider(SPI_CLOCK_DIV2);
    //SPI_put(0x00);
    //SPI_put(0x00);
}
/*********************************************************************************************************
** Function name:           begin
** Descriptions:            begin
*********************************************************************************************************/
void sd_epaper::begin(EPD_size sz)
{

    openFileTime = 1;
    closeFileTime = 1;
    //spi_on();
    switch(sz)
    {
        case EPD_1_44:              // 128*96
        SIZE_LEN    = 128;
        SIZE_WIDTH  = 96;
        break;
        
        case EPD_2_0:               // 200*96
        SIZE_LEN    = 200;
        SIZE_WIDTH  = 96;
        break;
        
        case EPD_2_7:               // 264*176
        SIZE_LEN    = 264;
        SIZE_WIDTH  = 176;
        break;
        
        default:
        println_sd("wrong size");
        while(1);                   // die here
    }
    
    DISP_LEN    = SIZE_LEN;
    DISP_WIDTH  = SIZE_WIDTH;
    
    LINE_BYTE = SIZE_LEN/8;
    clear();

}


void sd_epaper::setDirection(EPD_DIR dir)
{
    direction = dir;
    
    if((direction == DIRLEFT) || (direction == DIRRIGHT))
    {
        DISP_LEN    = SIZE_WIDTH;
        DISP_WIDTH  = SIZE_LEN;
    }
}


/*********************************************************************************************************
** Function name:           putPixel
** Descriptions:            putPixel
*********************************************************************************************************/
void sd_epaper::putPixel(int x, int y, unsigned char pixel)
{
    int x1 = x;
    int y1 = y;
    
    switch(direction)
    {
        case DIRLEFT:
        
        x = y1;
        y = SIZE_WIDTH-x1;
        
        break;
        
        case DIRRIGHT:
        
        x = SIZE_LEN - y1;
        y = x1;
        break;
        
        case DIRDOWN:
        
        x = SIZE_LEN - x1;
        y = SIZE_WIDTH - y1;
        break;
        
        default:
        
        ;
    }
    
    if(x>DISP_LEN || y>DISP_WIDTH)return;
    
    int bit = x & 0x07;
    int byte = (x>>3) + y * LINE_BYTE;
    
    int mask = 0x01 << bit;

    if (BLACK == pixel)
    {
        sram_image[byte] |= mask;
    }
    else
    {
        sram_image[byte] &= ~mask;
    }

}


/*********************************************************************************************************
** Function name:           begin
** Descriptions:            begin
*********************************************************************************************************/
unsigned char sd_epaper::clear()
{
    memset(sram_image, 0x00, 5808);
}

sd_epaper eSD;
