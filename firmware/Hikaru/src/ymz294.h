#ifndef H_YMZ294
#define H_YMZ294
#include <mbed.h>
#include "hc595.h"
#define word unsigned short int
class ymz294{
public:
    ymz294(hc595 shiftRegister,PinName wrcs,PinName ic,PinName a0);
    void setRegister(byte addr,byte data);
    void reset(void);
    void setFreqency(int ch,word freq);
    void setVolume(int ch,int vol);
    void sendZero(void);
private:
    DigitalOut _wrcs;
    DigitalOut _ic;
    DigitalOut _a0;
    hc595 _shiftRegister;
};
#endif