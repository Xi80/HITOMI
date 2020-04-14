#include"ym2413.h"

ym2413::ym2413(hc595 shiftRegister,PinName wrcs,PinName ic,PinName a0): _shiftRegister(shiftRegister), _wrcs(wrcs), _ic(ic), _a0(a0){
    _wrcs = 1;
}

//レジスターのセット
void ym2413::setRegister(byte addr,byte data){
    _wrcs   = 0;
    _a0     = 0;
    _shiftRegister.setData(addr);
    _wrcs   = 1;

    _wrcs   = 0;
    _a0     = 1;
    _shiftRegister.setData(data);
    _wrcs   = 1;
    return;
}

//リセット、複数ICつないでも動く
void ym2413::reset(void){
    _wrcs   = 0;
    _ic     = 0;
    thread_sleep_for(100);
    _ic     =1;
    thread_sleep_for(1000);
    return;
}

//音色と音量の変更
void ym2413::setInstVolume(int ch,byte inst,int vol){
    ym2413::setRegister((0x30 | ch),(inst << 4) | vol);
    return;
};

//音の変更
void ym2413::setFnumber(int ch,int oct,int f_number,bool key){
    ym2413::setRegister((0x10 | ch),f_number & 0xFF);
    ym2413::setRegister((0x20 | ch),(key << 4) | (oct << 1) | (f_number >> 8));
    return;
}

//リズム音源の制御
void ym2413::setDrums(byte data){
    ym2413::setRegister(0x0E,(1 << 5) | data);
    return;
}
