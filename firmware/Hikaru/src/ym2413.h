#ifndef H_YM2413
#define H_YM2413
#include<mbed.h>
#include"hc595.h"

class ym2413{
public:
    ym2413(hc595 shiftRegister,PinName wrcs,PinName ic,PinName a0);
    void setRegister(byte addr,byte data);
    void reset(void);
    void setInstVolume(int ch,byte inst,int vol);
    void setFnumber(int ch,int oct,int f_number,bool key);
    void setDrums(byte data);
private:
    hc595    _shiftRegister;
    DigitalOut      _wrcs;
    DigitalOut      _ic;
    DigitalOut      _a0;
};
#endif