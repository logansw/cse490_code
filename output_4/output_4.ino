const int LED_OUTPUT_PIN = 3;
const int MAX_ANALOG_OUT = 255; // the max analog output on the Uno is 255
const int DELAY_MS = 5;

int _fadeAmount = 1;      // the amount to fade the LED by on each step
int _curBrightness = 0;   // how bright the LED is
unsigned long previousMillis = 0;

// The setup function runs once when you press reset or power the board
void setup() {
  // set the LED pin to as an output
  pinMode(LED_OUTPUT_PIN, OUTPUT);
  Serial.begin(9600); // for using Serial.println
}

// The loop function runs over and over again forever
void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= DELAY_MS) {

    previousMillis = currentMillis;
  
    // set the brightness of the LED pin
    analogWrite(LED_OUTPUT_PIN, _curBrightness);
  
    // change the brightness for next time through the loop
    _curBrightness = _curBrightness + _fadeAmount;
  
    // reverse the direction of the fading at the end of each fade direction
    if (_curBrightness <= 0 || _curBrightness >= MAX_ANALOG_OUT) {
      _fadeAmount = -_fadeAmount; // reverses fade direction
    }
    
  }
}
