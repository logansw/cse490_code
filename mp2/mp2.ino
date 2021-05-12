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

const int RED_PIN = 9;
const int YELLOW_PIN = 10;
const int GREEN_PIN = 11;
const int BUZZER_PIN = 6;
const int VIBRO_PIN = 5;
const int BUTTON_PIN = 4;

void setup(){
  Serial.begin(9600);

  // Initialize the display. If it fails, print failure to Serial
  // and enter an infinite loop
  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(VIBRO_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop(){
  // Clear the display
  _display.clearDisplay();

  _display.drawCircle(50, 50, 10, SSD1306_WHITE);

  int val = digitalRead(BUTTON_PIN);

  if (val == LOW) {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(YELLOW_PIN, HIGH);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(VIBRO_PIN, HIGH);
    tone(BUZZER_PIN, 392);
  } else {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(VIBRO_PIN, LOW);
    noTone(BUZZER_PIN);
  }

  // Render graphics buffer to screen
  _display.display();
}
