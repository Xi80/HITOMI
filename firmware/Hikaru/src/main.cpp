#include <mbed.h>
#include "hc595.h"
#include "ym2413.h"
#include "ymz294.h"
#include "vals.h"

//  _ _ _    _ _ _                   _ _                                                                                     
// ( | ) |  | (_) |                 ( | )                                                                                    
//  V V| |__| |_| | ____ _ _ __ _   _V V                                                                                     
//     |  __  | | |/ / _` | '__| | | |                                                                                       
//     | |  | | |   < (_| | |  | |_| |                                                                                       
//  ___|_|_ |_|_|_|\_\__,_|_|__ \__,_|                                          __   _ _ _    _ _ _______ ___  __  __ __ _ _ 
// |__   __| |          |  ____(_)                                             / _| ( | ) |  | (_)__   __/ _ \|  \/  /_ ( | )
//    | |  | |__   ___  | |__   _ _ __ _ __ _____      ____ _ _ __ ___    ___ | |_   V V| |__| |_   | | | | | | \  / || |V V 
//    | |  | '_ \ / _ \ |  __| | | '__| '_ ` _ \ \ /\ / / _` | '__/ _ \  / _ \|  _|     |  __  | |  | | | | | | |\/| || |    
//    | |  | | | |  __/ | |    | | |  | | | | | \ V  V / (_| | | |  __/ | (_) | |       | |  | | |  | | | |_| | |  | || |    
//    |_|  |_| |_|\___| |_|    |_|_|  |_| |_| |_|\_/\_/ \__,_|_|  \___|  \___/|_|       |_|  |_|_|  |_|  \___/|_|  |_||_|    
//    Version 1.56-A
//    Written by Ivory Aveline Haskell
//    15/04/2020
//-------------------------

hc595 shiftRegister(PB_5,PB_4,PB_3,PA_11);
ymz294 PSG1(shiftRegister,PA_12,PA_2,PA_7);
ymz294 PSG2(shiftRegister,PB_0,PA_2,PA_7);
ym2413 FM1(shiftRegister,PB_7,PA_2,PA_7);
ym2413 FM2(shiftRegister,PB_6,PA_2,PA_7);
ym2413 FM3(shiftRegister,PF_1,PA_2,PA_7);
Serial midi(PA_9,PA_10,31250);
Serial pc(USBTX,USBRX,9600);
//-------------------------

_PSG status_PSG1;
_PSG status_PSG2;
_FM  status_FM1;
_FM  status_FM2;
_FM  status_FM3;
_MIDI status_MIDI;
byte drum = 0x20;

//-------------------------

//@brief  :ドラム用レジスタにおける対象のビット位置を返す
//@param  :ノートナンバー(data...int)
//@return :ビット位置(int)
int retDrums(int data){
  switch(data){
    case 0x24:
      //BASS DRUM
      return 4;
      break;
    case 0x26:
      //SNARE DRUM
      return 3;
      break;
    case 0x2F:
      //TOM TOM
      return 2;
      break;
    case 0x31:
      //TOP CYMBAL
      return 1;
      break;
    case 0x2C:
      //HI-HAT
      return 0;
      break;
    case 0x2E:
      //HI_HAT
      return 0;
      break;
    default:
      break;
    }
    return -1;
}

