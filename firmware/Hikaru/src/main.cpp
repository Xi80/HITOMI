#include <mbed.h>
#include "hc595.h"
#include "ym2413.h"
#include "ymz294.h"
#include "vals.h"

//-------------------------
hc595 shiftRegister(PB_5,PB_4,PB_3,PA_12);
ymz294 PSG1(shiftRegister,PB_0,PF_1,PA_8);
ymz294 PSG2(shiftRegister,PB_7,PF_1,PA_8);
ym2413 FM1(shiftRegister,PB_6,PF_1,PA_8);
ym2413 FM2(shiftRegister,PB_1,PF_1,PA_8);
ym2413 FM3(shiftRegister,PF_0,PF_1,PA_8);
Serial midi(PA_9,PA_10,31250);

//-------------------------
_PSG status_PSG1;
_PSG status_PSG2;
_FM  status_FM1;
_FM  status_FM2;
_FM  status_FM3;
_MIDI status_MIDI;
byte drum = 0x20;
//-------------------------

//ノートオン
void noteOn(int ch,int note){
  if(ch < 15 && ch != 9){
    int f_number = f_numbers[note];
    int oct      = octs[note];
    for(int i = 0;i < 9;i++){
      if(status_FM1.used[i] == false){
        status_FM1.used[i] = true;
        status_FM1.note[i] = note;
        status_FM1.ch[i]   = ch;
        FM1.setFnumber(i,oct,f_number,true);
      }
    }
    for(int i = 0;i < 9;i++){
      if(status_FM2.used[i] == false){
        status_FM2.used[i] = true;
        status_FM2.note[i] = note;
        status_FM2.ch[i]   = ch;
        FM2.setFnumber(i,oct,f_number,true);
      }
    }
    for(int i = 0;i < 6;i++){
      if(status_FM3.used[i] == false){
        status_FM3.used[i] = true;
        status_FM3.note[i] = note;
        status_FM3.ch[i]   = ch;
        FM3.setFnumber(i,oct,f_number,true);
      }
    }
    return;
  } else if(ch == 9){
    switch(note){
      case 0x24:
        //BASS DRUM
        drum |= (1 << 4);
        break;
      case 0x26:
        //SNARE DRUM
        drum |= (1 << 3);
        break;
      case 0x2F:
        //TOM TOM
        drum |= (1 << 2);
        break;
      case 0x31:
        //TOP CYMBAL
        drum |= (1 << 1);
        break;
      case 0x2C:
        //HI-HAT
        drum |= 1;
        break;
      case 0x2E:
        //HI_HAT
        drum |= 1;
        break;
      default:
        break;
    }
    FM3.setDrums(drum);
  } else {
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

//ノートオフ
void noteOff(int ch,int note){
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
    switch(note){
      case 0x24:
        //BASS DRUM
        drum &= ~(1 << 4);
        break;
      case 0x26:
        //SNARE DRUM
        drum &= ~(1 << 3);
        break;
      case 0x2F:
        //TOM TOM
        drum &= ~(1 << 2);
        break;
      case 0x31:
        //TOP CYMBAL
        drum &= ~(1 << 1);
        break;
      case 0x2C:
        //HI-HAT
        drum &= ~1;
        break;
      case 0x2E:
        //HI_HAT
        drum &= ~1;
        break;
      default:
        break;
    }
    FM3.setDrums(drum);
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

//コントロールチェンジ
void controlChange(int ch,int number,byte data)
{
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

//プログラムチェンジ
void programmeChange(int ch,int number){
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

//全ICのリセット
void resetAll(void){
  PSG1.sendZero();
  PSG2.sendZero();
  FM1.reset();
  wait_ms(100);
  FM3.setRegister(0x16,0x20);
  FM3.setRegister(0x17,0x50);
  FM3.setRegister(0x18,0xC0);
  FM3.setRegister(0x26,0x05);
  FM3.setRegister(0x27,0x05);
  FM3.setRegister(0x28,0x51);
  return;
}

//MIDIの解析
void midiParse(void){
  while(!midi.readable());
  byte data1  = midi.getc();
  byte data2  = 0xFF;
  byte data3  = 0xFF;
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