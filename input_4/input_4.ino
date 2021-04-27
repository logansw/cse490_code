const int LED_OUTPUT_PIN = 3;
const int POT_INPUT_PIN = A0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_OUTPUT_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int potVal = analogRead(POT_INPUT_PIN);
  float potValNorm =  potVal / 1023.0 * 255.0;

  analogWrite(LED_OUTPUT_PIN, potValNorm);
}
