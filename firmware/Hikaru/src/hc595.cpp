#include "hc595.h"

//@brief  :コンストラクタ
//@param  :各ピン(mosi,miso,sck,rclk...PinName)
//@return :なし(void)
hc595::hc595(PinName mosi,PinName miso,PinName sck,PinName rclk) : _spi(mosi,miso,sck), _rclk(rclk) {
    //SPIの初期化
    _spi.format(8,0);
    _spi.frequency(1000000);
    return;
}

//@brief  :74HC595へのデータ転送
//@param  :転送データ(data...byte)
//@return :なし
void hc595::setData(byte data){
    //データ転送
    _spi.write(data);
    //ラッチ
    _rclk = 1;
    _rclk = 0;
    return;
}