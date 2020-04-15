#include"ym2413.h"

//@brief  :コンストラクタ
//@param  :シフトレジスタ(shiftRegister...hc595)
//@param  :各ピン(wrcs,ic,a0)
//@return :なし(void)
ym2413::ym2413(hc595 shiftRegister,PinName wrcs,PinName ic,PinName a0): _shiftRegister(shiftRegister), _wrcs(wrcs), _ic(ic), _a0(a0){
    //チップセレクトをHIGHに
    _wrcs = 1;
}

//@brief  :レジスタの指定アドレスに指定データを書き込む
//@param  :アドレス(addr...byte)
//@param  :データ(data...byte)
//@return :なし(void)
void ym2413::setRegister(byte addr,byte data){
    //アドレスを書き込む
    _wrcs   = 0;
    _a0     = 0;
    _shiftRegister.setData(addr);
    _wrcs   = 1;
    //データを書き込む
    _wrcs   = 0;
    _a0     = 1;
    _shiftRegister.setData(data);
    _wrcs   = 1;
    return;
}

//@brief  :リセット
//@param  :なし(void)
//@return :なし(void)
void ym2413::reset(void){
    _wrcs   = 0;
    _ic     = 0;
    thread_sleep_for(100);
    _ic     =1;
    thread_sleep_for(1000);
    return;
}

//@brief  :音色と音量の変更
//@param  :対象チャンネル(ch...int)
//@param  :音色(inst...byte)
//@param  :音量(vol...int)
//@return :なし(void)
void ym2413::setInstVolume(int ch,byte inst,int vol){
    ym2413::setRegister((0x30 | ch),(inst << 4) | vol);
    return;
};

//@brief  :音階の変更
//@param  :対象チャンネル(ch...int)
//@param  :オクターブ(oct...int)
//@param  :FNumber(f_number...int)
//@param  :オンオフ(key...bool)
//@return :なし(void)
void ym2413::setFnumber(int ch,int oct,int f_number,bool key){
    ym2413::setRegister((0x10 | ch),f_number & 0xFF);
    ym2413::setRegister((0x20 | ch),(key << 4) | (oct << 1) | (f_number >> 8));
    return;
}

//@brief  :リズム音源制御
//@param  :ドラムデータ(data...byte)
//@return :なし(void)
void ym2413::setDrums(byte data){
    ym2413::setRegister(0x0E,(1 << 5) | data);
    return;
}
