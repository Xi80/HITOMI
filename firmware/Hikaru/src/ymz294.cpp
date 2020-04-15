#include <ymz294.h>

//@brief  :コンストラクタ
//@param  :シフトレジスタ(shiftRegister...hc595)
//@param  :各ピン(wrcs,ic,a0...PinName)
//@return :なし(void)
ymz294::ymz294(hc595 shiftRegister,PinName wrcs,PinName ic,PinName a0) : _shiftRegister(shiftRegister), _wrcs(wrcs), _ic(ic), _a0(a0){   
    _wrcs = 1;
}

//@brief  :指定アドレスに指定データを書き込む
//@param  :対象アドレス(addr...byte)
//@param  :データ(data...byte)
//@return :なし(void)
void ymz294::setRegister(byte addr,byte data){
    //アドレス
    _wrcs   = 0;
    _a0     = 0;
    _shiftRegister.setData(addr);
    _wrcs   = 1;
    //データ
    _wrcs   = 0;
    _a0     = 1;
    _shiftRegister.setData(data);
    _wrcs   = 1;
    return;
}

//@brief  :単一使用時限定のリセット
//@param  :なし(void)
//@return :なし(void)
void ymz294::reset(void){
    for(int i = 0;i < 14;i++){
        setRegister(i,0x00);
    }
    _wrcs   = 1;
    _a0     = 0;
    _ic     = 0;
    thread_sleep_for(10);
    _ic     = 1;
    return;
}

//@brief  :指定周波数にセットする
//@param  :対象チャンネル(ch...int)
//@param  :周波数(freq...word)
//@return :なし(void)
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

//@brief  :音量の変更
//@param  :対象チャンネル(ch...int)
//@param  :音量(vol...int)
//@return :なし(void)
void ymz294::setVolume(int ch,int vol){
    if(vol >=0 && vol <= 16){
        ymz294::setRegister((0x08 + ch), vol);
    } else {
        return;
    }
}

//@brief  :全レジスタに0を書き込む、一括リセットの下準備
//@param  :なし(void)
//@return :なし(void)
void ymz294::sendZero(void){
    for(int i = 0;i < 14;i++){
        setRegister(i,0x00);
    }
    _a0 = 0;
}

