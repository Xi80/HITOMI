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

void setFnum(int ch, int oct,tones tone) {
  int f_number = retFnum(tone);
  setRegister(FM,0x10 + ch, f_number & 0xff);
  setRegister(FM,0x20 + ch, (1 << 4) | (oct << 1) | (f_number >> 8));
}

void SetInstVol(int ch, int inst, int vol) {
  setRegister(FM,0x30 + ch, (inst << 4) | vol);
}

void setup() {
  // put your setup code here, to run once:
  reset();
  subroutineInit();
}

void loop() {
  // put your main code here, to run repeatedly:
}