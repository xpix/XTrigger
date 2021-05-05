#include <Arduino.h>

class SENSOR {
  private:
    int _firstpin;
    int _delay;
    int _treshold;
    int _average;

  public:
    void setup(int firstpin){
        _firstpin   = firstpin;
        pinMode(_firstpin, INPUT);
    }

    // get average enviroment light
    int average(int trys){
        int val = 0;
        for (int i = 0; i < trys; i++) {
            val += analogRead(_firstpin);
            delay(10);
        }
        _average = (val / trys);
        return _average;
    }

    // start sense for light change
    void sense(int treshold, int delay){
        _treshold   = treshold;
        _delay      = delay;
        average(10);
    }

    int sdelay(){
        return _delay;
    }

    /*
        i.e. average is 3000 and threshold 1000
        then it will shot if under 2000 (avg - ths) or over 4000(avg + ths)
    */
    bool tick(){
        int val = analogRead(_firstpin);
        if(_delay > 0){
            if(val < (_average - _treshold)){
                _delay = 0;
                return true;
            }
            else if(val > (_average + _treshold)){
                _delay = 0;
                return true;
            }
        }
        return false;
    }
};
