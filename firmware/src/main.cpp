#include <Arduino.h>
#include <SPI.h>

// _    _ _ _______ ___  __  __ __ 
// | |  | (_)__   __/ _ \|  \/  /_ |
// | |__| |_   | | | | | | \  / || |
// |  __  | |  | | | | | | |\/| || |
// | |  | | |  | | | |_| | |  | || |
// |_|  |_|_|  |_|  \___/|_|  |_||_|
// Hardware MIDI Tone Generator Machine "HiT0M1" Firmware                                  
                                  
//-----------------------------------
//ピン名の定義

// 74HC595
#define SRCLK       (PB5)
#define RCLK        (PB2)
#define SER         (PB3)

// 74HC139
#define CS_A        (PC0)
#define CS_B        (PC1)
#define CS_E        (PB0)

#define WR_A        (PC2)
#define WR_B        (PC3)
#define WR_E        (PB1)

// Tone Generator
#define IC_PIN      (PC4)
#define A0_PIN      (PC5)

// Status LEDs
#define PSG1_LED    (PD3)    
#define PSG2_LED    (PD4)
#define PSG3_LED    (PD5)
#define FMR_LED     (PD6)
#define FMM_LED     (PD7)
//-----------------------------------

//-----------------------------------
//チップ選択用の列挙型
enum chipSelect{
  chip_PSG1,
  chip_PSG2,
  chip_PSG3,
  chip_FM,
  chip_NONE
};

//チャンネル選択用の列挙型
enum channelSelect{
  ch_PSG1,
  ch_PSG2,
  ch_PSG3,
  ch_FM_Melody1,
  ch_FM_Melody2,
  ch_FM_Rythm,
  ch_NONE
};

//リズム選択用の列挙型
enum rythmSelect{
  R,
  BD,
  SD,
  TOM,
  TCY,
  HH,
};

//音色選択用の列挙型
enum instrumentSelect{
  ORIGINAL,
  VIOLIN,
  GUITAR,
  PIANO,
  FLUTE,
  CLARINET,
  OBOE,
  TRUMPET,
  ORGAN,
  HORN,
  SYNTH,
  HARP,
  VIBRAPHONE,
  SYNTHBASE,
  WOODBASE,
  ELEGUITAR,
  NONE
};

//音階選択用の列挙型
enum toneSelect{
    C,
    CS,
    D,
    DS,
    E,
    F,
    FS,
    G,
    GS,
    A,
    AS,
    B,
};

//PSGの状態把握用構造体
struct _PSG{
  bool  used[3]             = {false,false,false};
  int   nowFreq[3]          = {0,0,0};
  int   nowVol[3]           = {0,0,0};
};

//FMの状態把握用構造体
struct _FM{
  bool used[3]              = {false,false,false};
  int  nowFnum[3]           = {0,0,0};
  int  nowVol[3]            = {0,0,0};
  instrumentSelect nowInst  = PIANO;
};

struct _TONE{

};
struct _PSG status_PSG1;
struct _PSG status_PSG2;
struct _PSG status_PSG3;
struct _FM  status_FM1;
struct _FM  status_FM2;
byte        nowRythm = 0x00;
//-----------------------------------

//-----------------------------------
void init(void);
void resetAll(void);
void setChipSelect(chipSelect chip);
void setWriteEnable(chipSelect chip);
void sendData(byte data);
void setRegister(chipSelect chip,byte addr,byte data);
void setTonePSG(chipSelect chip,int ch,word freq);
void setToneFM(chipSelect chip,int ch,int fNumber,int oct);
void setFMInstVol(chipSelect chip,int ch,instrumentSelect inst,int vol);
void setPSGVol(channelSelect channel,int ch,int vol);
void setFMRythm(rythmSelect rythm,bool key);
instrumentSelect retInst(byte data);
int retFnum(toneSelect tone);
int retInstNum(instrumentSelect inst);
word retFreq(toneSelect tone,int oct);
toneSelect retTone(byte data);
int retOct(byte data);
//-----------------------------------

//-----------------------------------

