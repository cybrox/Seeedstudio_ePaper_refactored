#ifndef __GT20L16_DRIVE_H__
#define __GT20L16_DRIVE_H__

class GT20L16_drive{

  private:
    int pinCS;
    unsigned long getAddrFromUnicode(unsigned int uniCode);
    unsigned long GTRead(unsigned long Address);


  public:
    void begin(int pinSelect); 
    void begin(){begin(9);}
    int getMatrixUnicode(unsigned int uniCode, unsigned char *matrix);

};

extern GT20L16_drive GT20L16;

#endif
