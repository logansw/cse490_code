#define KEY_C 262  // 261.6256 Hz (middle C)
#define KEY_D 294  // 293.6648 Hz
#define KEY_E 330  // 329.6276 Hz
#define KEY_F 350  // 349.2282 Hz
#define KEY_G 392  // 391.9954 Hz
#define KEY_A 440
#define KEY_B 494
#define KEY_C2 523

// Inverted on mine because my piano is best used upside down!

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

const boolean _buttonsAreActiveLow = true;

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
}

void loop() {
  // put your main code here, to run repeatedly:
  if(isButtonPressed(INPUT_BUTTON_C_PIN)){
    tone(OUTPUT_PIEZO_PIN, KEY_C);
  }else if(isButtonPressed(INPUT_BUTTON_D_PIN)){
    tone(OUTPUT_PIEZO_PIN, KEY_D);
  }else if(isButtonPressed(INPUT_BUTTON_E_PIN)){
    tone(OUTPUT_PIEZO_PIN, KEY_E);
  }else if(isButtonPressed(INPUT_BUTTON_F_PIN)){
    tone(OUTPUT_PIEZO_PIN, KEY_F);
  }else if(isButtonPressed(INPUT_BUTTON_G_PIN)){
    tone(OUTPUT_PIEZO_PIN, KEY_G);
  }else if(isButtonPressed(INPUT_BUTTON_A_PIN)){
    tone(OUTPUT_PIEZO_PIN, KEY_A);
  }else if(isButtonPressed(INPUT_BUTTON_B_PIN)){
    tone(OUTPUT_PIEZO_PIN, KEY_B);
  }else if(isButtonPressed(INPUT_BUTTON_C2_PIN)){
    tone(OUTPUT_PIEZO_PIN, KEY_C2);
  }else{
    noTone(OUTPUT_PIEZO_PIN); // turn off the waveform
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }
}

boolean isButtonPressed(int btnPin){
  int btnVal = digitalRead(btnPin);
  if(_buttonsAreActiveLow && btnVal == LOW){
    // button is hooked up with pull-up resistor
    // and is in a pressed state
    digitalWrite(OUTPUT_LED_PIN, HIGH);
    return true;
  }else if(!_buttonsAreActiveLow && btnVal == HIGH){
    // button is hooked up with a pull-down resistor
    // and is in a pressed state
    digitalWrite(OUTPUT_LED_PIN, HIGH);
    return true;
  }

  // button is not pressed
  return false;
}