//@brief  :初期化関数
//@param  :なし
//@return :なし
void init(void){
  //74HC595関連の初期化
  DDRB=0xF4;
  DDRC=0xFF;
  DDRD=0x7F;
  SPI.begin();
  Serial.begin(31250);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
}

//@brief  :全チップのリセット
//@param  :なし
//@return :なし
void resetAll(void){
  setRegister(chip_PSG1,0x00,0);
  setRegister(chip_PSG1,0x01,0);
  setRegister(chip_PSG1,0x02,0);
  setRegister(chip_PSG1,0x03,0);
  setRegister(chip_PSG1,0x04,0);
  setRegister(chip_PSG1,0x05,0);
  setRegister(chip_PSG1,0x06,0);
  setRegister(chip_PSG1,0x07,0);
  setRegister(chip_PSG1,0x08,0);
  setRegister(chip_PSG1,0x09,0);
  setRegister(chip_PSG1,0x0a,0);
  setRegister(chip_PSG1,0x0b,0);
  setRegister(chip_PSG1,0x0c,0);
  setRegister(chip_PSG1,0x0d,0);

  setRegister(chip_PSG2,0x00,0);
  setRegister(chip_PSG2,0x01,0);
  setRegister(chip_PSG2,0x02,0);
  setRegister(chip_PSG2,0x03,0);
  setRegister(chip_PSG2,0x04,0);
  setRegister(chip_PSG2,0x05,0);
  setRegister(chip_PSG2,0x06,0);
  setRegister(chip_PSG2,0x07,0);
  setRegister(chip_PSG2,0x08,0);
  setRegister(chip_PSG2,0x09,0);
  setRegister(chip_PSG2,0x0a,0);
  setRegister(chip_PSG2,0x0b,0);
  setRegister(chip_PSG2,0x0c,0);
  setRegister(chip_PSG2,0x0d,0);

  setRegister(chip_PSG3,0x00,0);
  setRegister(chip_PSG3,0x01,0);
  setRegister(chip_PSG3,0x02,0);
  setRegister(chip_PSG3,0x03,0);
  setRegister(chip_PSG3,0x04,0);
  setRegister(chip_PSG3,0x05,0);
  setRegister(chip_PSG3,0x06,0);
  setRegister(chip_PSG3,0x07,0);
  setRegister(chip_PSG3,0x08,0);
  setRegister(chip_PSG3,0x09,0);
  setRegister(chip_PSG3,0x0a,0);
  setRegister(chip_PSG3,0x0b,0);
  setRegister(chip_PSG3,0x0c,0);
  setRegister(chip_PSG3,0x0d,0);

  setWriteEnable(chip_NONE);
  PORTC &= ~(_BV(A0_PIN) | _BV(IC_PIN));
  _delay_ms(100);
  PORTC |= _BV(IC_PIN);
  _delay_ms(100);
}

//@brief  :チップを選択する
//@param  :chip(chipSelect)
//@return :なし
void setChipSelect(chipSelect chip){
  switch(chip){
    case chip_PSG1:
      PORTB &= ~(_BV(CS_E));
      PORTC &= ~(_BV(CS_A) | _BV(CS_B));
      break;
    case chip_PSG2:
      PORTB &= ~(_BV(CS_E));
      PORTC &= ~(_BV(CS_B));
      PORTC |= _BV(CS_A);
      break;
    case chip_PSG3:
      PORTB &= ~(_BV(CS_E));
      PORTC |= _BV(CS_B);
      PORTC &= ~(_BV(CS_A));
      break;
    case chip_FM:
      PORTB &= ~(_BV(CS_E));
      PORTC |= (_BV(CS_A) | _BV (CS_B));
      break;
    case chip_NONE:
      PORTB |= _BV(CS_E);
      break;
  }
}

