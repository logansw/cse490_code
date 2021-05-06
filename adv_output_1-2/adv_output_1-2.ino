#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Instantiate SSD1306 driver display object
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int RADIUS = 8;
const int SQUISH = 8;
int _x_speed = 2;
int _y_speed = 2;
int _x_pos = SCREEN_WIDTH / 2;
int _y_pos = SCREEN_HEIGHT / 2;

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

  _x_pos += _x_speed;
  _y_pos += _y_speed;

  _display.drawCircle(_x_pos, _y_pos, RADIUS, SSD1306_WHITE);
  
  if (_x_pos + RADIUS >= SCREEN_WIDTH) {
    _display.drawCircle(_x_pos, _y_pos, RADIUS, SSD1306_BLACK);   // Hit right
    _x_speed = -2;
    _display.drawRoundRect(SCREEN_WIDTH - SQUISH, _y_pos - RADIUS, SQUISH, RADIUS * 2, SQUISH / 2, SSD1306_WHITE);
  } else if (_x_pos - RADIUS < 0) {
    _display.drawCircle(_x_pos, _y_pos, RADIUS, SSD1306_BLACK);   // Hit left
    _x_speed = 2;
    _display.drawRoundRect(0, _y_pos - RADIUS, SQUISH, RADIUS * 2, SQUISH / 2, SSD1306_WHITE);
  } else if (_y_pos + RADIUS >= SCREEN_HEIGHT) {
    _display.drawCircle(_x_pos, _y_pos, RADIUS, SSD1306_BLACK);   // Hit bottom
    _y_speed = -2;
    _display.drawRoundRect(_x_pos - RADIUS, SCREEN_HEIGHT - SQUISH, RADIUS * 2, SQUISH, SQUISH / 2, SSD1306_WHITE);
  } else if (_y_pos - RADIUS < 0) {
    _display.drawCircle(_x_pos, _y_pos, RADIUS, SSD1306_BLACK);   // Hit top
    _y_speed = 2;
    _display.drawRoundRect(_x_pos - RADIUS, 0, RADIUS * 2, SQUISH, SQUISH / 2, SSD1306_WHITE);
  }

  // Render graphics buffer to screen
  _display.display();
}
