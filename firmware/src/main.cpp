#include <Arduino.h>
#include <SPI.h>
#include "pinConfig.h"

enum chips {
    PSG1,
    PSG2,
    PSG3,
    FM,
    NONE
};

enum tones {
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

enum insts {
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
};

int retFnum(tones tone){
    switch(tone){
        case C:
            return 172;
            break;
        case CS:
            return 181;
            break;
        case D:
            return 192;
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

int retFreq(tones tone){
    switch(tone){
        case C:
            return 262;
            break;
        case CS:
            return 277;
            break;
        case D:
            return 294;
            break;
        case DS:
            return 311;
            break;
        case E:
            return 330;
            break;
        case F:
            return 349;
            break;
        case FS:
            return 370;
            break;
        case G:
            return 392;
            break;
        case GS:
            return 415;
            break;
        case A:
            return 440;
            break;
        case AS:
            return 466;
            break;
        case B:
            return 323;
            break;
    }    
}
void subroutineInit(void){
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK,  OUTPUT);
  pinMode(SER,   OUTPUT);
  pinMode(PSG1_LED, OUTPUT);
  pinMode(PSG2_LED, OUTPUT);
  pinMode(PSG3_LED, OUTPUT);
  pinMode(FMM_LED, OUTPUT);
  pinMode(FMR_LED, OUTPUT);
  pinMode(CS_A, OUTPUT);
  pinMode(CS_B, OUTPUT);
  pinMode(CS_E, OUTPUT);
  pinMode(WR_A, OUTPUT);
  pinMode(WR_B, OUTPUT);
  pinMode(WR_E, OUTPUT);
  pinMode(IC_PIN, OUTPUT);
  pinMode(A0_PIN, OUTPUT);
  SPI.begin();
  Serial.begin(31250);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
}

void dataTransfer(byte data){
    SPI.transfer(data);
    digitalWrite(RCLK, LOW);
    digitalWrite(RCLK, HIGH);
    delay(1);
}

void chipSelect(chips chip){
    switch(chip){
        case PSG1:
            digitalWrite(CS_E,LOW);
            digitalWrite(CS_A,LOW);
            digitalWrite(CS_B,LOW);
            break;
        case PSG2:
            digitalWrite(CS_E,LOW);
            digitalWrite(CS_A,LOW);
            digitalWrite(CS_B,HIGH);
            break;
        case PSG3:
            digitalWrite(CS_E,LOW);
            digitalWrite(CS_A,HIGH);
            digitalWrite(CS_B,LOW);
            break;
        case FM:
            digitalWrite(CS_E,LOW);
            digitalWrite(CS_A,HIGH);
            digitalWrite(CS_B,HIGH);
            break;
        case NONE:
            digitalWrite(CS_E,HIGH);
    }
}

void writeEnable(chips chip){
    switch(chip){
        case PSG1:
            digitalWrite(WR_E,LOW);
            digitalWrite(WR_A,LOW);
            digitalWrite(WR_B,LOW);
            break;
        case PSG2:
            digitalWrite(WR_E,LOW);
            digitalWrite(WR_A,LOW);
            digitalWrite(WR_B,HIGH);
            break;
        case PSG3:
            digitalWrite(WR_E,LOW);
            digitalWrite(WR_A,HIGH);
            digitalWrite(WR_B,LOW);
            break;
        case FM:
            digitalWrite(WR_E,LOW);
            digitalWrite(WR_A,HIGH);
            digitalWrite(WR_B,HIGH);
            break;
        case NONE:
            digitalWrite(WR_E,HIGH);
    }
}

void blinkSignal(chips chip){
    switch(chip){
        case PSG1:
                digitalWrite(PSG1_LED,HIGH);
                delay(1);
                digitalWrite(PSG1_LED,LOW);
            break;
        case PSG2:
                digitalWrite(PSG2_LED,HIGH);
                delay(1);
                digitalWrite(PSG2_LED,LOW);
            break;
        case PSG3:
                digitalWrite(PSG3_LED,HIGH);
                delay(1);
                digitalWrite(PSG3_LED,LOW);
            break;
        default:
            break;
    }
}

void setRegister(chips chip,byte addr,byte data){
    chipSelect(chip);
    writeEnable(chip);
    digitalWrite(A0_PIN,LOW);
    dataTransfer(addr);
    chipSelect(NONE);
    writeEnable(NONE);

    chipSelect(chip);
    writeEnable(chip);
    digitalWrite(A0_PIN,HIGH);
    dataTransfer(data);
    chipSelect(NONE);
    chipSelect(NONE);
}

void sendZero(chips chip){
    setRegister(chip,0x00,0x00);
    setRegister(chip,0x01,0x00);
    setRegister(chip,0x02,0x00);
    setRegister(chip,0x03,0x00);
    setRegister(chip,0x04,0x00);
    setRegister(chip,0x05,0x00);
    setRegister(chip,0x06,0x00);
    setRegister(chip,0x07,0x00);
    setRegister(chip,0x08,0x00);
    setRegister(chip,0x09,0x00);
    setRegister(chip,0x0a,0x00);
    setRegister(chip,0x0b,0x00);
    setRegister(chip,0x0c,0x00);
    setRegister(chip,0x0d,0x00);
}

void reset(void){
    sendZero(PSG1);
    sendZero(PSG2);
    sendZero(PSG3);
    chipSelect(NONE);
    chipSelect(NONE);
    digitalWrite(A0_PIN,LOW);
    digitalWrite(IC_PIN,LOW);
    delay(100);
    digitalWrite(IC_PIN,HIGH);
    delay(1000);
    setRegister(FM,0x0E,32);
    setRegister(FM,0x26,_BV(4));
    setRegister(FM,0x27,_BV(4));
    setRegister(FM,0x28,_BV(4));
}

//set tone
void set(chips chip,int ch, int oct,tones tone) {
    int octTable[] = {-4,-3,-2,-1,1,2,3,4,5,6};
    if(chip == FM){
        int f_number = retFnum(tone);
        setRegister(FM,0x10 + ch, f_number & 0xff);
        setRegister(FM,0x20 + ch, (1 << 4) | (oct << 1) | (f_number >> 8));
    } else {
        word freq = retFreq(tone);
        if(octTable[oct] > 0){
            freq *= octTable[oct];
        } else {
            freq /= abs(octTable[oct]);
        }
        word cal_freqency = 0;
        if (freq != 0) {
            cal_freqency = 125000 / freq;
        }
        cal_freqency &= 0b0000111111111111;
        setRegister(chip,0x00 + (ch * 2), cal_freqency & 0xff);
        setRegister(chip,0x01 + (ch * 2), (cal_freqency >> 8) & 0xff);
    }

}

void setInstVol(chips chip,int ch,insts inst, int vol) {
    if(chip == FM){
        setRegister(FM,0x30 + ch, (int(inst) << 4) | vol);
    } else {
        int addr;
        switch(chip){
            case PSG1:
                addr = 0x08;
                break;
            case PSG2:
                addr = 0x09;
                break;
            case PSG3:
                addr = 0x0A;
        }
        setRegister(chip,addr,vol);
    }
  
}

void notesOn(chips chip,tones tone,int oct){

}

void notesOff(){

}

void controlChange(){

}

void programChange(){

}

void allNotesOff(void){

}

void midiInput(void){
    while(!Serial.available());

    byte data1 = Serial.read();
    byte data2 = 0x00;
    byte data3 = 0x00;

    if(data1 >= 0x80 && data1 <= 0x89){
        //NoteOff
        while(!(Serial.available() >= 2));
        data2 = Serial.read();
        data3 = Serial.read();
    } else if(data1 >= 0x90 && data1 <= 0x99){
        //NoteOn
        while(!(Serial.available() >= 2));
        data2 = Serial.read();
        data3 = Serial.read();
    } else if(data1 >= 0xB0 && data1 <= 0xB4){
        //ControlChange
        while(!(Serial.available() >= 2));
        data2 = Serial.read();
        data3 = Serial.read();       
    } else if(data1 == 0xC3){
        //ProgramChange(FM Only)
        while(!(Serial.available() >= 1));
        data2 = Serial.read();
    } else if(data1 >= 0xB0 && data1 <= 0xB9){
        //AllNotesOff
        while(!(Serial.available() >= 2));
        data2 = Serial.read();
        data3 = Serial.read();
        if(data2 == 0x7B && data3 == 0x00){

        }

    } else {
        return;
    }

}
void setup() {
  // put your setup code here, to run once:
  reset();
  subroutineInit();
}

void loop() {
  // put your main code here, to run repeatedly:
}