//@brief  :書き込み許可の設定
//@param  :chip(chipSelect)
//@return :なし
void setWriteEnable(chipSelect chip){
  switch(chip){
    case chip_PSG1:
      PORTB &= ~(_BV(WR_E));
      PORTC &= ~(_BV(WR_A) | _BV(WR_B));
      break;
    case chip_PSG2:
      PORTB &= ~(_BV(WR_E));
      PORTC &= ~(_BV(WR_B));
      PORTC |= _BV(WR_A);
      break;
    case chip_PSG3:
      PORTB &= ~(_BV(WR_E));
      PORTC |= _BV(WR_B);
      PORTC &= ~(_BV(WR_A));
      break;
    case chip_FM:
      PORTB &= ~(_BV(WR_E));
      PORTC |= (_BV(WR_A) | _BV (WR_B));
      break;
    case chip_NONE:
      PORTB |= _BV(WR_E);
      break;
  }
}

//@brief  :74HC595にデータを送る
//@param  :data(byte)
//@return :なし
void sendData(byte data){
  SPI.transfer(data);
  PORTB |= _BV(RCLK);
  PORTB &= ~_BV(RCLK);
}

//@brief  :指定したチップ、アドレスのレジスタにデータを書き込む
//@param  :chip(chipSelect),addr(byte),data(byte)
//@return :なし
void setRegister(chipSelect chip,byte addr,byte data){
  PORTC &= ~(_BV(A0_PIN));
  setChipSelect(chip);
  setWriteEnable(chip);
  sendData(addr);
  _delay_us(12);
  setChipSelect(chip_NONE);
  setWriteEnable(chip_NONE);
  setChipSelect(chip);
  setWriteEnable(chip);
  PORTC |= _BV(A0_PIN);
  sendData(data);
  _delay_us(84);
  setChipSelect(chip_NONE);
  setWriteEnable(chip_NONE); 
}

//@brief  :指定チャンネルの音をセットする(PSG)
//@param  :channel(channelSelect),ch(int),freq(word)
//@return :なし
void setTonePSG(chipSelect chip,int ch,word freq){
  word cal_freqency = 0;
  if (freq != 0) {
    cal_freqency = 125000 / freq;
  }
  cal_freqency &= 0b0000111111111111;
  setRegister(chip,0x00 + (ch * 2), cal_freqency & 0xff);
  setRegister(chip,0x01 + (ch * 2), (cal_freqency >> 8) & 0xff);  
}

//@brief  :指定チャンネルの音をセットする(FM)
//@param  :channel(channelSelect),ch(int),fNumber(int),oct(int)
//@return :なし
void setToneFM(chipSelect chip,int ch,int fNumber,int oct){
  // F-Numberの下位8ビット
  setRegister(chip,0x10 + ch, fNumber & 0xff);
  // key=1, oct, F-Numberの上位1ビット
  setRegister(chip,0x20 + ch, (1 << 4) | (oct << 1) | (fNumber >> 8));
}

//@brief  :指定チップの音色と音量を変更する(FM)
//@param  :chip(chipSelect),ch(int),inst(instrumentSelect),vol(int)
//@return :なし
void setFMInstVol(chipSelect chip,int ch,instrumentSelect inst,int vol){
  setRegister(chip,0x30 + ch, (retInstNum(inst) << 4) | vol);
}

//@brief  :指定チャンネルの音量を変更する(PSG)
//@param  :channel(channelSelect),ch(int),inst(instrumentSelect),vol(int)
//@return :なし
void setPSGVol(channelSelect channel,int ch,int vol){
  switch(channel){
    case ch_PSG1:
        setRegister(chip_PSG1,0x08 + ch,vol);
      break;
    case ch_PSG2:
      setRegister(chip_PSG2,0x08 + ch,vol);
      break;
    case ch_PSG3:
      setRegister(chip_PSG3,0x08 + ch,vol);
      break;
  }
}

//@brief  :リズム音を指定する
//@param  :rythm(rythmSelect),key(bool)
//@return :なし
void setFMRythm(rythmSelect rythm,bool key){
  nowRythm |= _BV(6);
  switch(rythm){
    case BD:
      if(key){
        //ON
        nowRythm |= _BV(5);
      } else {
        //OFF
        nowRythm &= ~(_BV(5));
      }
      break;
    case SD:
      if(key){
        //ON
        nowRythm |= _BV(4);
      } else {
        //OFF
        nowRythm &= ~(_BV(4));
      }
      break;
    case TOM:
      if(key){
        //ON
        nowRythm |= _BV(3);
      } else {
        //OFF
        nowRythm &= ~(_BV(3));
      }
      break;
    case TCY:
      if(key){
        //ON
        nowRythm |= _BV(2);
      } else {
        //OFF
        nowRythm &= ~(_BV(2));
      }
      break;
    case HH:
      if(key){
        //ON
        nowRythm |= _BV(1);
      } else {
        //OFF
        nowRythm &= ~(_BV(1));
      }
      break;
  }
  setRegister(chip_FM,0x0E,nowRythm);
  return;
}

