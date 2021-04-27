#include "Blinker.h"
#include "PushButton.h"

const int BUTTON_PIN = 2;
Blinker _led1Blinker(11, 300);  // specify pin and blink interval (200ms)
PushButton button(BUTTON_PIN);
const int DEBOUNCE_TIME = 10;

int lastPressTimestamp = 0;

void setup() {
  // empty
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.setActiveLogic(LOW);
  button.setDebounceTime(DEBOUNCE_TIME);
  button.disableDoubleClick();
  Serial.begin(9600);
}

void loop() {
  _led1Blinker.update();
  button.update();

  if (button.isClicked()) {
    int timePressed = millis();
    int interval = timePressed - lastPressTimestamp;
    Serial.println(interval);
    _led1Blinker.setInterval(interval);
    lastPressTimestamp = millis();
  }

//  if (digitalRead(BUTTON_PIN) == LOW) {
//    int curTime = millis();
//    int interval = curTime - lastPressTimestamp;
//    Serial.println(interval);
//    _led1Blinker.setInterval(curTime - lastPressTimestamp);
//    lastPressTimestamp = millis();
//  }
}
