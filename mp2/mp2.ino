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

// Game Properties
const int JUMP_STRENGTH = 6;
const float LOW_GRAV = 0.4;
const float HIGH_GRAV = 1;
const int FLOOR_GAP = 30;
const int NUM_FLOORS = 4;

// Player Properties
int xPos = 0;
int yPos = 0;
const int Y_LOCK = 30;
float xVel = 1;
float yVel = 0;
boolean isGrounded = true;

// Floor Properties
int floorHeights[NUM_FLOORS];
int nearestFloorHeight = floorHeights[0];
int nextFloorHeight = floorHeights[1];

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

  for (int i=0; i<NUM_FLOORS; i++) {
    floorHeights[i] = FLOOR_GAP * i;
  }
}

void loop(){
  // Clear the display
  _display.clearDisplay();

  // Button input (pressed down if val == LOW)
  int val = digitalRead(BUTTON_PIN);

  // Turn player around if edge is reached
  if (xPos > 63 - 6) {
    xVel = -1;
  } else if (xPos <= 0) {
    xVel = 1;
  }

  // Apply gravity if player is not grounded
  if (!isGrounded) {
    if (val == LOW) {
      yVel -= LOW_GRAV;
    } else {
      yVel -= HIGH_GRAV;
    }
  }

  if (yPos >= nextFloorHeight) {
    nearestFloorHeight = nextFloorHeight;
    nextFloorHeight += FLOOR_GAP;
  }

  // Re-ground if player reaches floor
  if (yPos <= nearestFloorHeight) {
    yPos = nearestFloorHeight;
    isGrounded = true;
    yVel = 0;
  }

  // If button is pressed and player is grounded, jump!
  if (val == LOW && isGrounded) {
    yVel = JUMP_STRENGTH;
    isGrounded = false;
  }

  // Update Positions
  xPos = xPos + xVel;
  yPos = yPos + yVel;

  // Draw!
  drawCar(round(xPos), Y_LOCK);
  drawFloors();

  // Render graphics buffer to screen
  _display.display();
}

void drawCar(int x, int y) {
  drawRect(x, y+1, 6, 2);
  drawRect(x+1, y+3, 4, 2);
  drawRect(x+1, y, 1, 1);
  drawRect(x+4, y, 1, 1);
}

void drawFloors() {
  for (int i=0; i<NUM_FLOORS; i++) {
    int y = floorHeights[i]-1-yPos;
    if (y < 0) {
      floorHeights[i] += NUM_FLOORS * FLOOR_GAP;
    } else if (y > 127) {
      floorHeights[i] -= NUM_FLOORS * FLOOR_GAP;
    }
    drawRect(0, y, 64, 1);
  }
}

// Draws a rectangle in flipped coordinate system (screen vertical,
// origin in bottom left.
void drawRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h) {
  _display.fillRect(y0, x0, h, w, SSD1306_WHITE);
}
