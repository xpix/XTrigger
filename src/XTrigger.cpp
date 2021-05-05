#include <Arduino.h>

/*
https://oshwlab.com/xpixer/xtrigger

*/

// Own Libs
#include "WS2812.h"
#include "Trigger.h"
#include "Sensor.h"

// external libs
#include <AsyncTimer.h>             // https://github.com/Aasim-A/AsyncTimer
#include <Button2.h>               // https://github.com/LennartHennigs/Button2


// XTrigger Settings
#define FIRST   27 // 37
#define SECOND  26 // 38 at ESP32 35 - 38 just input pins
#define THIRD   25 // 36
#define SHOTMS  20 // shot in milliseconds
#define SHOTOFF 5  // pause after shot to switch pin off

// Button settings
#define BCenter 17
#define BLeft   2
#define BRight  15
#define BDown   13
#define BUp     12

// microphone pin
#define MIC     32

// photo transistor
#define PTR     33

// Init objects
AsyncTimer          timer;
WS2812              rgbled;
TRIGGER             trigger;
SENSOR              sensor;

Button2 center  = Button2(BCenter);
Button2 left    = Button2(BLeft);
Button2 right   = Button2(BRight);
Button2 down    = Button2(BDown);
Button2 up      = Button2(BUp);



// set trigger pins on and wait SHOTOFF 
// milliseconds to switch pin to off
void shot(int shot_delay){

  // add blinking red light until shot and switch to green

  timer.setTimeout([&]() { 
    trigger.all(true);

    timer.setTimeout([&]() { 
      trigger.all(false);
    }, SHOTOFF);

  }, shot_delay);
}

void tap(Button2& btn){
  if (btn == center) {
    Serial.println("center clicked");
  } else if (btn == left) {
    Serial.println("left clicked");
  } else {
    // ....
  }
}


void setup()
{
    Serial.begin(9600); /* prepare for possible serial debug */
    Serial.println("XTrigger 0.01 (c) Frank Herrmann / xpix");

    // micro
    //pinMode(MIC, INPUT);
    //analogSetPinAttenuation(MIC, ADC_11db);

    // WS2812
    rgbled.setup(27);
    rgbled.color(0,10,0); // set color
    //delay(1000);
    //rgbled.blink(100, 100, 0, 0); // blink with 100ms pause between last color and this color

    // Trigger
    trigger.setup(FIRST, SECOND, THIRD);

    // Sensor
    sensor.setup(PTR);        // set phototransistor pin

    // Timer
    timer.setup();
    timer.setInterval([&]() { 
      Serial.print("average: ");Serial.println(sensor.average(10));
      sensor.sense(1000, 50);    // sense for value < 100 and wait 50ms to shot
    }, 3000);

    // Buttons
    center.setTapHandler(tap);
    left.setTapHandler(tap);
    right.setTapHandler(tap);
    down.setTapHandler(tap);
    up.setTapHandler(tap);

}


void loop()
{
  timer.handle();
  rgbled.tick();

  // Check if sense on and return true if triggered
  // then wait sensor.delay and trigger outputs
  if( sensor.tick() == true ){
    shot( sensor.sdelay() ); 
  }

  if (Serial.available() > 0) {
    int inByte = Serial.read();

    switch (inByte) {
      case 'o':
        shot(10);
        Serial.println("All on");
        break;
      case 'f':
        trigger.all(false);
        Serial.println("All off");
        break;
      default:
        Serial.println("Don't understand command");
    }
  }

  center.loop();
  left.loop();
  right.loop();
  down.loop();
  up.loop();
}
