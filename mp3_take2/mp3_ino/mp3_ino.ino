#include <Wire.h>
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Instantiate SSD1306 driver display object
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int X_PIN = A0;
const int Y_PIN = A1;
const int VIBRO_PIN = 12;
const int BTN_PIN = 11;

const int DELAY_TIME = 200;
const int TOLERANCE = 70;

const int X_MARGIN = 28;
const int LINE_HEIGHT = 12;

String rcvdSerialData;

int feature = 0;
String browMessage;
String eyeMessage;
String noseMessage;
String mouthMessage;

void setup() {
  Serial.begin(115200);

  // Initialize the display. If it fails, print failure to Serial
  // and enter an infinite loop
  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(VIBRO_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);

  browMessage   = "Brow --   0 >";
  eyeMessage    = "Eye ---   0 >";
  noseMessage   = "Nose --   0 >";
  mouthMessage  = "Mouth -   0 >";

  _display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // put your main code here, to run repeatedly:
  _display.clearDisplay();

  input();

  displayText();
  
  // _display.println(rcvdSerialData);
  output();
}

void displayText() {
  if (feature == 0) {
    _display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  }
  _display.setCursor(X_MARGIN, LINE_HEIGHT);
  _display.println(browMessage);
  _display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);

  if (feature == 1) {
    _display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  }
  _display.setCursor(X_MARGIN, LINE_HEIGHT * 2);
  _display.println(eyeMessage);
  _display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);

  if (feature == 2) {
    _display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  }
  _display.setCursor(X_MARGIN, LINE_HEIGHT * 3);
  _display.println(noseMessage);
  _display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);

  if (feature == 3) {
    _display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  }
  _display.setCursor(X_MARGIN, LINE_HEIGHT * 4);
  _display.println(mouthMessage);
  _display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
}

void output() {
  _display.setCursor(20, 20);

  int x = map(analogRead(X_PIN), 0, 1023, 100, -100);
  int y = map(analogRead(Y_PIN), 0, 1023, -100, 100);

  int btnVal = digitalRead(BTN_PIN);

  bool pause = false;

  if (btnVal == HIGH) {
    if (x < -TOLERANCE) {
      Serial.println("left");
      pause = true;
    } else if (x > TOLERANCE) {
      Serial.println("right");
      pause = true;
    }
    if (y < -TOLERANCE) {
      Serial.println("down");
      feature++;
      if (feature > 3)
        feature = 3;
      pause = true;
    } else if (y > TOLERANCE) {
      Serial.println("up");
      feature--;
      if (feature < 0)
        feature = 0;
      pause = true;
    }
  } else {
    if (x < -TOLERANCE) {
      Serial.println("decrease_width");
      pause = true;
    } else if (x > TOLERANCE) {
      Serial.println("increase_width");
      pause = true;
    }
    if (y < -TOLERANCE) {
      Serial.println("decrease_height");
      pause = true;
    } else if (y > TOLERANCE) {
      Serial.println("increase_height");
      pause = true;
    }
  }

  _display.display();
  if (pause)
    delay(DELAY_TIME);
}

void input() {  
  if (Serial.available() > 0) {
    if (feature == 0) {
      browMessage = Serial.readStringUntil('\n');
    } else if (feature == 1) {
      eyeMessage = Serial.readStringUntil('\n');
    } else if (feature == 2) {
      noseMessage = Serial.readStringUntil('\n');
    } else if (feature == 3) {
      mouthMessage = Serial.readStringUntil('\n');
    }
    buzz();
  }
}

void buzz() {
  analogWrite(VIBRO_PIN, 200);
  delay(100);
  analogWrite(VIBRO_PIN, 0);
}
