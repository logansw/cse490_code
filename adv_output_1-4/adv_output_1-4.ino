#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Instantiate SSD1306 driver display object
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int POT_PIN_X = A0;
const int POT_PIN_Y = A1;
const int MAX_ANALOG_INPUT = 1023;
const int RADIUS = 10;

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

  _display.clearDisplay();
  pinMode(POT_PIN_X, INPUT_PULLUP);
  pinMode(POT_PIN_Y, INPUT_PULLUP);
}

void loop(){
  // Clear the display
  // _display.clearDisplay();

  int xVal = analogRead(POT_PIN_X);
  int yVal = analogRead(POT_PIN_Y);

  _x_pos = map(xVal, 0, MAX_ANALOG_INPUT, 0, _display.width());
  _y_pos = map(yVal, 0, MAX_ANALOG_INPUT, 0, _display.height());

  _display.drawPixel(_x_pos, _y_pos, SSD1306_WHITE);
  // _display.drawCircle(_x_pos, _y_pos, RADIUS, SSD1306_WHITE);

  // Render graphics buffer to screen
  _display.display();
}
