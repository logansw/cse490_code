#include <Arduino.h>

/**
 * 
 * Modified code from https://github.com/makeabilitylab/arduino/tree/master/Basics/digitalWrite/BlinkMultipleWithExternalClass
 * 
 */

class Blinker{

  private:
    const int _pin;                 // output pin
    int _state;                     // current state (either HIGH OR LOW)
    unsigned long _lastToggledTimestamp; // last state toggle in ms

  public:
    unsigned long _interval;  // blink interval in ms
    Blinker(const int pin, const unsigned long blinkInterval);
    void update(int strength);
    void setInterval(unsigned long blinkerInterval);
};
