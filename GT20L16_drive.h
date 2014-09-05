#ifndef __GT20L16_DRIVE_H__
#define __GT20L16_DRIVE_H__

#define PIN_GT20L16_CS      9

class GT20L16_drive{

private:

    int pinCS;

private:

    void GT_Select();
    void GT_UnSelect();
    unsigned long getAddrFromUnicode(unsigned int uniCode);
    unsigned long GTRead(unsigned long Address);
public:

    void begin(int pinSelect);
    
    void begin(){begin(PIN_GT20L16_CS);}

    int getMatrixUnicode(unsigned int uniCode, unsigned char *matrix);


};

extern GT20L16_drive GT20L16;

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/