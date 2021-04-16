const int LED1_OUTPUT_PIN = 2;
const int LED1_BLINK_INTERVAL_MS = 800; // interval at which to blink LED1 (in milliseconds)

const int LED2_OUTPUT_PIN = 5;
const int LED2_BLINK_INTERVAL_MS = 400; // interval at which to blink LED2 (in milliseconds)

const int LED3_OUTPUT_PIN = 9;
const int LED3_BLINK_INTERVAL_MS = 200; // interval at which to blink LED3 (in milliseconds)

unsigned long _led1LastToggledTimestampMs = 0; // tracks the last time LED1 was updated
int _led1State = LOW; // will toggle between LOW and HIGH

unsigned long _led2LastToggledTimestampMs = 0; // tracks the last time LED2 was updated
int _led2State = LOW; // will toggle between LOW and HIGH

unsigned long _led3LastToggledTimestampMs = 0; // tracks the last time LED3 was updated
int _led3State = LOW; // will toggle between LOW and HIGH


void setup() {
  pinMode(LED1_OUTPUT_PIN, OUTPUT);
  pinMode(LED2_OUTPUT_PIN, OUTPUT);
  pinMode(LED3_OUTPUT_PIN, OUTPUT);
}

void loop() {
  unsigned long currentTimestampMs = millis();

  // Check to see if we reached the toggle state interval for LED1 
  if (currentTimestampMs - _led1LastToggledTimestampMs >= LED1_BLINK_INTERVAL_MS) {
    _led1LastToggledTimestampMs = millis();
    _led1State = !_led1State;
    digitalWrite(LED1_OUTPUT_PIN, _led1State);
  }
  
  // Check to see if we reached the toggle state interval for LED2
  if (currentTimestampMs - _led2LastToggledTimestampMs >= LED2_BLINK_INTERVAL_MS) {
    _led2LastToggledTimestampMs = millis();
    _led2State = !_led2State;
    digitalWrite(LED2_OUTPUT_PIN, _led2State);
  }
  
  // Check to see if we reached the toggle state interval for LED3
  if (currentTimestampMs - _led3LastToggledTimestampMs >= LED3_BLINK_INTERVAL_MS) {
    _led3LastToggledTimestampMs = millis();
    _led3State = !_led3State;
    digitalWrite(LED3_OUTPUT_PIN, _led3State);
  }
}
