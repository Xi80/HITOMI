#include <ymz294.h>

ymz294::ymz294(hc595::hc595 shiftRegister,PinName wrcs,PinName ic,PinName a0) : _shiftRegister(shiftRegister), _wrcs(wrcs), _ic(ic), _a0(a0){
    //doNothing    
}

void ymz294::setRegister(byte addr,byte data){
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

//2個以上つなぐとこれ使えない
void ymz294::reset(void){
    for(int i = 0;i < 14;i++){
        setRegister(i,0x00);
    }
    _wrcs   = 1;
    _a0     = 0;
    _ic     = 0;
    wait_ms(10);
    _ic     = 1;
    return;
}

void ymz294::setFreqency(int ch,word freq){
    word calFreqency = 0;
    if(freq){
        calFreqency = 125000 / freq;
    }
    calFreqency &= 0xFFF;
    ymz294::setRegister(0x00 + (ch << 1),calFreqency & 0xFF);
    ymz294::setRegister(0x01 + (ch << 1),(calFreqency >> 8) & 0xFF);
    return;
}

void ymz294::setVolume(int ch,int vol){
    if(vol >=0 && vol <= 16){
        ymz294::setRegister((0x08 + ch), vol);
    } else {
        return;
    }
};