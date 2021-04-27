/*
 * This example reads in a force-sensitive resistor value (from A0 using a voltage-divider 
 * circuit) and uses it to play a tone on a connected piezo buzzer and set the brightness 
 * of an LED. 
 * 
 * By Jon E. Froehlich
 * @jonfroehlich
 * http://makeabilitylab.io
 * 
 * For a walkthrough and circuit diagram, see:
 * https://makeabilitylab.github.io/physcomp/arduino/force-sensitive-resistors.html
 * 
 */

const int OUTPUT_LED_PIN = LED_BUILTIN;
const int OUTPUT_PIEZO_PIN = 2;
const int INPUT_FSR_PIN = A0;
const int DELAY_MS = 20; // how often to read from the sensor input

void setup() {
  pinMode(OUTPUT_LED_PIN, OUTPUT);
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);
  pinMode(INPUT_FSR_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {

  // Read the force-sensitive resistor value
  int fsrVal = analogRead(INPUT_FSR_PIN);

  // Remap the value for output. 
  int ledVal = map(fsrVal, 0, 1023, 0, 255);
  int freq = map(fsrVal, 0, 1023, 50, 1500); // change min/max freq here

//  // only play sound if the user is pressing on the FSR
//  if(fsrVal > 0){
//    tone(OUTPUT_PIEZO_PIN, freq);
//  }else{
//    noTone(OUTPUT_PIEZO_PIN);
//  }

  if(ledVal > 0 && ledVal < 32) {
    tone(OUTPUT_PIEZO_PIN, 262);
  }else if(ledVal >= 32 && ledVal < 64) {
    tone(OUTPUT_PIEZO_PIN, 294);
  }else if(ledVal >= 64 && ledVal < 96) {
    tone(OUTPUT_PIEZO_PIN, 330);
  }else if(ledVal >= 96 && ledVal < 128) {
    tone(OUTPUT_PIEZO_PIN, 349);
  }else if(ledVal >= 128 && ledVal < 160) {
    tone(OUTPUT_PIEZO_PIN, 392);
  }else if(ledVal >= 160 && ledVal < 192) {
    tone(OUTPUT_PIEZO_PIN, 440);
  }else if(ledVal >= 192 && ledVal < 224) {
    tone(OUTPUT_PIEZO_PIN, 494);
  }else if(ledVal >= 224 && ledVal < 256) {
    tone(OUTPUT_PIEZO_PIN, 523);
  }else {
    noTone(OUTPUT_PIEZO_PIN);
  }

  // Print the raw sensor value and the converted led value (e,g., for Serial Plotter)
  Serial.println(ledVal);

  // Write out the LED value. 
  analogWrite(OUTPUT_LED_PIN, ledVal);

  delay(DELAY_MS);
}
