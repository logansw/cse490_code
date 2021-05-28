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

const int Y_PIN = A0;
const int X_PIN = A1;
const int BTN_PIN = 4;

const int Y_MAX = 888;
const int Y_MIN = 235;
const int X_MAX = 785;
const int X_MIN = 156;

const int TOLERANCE = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(X_PIN, INPUT_PULLUP);
  pinMode(Y_PIN, INPUT_PULLUP);
  pinMode(BTN_PIN, INPUT_PULLUP);

  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  Serial.begin(9600);
}

void loop() {
  _display.clearDisplay();

  _display.fillRect(10, 10, 10, 10, SSD1306_WHITE);
  
  int xRaw = analogRead(X_PIN);
  int yRaw = analogRead(Y_PIN);
  int btnRaw = digitalRead(BTN_PIN);
  
  int x = map(xRaw, X_MIN, X_MAX, 100, -100);
  int y = map(yRaw, Y_MIN, Y_MAX, -100, 100);

  if (x > 100 - TOLERANCE) {
    if (btnRaw == LOW) {
      Serial.println("ROTATE RIGHT");
    } else {
      Serial.println("CYCLE RIGHT");
    }
  } else if (x < -100 + TOLERANCE) {
    if (btnRaw == LOW) {
      Serial.println("ROTATE LEFT");
    } else {
      Serial.println("CYCLE LEFT");
    }
  }

  if (y > 100 - TOLERANCE) {
    if (btnRaw == LOW) {
      Serial.println("SCALE UP");
    } else {
      Serial.println("CYCLE UP");
    }
  } else if (y < -100 + TOLERANCE) {
    if (btnRaw == LOW) {
      Serial.println("SCALE DOWN");
    } else {
      Serial.println("CYCLE DOWN");
    }
  }

  _display.display();
}
