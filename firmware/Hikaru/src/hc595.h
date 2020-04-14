#ifndef H_HC595
#define H_HC595
#include "vals.h"
#include <mbed.h>
#define byte uint8_t

/*
 *  74HC595用のライブラリ
 *  setDataでデータを転送する。
 *  それだけ。
*/ 

class hc595 {
public:
    hc595(PinName mosi,PinName miso,PinName sck,PinName rclk);
    void setData(byte data);
private:
    SPI _spi;
    DigitalOut _rclk;
};
#endif