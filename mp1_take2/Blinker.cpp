#include "Blinker.h"

/**
 * 
 * Modified code from https://github.com/makeabilitylab/arduino/tree/master/Basics/digitalWrite/BlinkMultipleWithExternalClass
 * 
 */

Blinker::Blinker(const int pin, unsigned long blinkInterval) :
  _pin(pin) // initialize const like this in C++
{
  _interval = blinkInterval;
  _state = LOW;
  _lastToggledTimestamp = 0;
  pinMode(_pin, OUTPUT);
}

void Blinker::setInterval(unsigned long blinkInterval) {
  _interval = blinkInterval;
}

/**
* Calculates whether to toggle output state based on the set interval
* Call this function once per loop()
*/
void Blinker::update(int strength) {
  unsigned long currentTimestampMs = millis();
  unsigned long flashDuration = 200;

  if (_state == HIGH && currentTimestampMs - _lastToggledTimestamp >= (_interval - flashDuration)) {
    _lastToggledTimestamp = currentTimestampMs;
    _state = !_state;
    analogWrite(_pin, strength);
  } else if (_state == LOW && currentTimestampMs - _lastToggledTimestamp >= flashDuration) {
    _lastToggledTimestamp = currentTimestampMs;
    _state = !_state;
    analogWrite(_pin, 0);
  }
}
