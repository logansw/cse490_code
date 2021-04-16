const int LED1_OUTPUT_PIN = 3;  // Anode faces Pin 3 (cathode connected to 0V)
const int LED2_OUTPUT_PIN = 4;  // Cathode faces Pin 4 (anode connected to 5V)
const int DELAY_MS = 1000;      // 1 second delay between blinks

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1_OUTPUT_PIN, OUTPUT);
  pinMode(LED2_OUTPUT_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED1_OUTPUT_PIN, HIGH);  // turns ON LED1
  digitalWrite(LED2_OUTPUT_PIN, HIGH);  // turns OFF LED2
  delay(DELAY_MS);

  digitalWrite(LED1_OUTPUT_PIN, LOW);  // turns OFF LED1
  digitalWrite(LED2_OUTPUT_PIN, LOW);  // turns ON LED2
  delay(DELAY_MS);
}
