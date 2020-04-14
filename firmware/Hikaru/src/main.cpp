#include <mbed.h>
#include "hc595.h"
#include "ym2413.h"
#include "ymz294.h"
//-------------------------
hc595::hc595 shiftRegister(PB_5,PB_4,PB_3,PA_12);
ymz294::ymz294 PSG1(shiftRegister,PB_0,PF_1,PA_8);
ymz294::ymz294 PSG2(shiftRegister,PB_7,PF_1,PA_8);
ym2413::ym2413 FM1(shiftRegister,PB_6,PF_1,PA_8);
ym2413::ym2413 FM2(shiftRegister,PB_1,PF_1,PA_8);
ym2413::ym2413 FM3(shiftRegister,PF_0,PF_1,PA_8);
Serial debug(USBTX,USBRX,9600);
Serial midi(PA_9,PA_10,31250);

//-------------------------
int main() {

  // put your setup code here, to run once:

  while(1) {
    // put your main code here, to run repeatedly:
  }
}