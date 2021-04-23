#include "src/PushButton/PushButton.h"

#define KEY_C 262  // 261.6256 Hz (middle C)
#define KEY_D 294  // 293.6648 Hz
#define KEY_E 330  // 329.6276 Hz
#define KEY_F 350  // 349.2282 Hz
#define KEY_G 392  // 391.9954 Hz
#define KEY_A 440
#define KEY_B 494
#define KEY_C2 523

const int INPUT_BUTTON_C_PIN = 1;
const int INPUT_BUTTON_D_PIN = 2;
const int INPUT_BUTTON_E_PIN = 3;
const int INPUT_BUTTON_F_PIN = 4;
const int INPUT_BUTTON_G_PIN = 5;
const int INPUT_BUTTON_A_PIN = 6;
const int INPUT_BUTTON_B_PIN = 7;
const int INPUT_BUTTON_C2_PIN = 8;

const int OUTPUT_PIEZO_PIN = 9;
const int OUTPUT_LED_PIN = LED_BUILTIN; // visual feedback on button press
const int DEBOUNCE_TIME = 50;

const boolean _buttonsAreActiveLow = true;

int activeButton = 0;

PushButton buttonC1(INPUT_BUTTON_C_PIN);
PushButton buttonD(INPUT_BUTTON_D_PIN);
PushButton buttonE(INPUT_BUTTON_E_PIN);
PushButton buttonF(INPUT_BUTTON_F_PIN);
PushButton buttonG(INPUT_BUTTON_G_PIN);
PushButton buttonA(INPUT_BUTTON_A_PIN);
PushButton buttonB(INPUT_BUTTON_B_PIN);
PushButton buttonC2(INPUT_BUTTON_C2_PIN);

void setup() {
  // put your setup code here, to run once:
  pinMode(INPUT_BUTTON_C_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_D_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_E_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_F_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_G_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_B_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_C2_PIN, INPUT_PULLUP);
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);
  pinMode(OUTPUT_LED_PIN, OUTPUT);

  buttonC1.setActiveLogic(LOW);
  buttonD.setActiveLogic(LOW);
  buttonE.setActiveLogic(LOW);
  buttonF.setActiveLogic(LOW);
  buttonG.setActiveLogic(LOW);
  buttonA.setActiveLogic(LOW);
  buttonB.setActiveLogic(LOW);
  buttonC2.setActiveLogic(LOW);

  buttonC1.setDebounceTime(DEBOUNCE_TIME);
  buttonD.setDebounceTime(DEBOUNCE_TIME);
  buttonE.setDebounceTime(DEBOUNCE_TIME);
  buttonF.setDebounceTime(DEBOUNCE_TIME);
  buttonG.setDebounceTime(DEBOUNCE_TIME);
  buttonA.setDebounceTime(DEBOUNCE_TIME);
  buttonB.setDebounceTime(DEBOUNCE_TIME);
  buttonC2.setDebounceTime(DEBOUNCE_TIME);
}

void loop() {
  buttonC1.update();
  buttonD.update();
  buttonE.update();
  buttonF.update();
  buttonG.update();
  buttonA.update();
  buttonB.update();
  buttonC2.update();

  // I feel like this was the wrong way to complete this assignment :0

  if(buttonC1.isClicked()){
    activeButton = 1;
    tone(OUTPUT_PIEZO_PIN, KEY_C);
  }else if(buttonD.isClicked()){
    activeButton = 2;
    tone(OUTPUT_PIEZO_PIN, KEY_D);
  }else if(buttonE.isClicked()){
    activeButton = 3;
    tone(OUTPUT_PIEZO_PIN, KEY_E);
  }else if(buttonF.isClicked()){
    activeButton = 4;
    tone(OUTPUT_PIEZO_PIN, KEY_F);
  }else if(buttonG.isClicked()){
    activeButton = 5;
    tone(OUTPUT_PIEZO_PIN, KEY_G);
  }else if(buttonA.isClicked()){
    activeButton = 6;
    tone(OUTPUT_PIEZO_PIN, KEY_A);
  }else if(buttonB.isClicked()){
    activeButton = 7;
    tone(OUTPUT_PIEZO_PIN, KEY_B);
  }else if(buttonC2.isClicked()){
    activeButton = 8;
    tone(OUTPUT_PIEZO_PIN, KEY_C2);
  }else if(buttonC1.isReleased() && activeButton == 1) {
    activeButton = 0;
    noTone(OUTPUT_PIEZO_PIN);
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }else if(buttonD.isReleased() && activeButton == 2) {
    activeButton = 0;
    noTone(OUTPUT_PIEZO_PIN);
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }else if(buttonE.isReleased() && activeButton == 3) {
    activeButton = 0;
    noTone(OUTPUT_PIEZO_PIN);
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }else if(buttonF.isReleased() && activeButton == 4) {
    activeButton = 0;
    noTone(OUTPUT_PIEZO_PIN);
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }else if(buttonG.isReleased() && activeButton == 5) {
    activeButton = 0;
    noTone(OUTPUT_PIEZO_PIN);
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }else if(buttonA.isReleased() && activeButton == 6) {
    activeButton = 0;
    noTone(OUTPUT_PIEZO_PIN);
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }else if(buttonB.isReleased() && activeButton == 7) {
    activeButton = 0;
    noTone(OUTPUT_PIEZO_PIN);
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }else if(buttonC2.isReleased() && activeButton == 8) {
    activeButton = 0;
    noTone(OUTPUT_PIEZO_PIN);
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }
}
