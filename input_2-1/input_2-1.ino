const int OUTPUT_PIEZO_PIN = 9;
const int OUTPUT_LED_PIN = LED_BUILTIN;
const int SOUND_DURATION_MS = 500;

void setup() {
  // put your setup code here, to run once:
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);
  pinMode(OUTPUT_LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  tone(OUTPUT_PIEZO_PIN, 392);
  digitalWrite(OUTPUT_LED_PIN, HIGH);
  delay(SOUND_DURATION_MS);

  tone(OUTPUT_PIEZO_PIN, 262);
  digitalWrite(OUTPUT_LED_PIN, LOW);
  delay(SOUND_DURATION_MS);
}
