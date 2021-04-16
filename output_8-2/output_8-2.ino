#include "Blinker.h"

Blinker _led1Blinker(2, 200);  // specify pin and blink interval (200ms)
Blinker _led2Blinker(5, 333);  // specify pin and blink interval (333ms)
Blinker _led3Blinker(9, 1111); // specify pin and blink interval (1111ms)

// The setup function runs once when you press reset or power the board
void setup() {
  // empty
}

// The loop function runs over and over again forever
void loop() {
  _led1Blinker.update();
  _led2Blinker.update();
  _led3Blinker.update();
}
