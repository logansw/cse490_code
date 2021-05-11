const int VIBROMOTOR_OUTPUT_PIN = 3;
const int POT_PIN = A0;

void setup() {
  // put your setup code here, to run once:
  pinMode(VIBROMOTOR_OUTPUT_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(POT_PIN);
  
  analogWrite(VIBROMOTOR_OUTPUT_PIN, val);
  delay(50);
}
