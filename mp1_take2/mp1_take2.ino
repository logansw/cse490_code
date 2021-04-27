#include "Blinker.h"
#include "PushButton.h"
#include "src/RGBConverter/RGBConverter.h"

// Input Pins
const int BUTTON_PIN = 2;
const int DIAL_INPUT_PIN = A0;
const int PHOTO_PIN = A1;

// Output Pins
const int RED_PIN = 11;
const int BLUE_PIN = 10;
const int GREEN_PIN = 9;

// Other Settings
const int DEBOUNCE_TIME = 10;
const byte MAX_RGB_VALUE = 255;
const float PHOTO_MAX = 600.0;  // Max observed reading from photoresistor pin
const float DISK_MIN = 350.0;   // Min observed reading from disk potentiometer pin
const float DISK_MAX = 850.0;   // Max observed reading from disk potentiometer pin
const int MODE_DELAY = 300;     // How long the mode indicator light flashes at each interval

// Global Variables
int _curMode = 1;
int _lastPressedTimestamp = 0;  // Tracks when button was last pressed, for setting BPM
float _hue = 0;   // hue varies between 0 - 1
float _step = 0.001f;   // Rate at which color changes in MODE 2
int _blinkRate = 1000;
Blinker _redBlinker(RED_PIN, _blinkRate);
Blinker _greenBlinker(GREEN_PIN, _blinkRate);
Blinker _blueBlinker(BLUE_PIN, _blinkRate);
PushButton _button(BUTTON_PIN);
RGBConverter _rgbConverter;
byte _rgb[3];   // Tracks color of RGB LED between modes

// ----------------------------------------------------

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  _button.setActiveLogic(LOW);
  _button.setDebounceTime(DEBOUNCE_TIME);
  _button.disableDoubleClick();
  _button.setHoldTime(1000);

  // Initially set LED to white
  _rgb[0] = MAX_RGB_VALUE;
  _rgb[1] = MAX_RGB_VALUE;
  _rgb[2] = MAX_RGB_VALUE;
}

void loop() {
  tick(); // Always tick
  switch(_curMode) {
    case 1:
      modeOne();
      // Don't blink in MODE 1
      break;
    case 2:
      modeTwo();
      // Blink in MODE 2 and MODE 3
      _redBlinker.update(_rgb[0]);
      _greenBlinker.update(_rgb[1]);
      _blueBlinker.update(_rgb[2]);
      break;
    case 3:
      modeThree();
      // Blink in MODE 2 and MODE 3
      _redBlinker.update(_rgb[0]);
      _greenBlinker.update(_rgb[1]);
      _blueBlinker.update(_rgb[2]);
      break;
  }
}

/*
  MODE 1: Nightlight Mode
  After a long party, nothing helps calm the mind down like a good old nightlight (?)
*/
void modeOne() {
  int photoVal = analogRead(PHOTO_PIN);
  float photoValNorm = 1.0 - (photoVal / 900.0);

  
  if (photoValNorm < 0.0) {
    photoValNorm = 0;
  }

  _hue = photoValNorm / 2.0;
  _rgbConverter.hslToRgb(_hue, 1, 0.5, _rgb);

  int redVal = (int)(_rgb[0] * photoValNorm);
  int greenVal = (int)(_rgb[1] * photoValNorm);
  int blueVal = (int)(_rgb[2] * photoValNorm);
  
  analogWrite(RED_PIN, redVal);
  analogWrite(GREEN_PIN, greenVal);
  analogWrite(BLUE_PIN, blueVal);
}

/*
  MODE 2: PARTY MODE
  Warm up your DJ hands and get ready to party by changing the LED's color by spinning some disks
*/
void modeTwo() {
  Serial.println("Mode 2");
  
  int potVal = analogRead(DIAL_INPUT_PIN);
  float potValNorm = (potVal - DISK_MIN) / (DISK_MAX - DISK_MIN);  // (Ranges from about 350 to 850)

//  Serial.println(potValNorm);

  /**
   * setColor takes in values between 0 - 255 for the amount of red, green, and blue, respectively
   * where 255 is the maximum amount of that color and 0 is none of that color. You can illuminate
   * all colors by intermixing different combinations of red, green, and blue.
   *
   * This function is based on https://gist.github.com/jamesotron/766994
   *
   */
  _rgbConverter.hslToRgb(_hue, 1, 0.5, _rgb);

  if (_hue > potValNorm) {
    _hue -= _step;
  } else if (_hue < potValNorm) {
    _hue += _step;
  }
}

/* 
  MODE 3: Song Picker Mode
  When you want to DJ with a different song, you'll need to adjust the BPM of the LED by clicking to the beat
*/
void modeThree() {
//  Serial.println("Mode 3");
  if (_button.isClicked()) {
    int timePressed = millis();
    _blinkRate = timePressed - _lastPressedTimestamp;
    _lastPressedTimestamp = timePressed;
  }

  _redBlinker.setInterval(_blinkRate);
  _greenBlinker.setInterval(_blinkRate);
  _blueBlinker.setInterval(_blinkRate);
}

/*
  Ticks forward in time, reacting to updated input values and responding accordingly via output ports.
*/
void tick() {
  _button.update();

  if(_button.isHeld()) {
    _curMode++;
    if (_curMode > 3) {
      _curMode = 1;
    }
    // Flash white briefly to indicate mode #
    for (int i = 0; i < _curMode; i++) {
      analogWrite(RED_PIN, MAX_RGB_VALUE);
      analogWrite(GREEN_PIN, MAX_RGB_VALUE);
      analogWrite(BLUE_PIN, MAX_RGB_VALUE);
      delay(MODE_DELAY);
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
      delay(MODE_DELAY);
    }
  }
}
