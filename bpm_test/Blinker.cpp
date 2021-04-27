#include "Blinker.h"

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
void Blinker::update() {
  unsigned long currentTimestampMs = millis();

  if (currentTimestampMs - _lastToggledTimestamp >= _interval) {
    _lastToggledTimestamp = currentTimestampMs;
    _state = !_state;
    digitalWrite(_pin, _state);
  }
}