//@brief  :MIDIから音色を返す
//@param  :data(byte)
//@return :instrumentSelect
instrumentSelect retInst(byte data){
  switch(data){
    case 0xFF:
      return ORIGINAL;
      break;
    case 0x28:
      return VIOLIN;
      break;
    case 0x18:
      return GUITAR;
      break;
    case 0x00:
      return PIANO;
      break;
    case 0x49:
      return FLUTE;
      break;
    case 0x47:
      return CLARINET;
      break;
    case 0x44:
      return OBOE;
      break;
    case 0x38:
      return TRUMPET;
      break;
    case 0x14:
      return ORGAN;
      break;
    case 0x3C:
      return HORN;
      break;
    case 0x50:
      return SYNTH;
      break;
    case 0x06:
      return HARP;
      break;
    case 0x0B:
      return VIBRAPHONE;
      break;
    case 0x26:
      return SYNTHBASE;
      break;
    case 0x24:
      return WOODBASE;
      break;
    case 0x1B:
      return ELEGUITAR;
      break;

  }
}

//@brief  :音階からF-Numberを返す
//@param  :tone(toneSelect)
//@return :int
int retFnum(toneSelect tone){
  switch(tone){
      case C:
        return 172;
        break;
      case CS:
        return 181;
        break;
      case D:
        return 191;
        break;
      case DS:
        return 204;
        break;
      case E:
        return 216;
        break;
      case F:
        return 229;
        break;
      case FS:
        return 242;
        break;
      case G:
        return 257;
        break;
      case GS:
        return 272;
        break;
      case A:
        return 288;
        break;
      case AS:
        return 305;
        break;
      case B:
        return 323;
        break;
  }
}

//@brief  :音色から送るべきデータを返す
//@param  :tone(toneSelect)
//@return :int
int retInstNum(instrumentSelect inst){
  switch(inst){
    case ORIGINAL:
        return 0x00;
        break;
    case VIOLIN:
        return 0x01;
        break;
    case GUITAR:
        return 0x02;
        break;
    case PIANO:
        return 0x03;
        break;
    case FLUTE:
        return 0x04;
        break;
    case CLARINET:
        return 0x05;
        break;
    case OBOE:
        return 0x06;
        break;
    case TRUMPET:
        return 0x07;
        break;
    case ORGAN:
        return 0x08;
        break;
    case HORN:
        return 0x09;
        break;
    case SYNTH:
        return 0x0A;
        break;
    case HARP:
        return 0x0B;
        break;
    case VIBRAPHONE:
        return 0x0C;
        break;
    case SYNTHBASE:
        return 0x0D;
        break;
    case WOODBASE:
        return 0x0E;
        break;
    case ELEGUITAR:
        return 0x0F;
        break;

  }
}


//@brief  :音階から周波数を返す
//@param  :tone(toneSelect),oct(int)
//@return :word
word retFreq(toneSelect tone,int oct){
  int freq = 0;
  switch(tone){
    case C:
        freq = 263;
        break;
    case CS:
        freq = 277;
        break;
    case D:
        freq = 294;
        break;
    case DS:
        freq = 311;
        break;
    case E:
        freq = 330;
        break;
    case F:
        freq = 349;
        break;
    case FS:
        freq = 370;
        break;
    case G:
        freq = 392;
        break;
    case GS:
        freq = 415;
        break;
    case A:
        freq = 440;
        break;
    case AS:
        freq = 466;
        break;
    case B:
        freq = 494;
        break;
  }
  if(oct >= 4){
    freq *= (oct - 4);
  } else {
    freq /= abs(oct - 4 + 1);
  }
  return freq;
}

