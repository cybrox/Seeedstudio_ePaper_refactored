#ifndef __EPAPERDFS_H__
#define __EPAPERDFS_H__

// pin define

typedef enum {
	DIRNORMAL,
	DIRLEFT,
	DIRRIGHT,
    DIRDOWN
} EPD_DIR;


#define Pin_TEMPERATURE   A0
#define Pin_PANEL_ON      2
#define Pin_BORDER        3
#define Pin_DISCHARGE     8
#define Pin_PWM           5
#define Pin_RESET         6
#define Pin_BUSY          7

#define Pin_EPD_CS        10

#define Pin_SD_CS         4

#define Pin_OE123         A1
#define Pin_STV_IN        A3


// spi cs

#define EPD_SELECT()        digitalWrite(Pin_EPD_CS, LOW)
#define EPD_UNSELECT()      digitalWrite(Pin_EPD_CS, HIGH)
#define SD_SELECT()         digitalWrite(Pin_SD_CS, LOW) 
#define SD_UNSELECT()       digitalWrite(Pin_SD_CS, HIGH)
#define FONT_SELECT()       digitalWrite(Pin_Font_CS, LOW) 
#define FONT_UNSELECT()     digitalWrite(Pin_Font_CS, HIGH)


#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/