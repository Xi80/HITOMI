#include"ym2413.h"

ym2413::ym2413(hc595::hc595 shiftRegister,PinName wrcs,PinName ic,PinName a0): _shiftRegister(shiftRegister), _wrcs(wrcs), _ic(ic), _a0(a0){
    //doNothing
}

void ym2413::setRegister(byte addr,byte data){
    //sendAddr
    _wrcs   = 0;
    _a0     = 0;
    _shiftRegister.setData(addr);
    _wrcs   = 1;

    //sendData
    _wrcs   = 0;
    _a0     = 1;
    _shiftRegister.setData(data);
    _wrcs   = 1;
    return;
}

//こっちは多分複数つないでも動く
void ym2413::reset(void){
    _wrcs   = 0;
    _ic     = 0;
    wait_ms(100);
    _ic     =1;
    wait_ms(1000);
    return;
}

void ym2413::setInstVolume(int ch,byte inst,int vol){
    ym2413::setRegister((0x30 | ch),(inst << 4) | vol);
    return;
};

void ym2413::setFnumber(int ch,int oct,int f_number){
    ym2413::setRegister((0x10 | ch),f_number & 0xFF);
    return;
}

void ym2413::setDrums(byte data){
    ym2413::setRegister(0x0E,(1 << 5) | data);
    return;
}