#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Instantiate SSD1306 driver display object
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int _x = 0;
boolean movingRight = true;

void setup(){
  Serial.begin(9600);
  
  // Initialize the display. If it fails, print failure to Serial
  // and enter an infinite loop
  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
}

void loop(){
  // Clear the display
  _display.clearDisplay();

  if (_x + 26 >= 128) {
    movingRight = false;
  } else if (_x + 20 <= 0) {
    movingRight = true;
  }

  if (movingRight) {
    _x++;
  } else {
    _x--;
  }

  // Put in drawing routines
  // In this case, draw a circle at x,y location of 50,20 with a radius of 10
//  _display.fillCircle(50, 20, 10, SSD1306_WHITE);
//  _display.drawLine(10, 10, 40, 40, SSD1306_WHITE);
//  _display.fillCircle(100, 20, 10, SSD1306_BLACK);
//  _display.drawRect(40, 0, 100, 20, SSD1306_WHITE);

  _display.drawRect(20 + _x, 20, 6, 2, SSD1306_WHITE);
  _display.drawRect(21 +_x, 18, 4, 2, SSD1306_WHITE);
  _display.drawPixel(21 + _x, 22, SSD1306_WHITE);
  _display.drawPixel(24 + _x, 22, SSD1306_WHITE);

  _display.drawChar(40, 40, 'a', SSD1306_WHITE, SSD1306_BLACK, 3);

  _display.setTextColor(SSD1306_WHITE);
  _display.setCursor(80, 40);
  _display.print("Hello world!");

  // Render graphics buffer to screen
  _display.display();
}
