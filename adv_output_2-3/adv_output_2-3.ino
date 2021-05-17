const int VIBROMOTOR_OUTPUT_PIN = 5;
const int BUTTON_PIN = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(VIBROMOTOR_OUTPUT_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {

  secondLoop();

  // delay(50);
}

void secondLoop() {
  // put your main code here, to run repeatedly:
  int val = digitalRead(BUTTON_PIN);
  
  if (val == LOW) {
    // Short buzz
    analogWrite(VIBROMOTOR_OUTPUT_PIN, 255);
    delay(100);
    // Intermission
    analogWrite(VIBROMOTOR_OUTPUT_PIN, 0);
    delay(50);
    // Fade-out buzz
    for (int i = 255; i > 0; i--) {
      analogWrite(VIBROMOTOR_OUTPUT_PIN, i);
      delay(3);
    }
  } 
}