//@brief  :ノートオンに対する処理
//@param  :対象チャンネル(ch...int)
//@param  :ノートナンバー(note...int)
//@return :なし(void)
void noteOn(int ch,int note){
  if(ch < 15 && ch != 9){
    //FMメロディー音源の担当チャンネルは1~9,11~14
    //定数テーブルからFNumberとオクターブを求める
    int f_number = f_numbers[note];
    int oct      = octs[note];

    //空きスロットを探す
    //見つかった場合、状態を使用中にした上で、該当チャンネルの情報に置き換え発音させる
    for(int i = 0;i < 9;i++){
      //FM.1
      if(status_FM1.used[i] == false){
        status_FM1.used[i] = true;
        status_FM1.note[i] = note;
        status_FM1.ch[i]   = ch;
        FM1.setFnumber(i,oct,f_number,true);
      }
    }
    for(int i = 0;i < 9;i++){
      //FM.2
      if(status_FM2.used[i] == false){
        status_FM2.used[i] = true;
        status_FM2.note[i] = note;
        status_FM2.ch[i]   = ch;
        FM2.setFnumber(i,oct,f_number,true);
      }
    }
    for(int i = 0;i < 6;i++){
      //FM.3
      if(status_FM3.used[i] == false){
        status_FM3.used[i] = true;
        status_FM3.note[i] = note;
        status_FM3.ch[i]   = ch;
        FM3.setFnumber(i,oct,f_number,true);
      }
    }
    return;
  } else if(ch == 9){
    //ドラムパートの処理
    if(retDrums(note) != -1){
      drum |= (1 << retDrums(note));
      FM3.setDrums(drum);
    }
  } else {
    //PSG担当ch(15,16)の処理
    int freq = freqs[note];
    for(int i = 0;i < 3;i++){
      if(status_PSG1.used[i] == false){
        status_PSG1.used[i] = true;
        status_PSG1.ch[i] =ch;
        status_PSG1.note[i] = note;
        PSG1.setFreqency(i,freq);
        PSG1.setVolume(i,status_PSG1.vol[i]);
      }
    }
    for(int i = 0;i < 3;i++){
      if(status_PSG2.used[i] == false){
        status_PSG2.used[i] = true;
        status_PSG2.ch[i] =ch;
        status_PSG2.note[i] = note;
        PSG2.setFreqency(i,freq);
        PSG2.setVolume(i,status_PSG1.vol[i]);
      }
    }
  }

}

//@brief  :ノートオフに対する処理
//@param  :対象チャンネル(ch...int)
//@param  :ノートナンバー(note...int)
//@return :なし(void)
void noteOff(int ch,int note){
  //ほぼnoteOnと同じであるため説明はそちらを参照
  //探す際の条件が「使用中 && 対象チャンネルである && 対象のノート番号」となっていること以外は同じである
  if(ch < 15 && ch != 9){
    int f_number = f_numbers[note];
    int oct      = octs[note];
    for(int i = 0;i < 9;i++){
      if(status_FM1.used[i] == true && status_FM1.ch[i] == ch && status_FM1.note[i] == note){
        status_FM1.used[i] = false;
        status_FM1.note[i] = note;
        status_FM1.ch[i]   = ch;
        FM1.setFnumber(i,oct,f_number,false);
      }
    }
    for(int i = 0;i < 9;i++){
      if(status_FM2.used[i] == true && status_FM2.ch[i] == ch && status_FM2.note[i] == note){
        status_FM2.used[i] = false;
        status_FM2.note[i] = note;
        status_FM2.ch[i]   = ch;
        FM2.setFnumber(i,oct,f_number,false);
      }
    }
    for(int i = 0;i < 6;i++){
      if(status_FM3.used[i] == true && status_FM3.ch[i] == ch && status_FM3.note[i] == note){
        status_FM3.used[i] = false;
        status_FM3.note[i] = note;
        status_FM3.ch[i]   = ch;
        FM3.setFnumber(i,oct,f_number,false);
      }
    }
    return;
  } else if(ch == 9){
    if(retDrums(note) != -1){
      drum &= ~(1 << retDrums(note));
      FM3.setDrums(drum);
    }
  } else {
    for(int i = 0;i < 3;i++){
      if(status_PSG1.used[i] == true && status_PSG1.ch[i] == true && status_PSG1.note[i] == note){
        status_PSG1.used[i] = false;
        PSG1.setVolume(ch,0);
      }
    }
    for(int i = 0;i < 3;i++){
      if(status_PSG2.used[i] == true && status_PSG2.ch[i] == true && status_PSG2.note[i] == note){
        status_PSG2.used[i] = false;
        PSG2.setVolume(ch,0);
      }
    }
  }
}