//@brief  :MIDIから音階を返す
//@param  :tone(toneSelect)
//@return :int
toneSelect retTone(byte data){
  int base = (int)data % 12;
  switch(base){
    case 0:
        return C;
        break;
    case 1:
        return CS;
        break;
    case 2:
        return D;
        break;
    case 3:
        return DS;
        break;
    case 4:
        return E;
        break;
    case 5:
        return F;
        break;
    case 6:
        return FS;
        break;
    case 7:
        return G;
        break;
    case 8:
        return GS;
        break;
    case 9:
        return A;
        break;
    case 10:
        return AS;
        break;
    case 11:
        return B;
        break;
  }
}

//@brief  :MIDIからオクターブを返す
//@param  :tone(toneSelect)
//@return :int
int retOct(byte data){
  return ((int)data / 12) - 1;
}

rythmSelect retRythm(byte data){
  switch(data){
    case 0x24:
      return BD;
      break;
    case 0x26:
      return SD;
      break;
    case 0x2F:
      return TOM;
      break;
    case 0x31:
      return TCY;
      break;
    case 0x2A:
      return HH;
      break;
  }
}

//-----------------------------------


//-----------------------------------

//@brief  :ノートオン
//@param  :ch(int),note(int)
//@return :なし
void noteOn(int ch,int note){
  int oct = retOct(note);
  toneSelect tone = retTone(note);
  
  switch(ch){
    case 0:
      //PSG1
      if(!(status_PSG1.used[0])){
        //PSG1.A is available
        status_PSG1.used[0] = true;
        int freq = retFreq(tone,oct);
        status_PSG1.nowFreq[0] = freq;
        setTonePSG(chip_PSG1,0,freq);
      } else if(!(status_PSG1.used[1])){
        //PSG1.B is available
        status_PSG1.used[1] = true;
        int freq = retFreq(tone,oct);
        status_PSG1.nowFreq[1] = freq;
        setTonePSG(chip_PSG1,1,freq);
      } else if(!(status_PSG1.used[2])){
        //PSG1.C is available
        status_PSG1.used[2] = true;
        int freq = retFreq(tone,oct);
        status_PSG1.nowFreq[2] = freq;
        setTonePSG(chip_PSG1,2,freq);      
      } else {
        //Any Tone generator is unavailable
        break;
      }
      break;
    case 1:
      //PSG2
      if(!(status_PSG2.used[0])){
        //PSG2.A is available
        status_PSG2.used[0] = true;
        int freq = retFreq(tone,oct);
        status_PSG2.nowFreq[0] = freq;
        setTonePSG(chip_PSG2,0,freq);
      } else if(!(status_PSG2.used[1])){
        //PSG2.B is available
        status_PSG2.used[1] = true;
        int freq = retFreq(tone,oct);
        status_PSG2.nowFreq[1] = freq;
        setTonePSG(chip_PSG2,1,freq);
      } else if(!(status_PSG2.used[2])){
        //PSG2.C is available
        status_PSG3.used[2] = true;
        int freq = retFreq(tone,oct);
        status_PSG2.nowFreq[2] = freq;
        setTonePSG(chip_PSG2,2,freq);      
      } else {
        //Any Tone generator is unavailable
        break;
      }
      break;
    case 2:
      //PSG3
      if(!(status_PSG3.used[0])){
        //PSG3.A is available
        status_PSG3.used[0] = true;
        int freq = retFreq(tone,oct);
        status_PSG3.nowFreq[0] = freq;
        setTonePSG(chip_PSG3,0,freq);
      } else if(!(status_PSG3.used[1])){
        //PSG3.B is available
        status_PSG3.used[1] = true;
        int freq = retFreq(tone,oct);
        status_PSG3.nowFreq[1] = freq;
        setTonePSG(chip_PSG3,1,freq);
      } else if(!(status_PSG3.used[2])){
        //PSG3.C is available
        status_PSG3.used[2] = true;
        int freq = retFreq(tone,oct);
        status_PSG3.nowFreq[0] = freq;
        setTonePSG(chip_PSG3,2,freq);      
      } else {
        //Any Tone generator is unavailable
        break;
      }
      break;
    case 3:
      //FM1(ch0-2)
      if(!(status_FM1.used[0])){
        //FM1.A(ch0) is available
        status_FM1.used[0] = true;
        int fNumber = retFnum(tone);
        status_FM1.nowFnum[0] = fNumber;
        setToneFM(chip_FM,0,fNumber,oct);
      } else if(!status_FM1.used[1]){
        //FM1.B(ch1) is available
        status_FM1.used[1] = true;
        int fNumber = retFnum(tone);
        status_FM1.nowFnum[1] = fNumber;
        setToneFM(chip_FM,1,fNumber,oct);
      } else if(!status_FM1.used[2]){
        //FM1.C(ch2) is available
        status_FM1.used[2] = true;
        int fNumber = retFnum(tone);
        status_FM1.nowFnum[2] = fNumber;
        setToneFM(chip_FM,2,fNumber,oct);
      } else {
        //Any Tone Generator is unavailable
      }
      break;
    case 4:
      //FM2(ch3-5)
      if(!(status_FM2.used[0])){
        //FM2.A(ch3) is available
        status_FM2.used[0] = true;
        int fNumber = retFnum(tone);
        status_FM2.nowFnum[0] = fNumber;
        setToneFM(chip_FM,3,fNumber,oct);
      } else if(!status_FM2.used[1]){
        //FM2.B(ch4) is available
        status_FM2.used[1] = true;
        int fNumber = retFnum(tone);
        status_FM2.nowFnum[1] = fNumber;
        setToneFM(chip_FM,4,fNumber,oct);
      } else if(!status_FM2.used[2]){
        //FM2.C(ch5) is available
        status_FM2.used[2] = true;
        int fNumber = retFnum(tone);
        status_FM2.nowFnum[2] = fNumber;
        setToneFM(chip_FM,5,fNumber,oct);
      } else {
        //Any Tone Generator is unavailable
      }
      break;
    case 9:
      //FM_Rythm
      rythmSelect rythm = retRythm(note);
      setFMRythm(rythm,true);
    default:
      break;
  }
}

