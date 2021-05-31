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
const int TOLERANCE = 50;

String rcvdSerialData;

void setup() {
  Serial.begin(115200);

  // Initialize the display. If it fails, print failure to Serial
  // and enter an infinite loop
  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  _display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // put your main code here, to run repeatedly:
  _display.clearDisplay();

  input();
  _display.println(rcvdSerialData);
  output();
}

void output() {
  _display.setCursor(20, 20);

  int x = map(analogRead(X_PIN), 0, 1023, 100, -100);
  int y = map(analogRead(Y_PIN), 0, 1023, -100, 100);

  bool pause = false;

  if (x < -TOLERANCE) {
    Serial.println("left");
    pause = true;
  } else if (x > TOLERANCE) {
    Serial.println("right");
    pause = true;
  }
  if (y < -TOLERANCE) {
    Serial.println("down");
    pause = true;
  } else if (y > TOLERANCE) {
    Serial.println("up");
    pause = true;
  }

  _display.display();
  if (pause)
    delay(DELAY_TIME);
}

void input() {
  _display.setCursor(12, 32);
  
  if (Serial.available() > 0) {
    rcvdSerialData = Serial.readStringUntil('\n');
    buzz();
  }
}

void buzz() {
  analogWrite(VIBRO_PIN, 200);
  delay(100);
  analogWrite(VIBRO_PIN, 0);
}