//@brief  :コントロールチェンジの処理
//@param  :対象チャンネル(ch...int)
//@param  :コントロール番号(number...int)
//@param  :データ(data...byte)
//@return :なし(void)
void controlChange(int ch,int number,byte data)
{
  //現状エクスプレッションとチャンネルボリュームしか実装していない。
  if(ch < 15){
    if(number == 0x07 || number == 0x0B){
      status_MIDI.exp[ch] = (data >> 3);
      for(int i = 0;i < 9;i++){
        if(status_FM1.ch[i] == ch){
          status_FM1.vol[i] = status_MIDI.exp[ch];
          FM1.setInstVolume(i,status_FM1.inst[i],status_FM1.vol[i]);
        }
      }
      for(int i = 0;i < 9;i++){
        if(status_FM2.ch[i] == ch){
          status_FM2.vol[i] = status_MIDI.exp[ch];
          FM1.setInstVolume(i,status_FM2.inst[i],status_FM2.vol[i]);
        }
      }
      for(int i = 0;i < 6;i++){
        if(status_FM3.ch[i] == ch){
          status_FM3.vol[i] = status_MIDI.exp[ch];
          FM1.setInstVolume(i,status_FM3.inst[i],status_FM3.vol[i]);
        }
      }
    }
  } else if(ch == 9){
    byte drum_vol = (data >> 3);
    FM3.setRegister(0x36,drum_vol);
    FM3.setRegister(0x37,(drum_vol << 4) | drum_vol);
    FM3.setRegister(0x38,(drum_vol << 4) | drum_vol);
  } else {
    if(number == 0x07 || number == 0x0B){
      status_MIDI.exp[ch] = (data >> 3);
      for(int i = 0;i < 3;i++){
        if(status_PSG1.ch[i] == ch){
          status_PSG1.vol[i] = status_MIDI.exp[ch];
          PSG1.setVolume(i,status_PSG1.vol[i]);
        }
      }
      for(int i = 0;i < 3;i++){
        if(status_PSG2.ch[i] == ch){
          status_PSG2.vol[i] = status_MIDI.exp[ch];
          PSG2.setVolume(i,status_PSG2.vol[i]);
        }
      }
    }
  }

}

//@brief  :プログラムチェンジの処理尾
//@param  :対象チャンネル(ch...int)
//@param  :プログラム番号(number...int)
//@return :なし(void)
void programmeChange(int ch,int number){
  //言わずもがなFM専用である。
  status_MIDI.inst[ch] = insts[number];
  for(int i = 0;i < 9;i++){
    if(status_FM1.ch[i] == ch){
      status_FM1.inst[i] = status_MIDI.inst[ch];
      FM1.setInstVolume(i,status_FM1.inst[i],status_FM1.vol[i]);
    }
  }
  for(int i = 0;i < 9;i++){
    if(status_FM2.ch[i] == ch){
      status_FM2.inst[i] = status_MIDI.inst[ch];
      FM1.setInstVolume(i,status_FM2.inst[i],status_FM2.vol[i]);
    }
  }
  for(int i = 0;i < 6;i++){
    if(status_FM3.ch[i] == ch){
      status_FM3.inst[i] = status_MIDI.inst[ch];
      FM1.setInstVolume(i,status_FM3.inst[i],status_FM3.vol[i]);
    }
  }
  return;
}

//-------------------------

//@brief  :リセット
//@param  :なし(void)
//@return :なし(void)
void resetAll(void){
  PSG1.sendZero();
  PSG2.sendZero();
  FM1.reset();
  thread_sleep_for(100);
  //リズム音源のセットアップ
  FM3.setRegister(0x16,0x20);
  FM3.setRegister(0x17,0x50);
  FM3.setRegister(0x18,0xC0);
  FM3.setRegister(0x26,0x05);
  FM3.setRegister(0x27,0x05);
  FM3.setRegister(0x28,0x51);
  return;
}

//@brief  :MIDIのパース
//@param  :なし(void)
//@return :なし(void)
void midiParse(void){
  //データを読んで適切な関数に適切な値を渡す
  while(!midi.readable());
  byte data1  = midi.getc();
  byte data2  = 0xFF;
  byte data3  = 0xFF;
  //命令部分と対象チャンネルを分ける
  byte op     = data1 >> 4;
  byte ch     = data1 ^ (op << 4);
  switch(op){
    case 0x08:
      while(!midi.readable());
      data2 = midi.getc();
      while(!midi.readable());
      data3 = midi.getc();
      noteOff(ch,data2);
      break;
    case 0x09:
      while(!midi.readable());
      data2 = midi.getc();
      while(!midi.readable());
      data3 = midi.getc();
      noteOn(ch,data2);
      break;
    case 0x0B:
      while(!midi.readable());
      data2 = midi.getc();
      while(!midi.readable());
      data3 = midi.getc();
      controlChange(ch,data2,data3);
      break;
    case 0x0C:
      while(!midi.readable());
      data2 = midi.getc();
      programmeChange(ch,data2);
      break;
    default:
      break;
  }
}

//-------------------------

int main() {
  resetAll();
  while(1) {
    midiParse();
  }
}