//@brief  :ノートオフ
//@param  :ch(int),note(int)
//@return :なし
void noteOff(int ch,int note){

}

//@brief  :プログラムチェンジ
//@param  :ch(int),pNumber(int)
//@return :なし
void programChange(int ch,int pNumber){

}

//@brief  :コントロールチェンジ
//@param  :ch(int),cNumber(int),data(int)
//@return :なし
void controlChange(int ch,int cNumber,int data){

}

//@brief  :MIDIのパース
//@param  :なし
//@return :なし
void parseMIDI(){
  byte data1 = 0xFF;
  byte top;
  byte bottom;
  byte data2 = 0xFF;
  byte data3 = 0xFF;
  while(!(Serial.available()));
  data1 = Serial.read();
  top = data1 >> 4;
  bottom = data1 ^ (top << 4);
  switch(top){
    case 0x08:
      //NoteOFF
      while(!(Serial.available() >= 2));
      data2 = Serial.read();
      data3 = Serial.read();
      noteOff(bottom,data2);
      break;
    case 0x09:
      //NoteON
      while(!(Serial.available() >= 2));
      data2 = Serial.read();
      data3 = Serial.read();
      noteOn(bottom,data2);
      break;
    case 0x0B:
      //ControlChange
      while(!(Serial.available() >= 2));
      data2 = Serial.read();
      data3 = Serial.read();
      controlChange(bottom,data2,data3);
      break;
    case 0x0C:
      //ProgramChange
      while(!(Serial.available() >= 1));
      data2 = Serial.read();
      data3 = Serial.read();
      programChange(bottom,data2);
      break;
  }
}


//@brief  :セットアップ
//@param  :なし
//@return :なし
void setup() {
  init();
  resetAll();
  setRegister(chip_FM,0x0E,0x20);
}

//@brief  :メインループ
//@param  :なし
//@return :なし
void loop() {
  parseMIDI();
}
//-----------------------------------