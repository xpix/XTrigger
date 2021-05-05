#include <Adafruit_NeoPixel.h>

#define PIXEL 0

class WS2812 {
  private:
    int _pin;
    int _milliseconds;
    uint32_t _oldcolor;
    uint32_t _newcolor;
    Adafruit_NeoPixel *pixels;

    void _blink(){
        if( pixels->getPixelColor(PIXEL) == _newcolor){
            color32(_oldcolor);
        } else {
            color32(_newcolor);
        }
    }

  public:
    void setup(int newpin){
        _pin = newpin;
        pixels = new Adafruit_NeoPixel(1, _pin, NEO_GRB + NEO_KHZ800);
        pixels->begin();
    }
    
    void color(uint8_t r, uint8_t g, uint8_t b){
        color32(pixels->Color(r, g, b));
    }

    void color32(uint32_t farbe){
        pixels->setPixelColor(PIXEL, farbe);
        pixels->show();
    }

    // blink in color between last color and this color
    void blink(int milliseconds, uint8_t r, uint8_t g, uint8_t b){
        _milliseconds = milliseconds;
        _oldcolor = pixels->getPixelColor(PIXEL);
        _newcolor = pixels->Color(r, g, b);
    }

    void stop(){
        _milliseconds = 0;
    }

    void tick(){
        if(_milliseconds && millis() % _milliseconds <= 5){
             _blink();
        }
    }

};
