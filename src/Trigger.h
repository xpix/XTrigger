#include <Arduino.h>

class TRIGGER {
  private:
    int _firstpin;
    int _secondpin;
    int _thirdpin;
    int _shotms;
    bool _active;

  public:
    void setup(int firstpin, int secondpin, int thirdpin){
        _firstpin   = firstpin;
        _secondpin  = secondpin;
        _thirdpin   = thirdpin;

        pinMode(_firstpin, OUTPUT);
        pinMode(_secondpin, OUTPUT);
        pinMode(_thirdpin, OUTPUT);
        all(false);
    }

    void shot(int pin, bool state){
        if(state){
            digitalWrite(pin,  HIGH );
        } else
        {
            digitalWrite(pin,  LOW );
        }
    }

    void first(bool state){
        shot(_firstpin, state);
    }

    void second(bool state){
        shot(_secondpin, state);
    }

    void third(bool state){
        shot(_thirdpin, state);
    }

    void all(bool state){
        first(state);
        second(state);
        third(state);
    }
};
