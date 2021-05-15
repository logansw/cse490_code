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
int currentFloor = 0;
int consecutive = -1;
int spawnsLeft = 0;
bool buttonLifted;

// Player Properties
int xPos = 1;
int yPos = 0;
const int Y_LOCK = 30;
float xVel = 1;
float yVel = 0;
boolean isGrounded = true;

// Floor Properties
int floorHeights[NUM_FLOORS];
int nearestFloorHeight;
int nextFloorHeight;

// Enemy Car Properties
int enemyPosX[4];
int enemyVel[4];
const int NUM_CARS = 4;

// Frame Data
const int FRAME_DELAY = 500;

enum GameState {
  NEW_GAME,
  PLAYING,
  GAME_OVER,
};

GameState gameState = NEW_GAME;

void setup() {
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

  for (int i = 0; i < NUM_FLOORS; i++) {
    floorHeights[i] = FLOOR_GAP * i;
  }

  nearestFloorHeight = floorHeights[0];
  nextFloorHeight = floorHeights[1];

  for (int i = 0; i < NUM_CARS; i++) {
    spawnCar(i);
  }
}

void loop() {
  // Clear the display
  _display.clearDisplay();

  switch (gameState) {
    case NEW_GAME:
      newGameLoop();
      break;
    case PLAYING:
      gameplayLoop();
      break;
    case GAME_OVER:
      gameOverLoop();
      break;
  }

  // Render graphics buffer to screen
  _display.display();
}

void newGameLoop() {
  // Button input (pressed down if val == LOW)
  int val = digitalRead(BUTTON_PIN);

  drawStartScreen();

  if (val == LOW) {
    gameState = PLAYING;
    delay(200);
  }
}

void gameplayLoop() {
  // Button input (pressed down if val == LOW)
  int val = digitalRead(BUTTON_PIN);

  // Turn player around if edge is reached
  if (xPos > 63 - 6 || xPos <= 0) {
    xVel *= -1;
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

  // Update Player Position
  xPos = xPos + xVel;
  yPos = yPos + yVel;

  // Update Enemy Positions
  for (int i = 0; i < NUM_CARS; i++) {
    if (enemyPosX[i] > 63 - 6 || enemyPosX[i] <= 0) {
      enemyVel[i] *= -1;
    }
    enemyPosX[i] += enemyVel[i];
  }

  checkCollision();

  // Draw!
  drawCar(round(xPos), Y_LOCK);
  drawFloors();
  drawCars();

  displayScore(currentFloor, 4, 122);
}

void gameOverLoop() {
  // Button input (pressed down if val == LOW)
  int val = digitalRead(BUTTON_PIN);

  drawEndScreen();

  if (val == HIGH) {
    buttonLifted = true;
  }

  if (buttonLifted && val == LOW) {
    reset();
    gameState = PLAYING;
    delay(200);
  }
}

void reset() {
  currentFloor = 0;
  consecutive = -1;
  spawnsLeft = 0;
  xPos = 1;
  yPos = 0;
  xVel = 1;
  yVel = 0;
  isGrounded = true;
  for (int i = 0; i < NUM_FLOORS; i++) {
    floorHeights[i] = FLOOR_GAP * i;
  }

  nearestFloorHeight = floorHeights[0];
  nextFloorHeight = floorHeights[1];

  for (int i = 0; i < NUM_CARS; i++) {
    spawnCar(i);
  }
}

void drawCar(int x, int y) {
  drawRect(x, y + 1, 6, 2);
  drawRect(x + 1, y + 3, 4, 2);
  drawRect(x + 1, y, 1, 1);
  drawRect(x + 4, y, 1, 1);
}

void drawFloors() {
  for (int i = 0; i < NUM_FLOORS; i++) {
    int y = floorHeights[i] - 1 - yPos;
    if (y < 0) {
      floorHeights[i] += NUM_FLOORS * FLOOR_GAP;
      currentFloor++;
      spawnCar(i);
    }
    drawRect(0, y, 64, 1);
  }
}

void drawCars() {
  for (int i = 0; i < NUM_CARS; i++) {
    int y = floorHeights[i] - yPos;
    drawCar(enemyPosX[i], y);
  }
}

void spawnCar(int i) {
  // If no spawns remain...
  if (spawnsLeft == 0) {
    // Increase consecutive count
    consecutive++;
    // Reset spawns to updated count
    spawnsLeft = consecutive;
    // Spawn next car offscreen
    enemyPosX[i] = -10;
    enemyVel[i] = 0;
  } else {
    // Reduce remaining spawns
    spawnsLeft--;
    // Randomly generate properties for enemy car
    int seed = rand();
    enemyPosX[i] = seed % 56 + 1;
    enemyVel[i] = seed % 3 + 1;
    if (seed % 2 == 0) {
      enemyVel[i] *= -1;
    }
  }
}

void checkCollision() {
  if (yPos <= nearestFloorHeight + 5) {
    int carMod = currentFloor % NUM_CARS;
    if ((xPos >= enemyPosX[carMod] && xPos <= enemyPosX[carMod] + 6) ||
        (enemyPosX[carMod] >= xPos && enemyPosX[carMod] <= xPos + 6)) {
      gameState = GAME_OVER;
      buttonLifted = false;
    }
  } else if (yPos >= nextFloorHeight - 5) {
    int carMod = (currentFloor + 1) % NUM_CARS;
    if ((xPos >= enemyPosX[carMod] && xPos <= enemyPosX[carMod] + 6) ||
        (enemyPosX[carMod] >= xPos && enemyPosX[carMod] <= xPos + 6)) {
      gameState = GAME_OVER;
      buttonLifted = false;
    }
  }
}

void displayScore(int score, int x, int y) {
  int temp = score;
  int len = 0;
  while (temp > 0) {
    len++;
    temp /= 10;
  }

  for (int i = len; i > 0; i--) {
    int digit = score % 10;
    score = score / 10;
    drawDigit(digit, x + 4 * i - 4, y);
  }
}

void drawDigit(int digit, int x, int y) {
  switch (digit) {
    case 1:
      drawOne(x, y);
      break;
    case 2:
      drawTwo(x, y);
      break;
    case 3:
      drawThree(x, y);
      break;
    case 4:
      drawFour(x, y);
      break;
    case 5:
      drawFive(x, y);
      break;
    case 6:
      drawSix(x, y);
      break;
    case 7:
      drawSeven(x, y);
      break;
    case 8:
      drawEight(x, y);
      break;
    case 9:
      drawNine(x, y);
      break;
    case 0:
      drawZero(x, y);
      break;
    default:
      drawRect(x, y, 3, 5);
  }
}

void drawOne(int x, int y) {
  drawRect(x, y + 4, 2, 1);
  drawRect(x + 1, y + 1, 1, 3);
  drawRect(x, y, 3, 1);
}

void drawTwo(int x, int y) {
  drawRect(x, y, 3, 5);
  _display.fillRect(y + 1, x + 1, 1, 2, SSD1306_BLACK);
  _display.fillRect(y + 3, x, 1, 2, SSD1306_BLACK);
}

void drawThree(int x, int y) {
  drawRect(x, y, 3, 1);
  drawRect(x + 2, y + 1, 1, 3);
  drawRect(x, y + 4, 3, 1);
  drawRect(x + 1, y + 2, 1, 1);
}

void drawFour(int x, int y) {
  drawRect(x, y + 2, 1, 3);
  drawRect(x + 1, y + 2, 1, 1);
  drawRect(x + 2, y, 1, 5);
}

void drawFive(int x, int y) {
  drawRect(x, y, 3, 5);
  _display.fillRect(y + 1, x, 1, 2, SSD1306_BLACK);
  _display.fillRect(y + 3, x + 1, 1, 2, SSD1306_BLACK);
}

void drawSix(int x, int y) {
  drawRect(x, y, 3, 5);
  _display.fillRect(y + 1, x + 1, 1, 1, SSD1306_BLACK);
  _display.fillRect(y + 3, x + 1, 1, 2, SSD1306_BLACK);
}

void drawSeven(int x, int y) {
  drawRect(x, y + 4, 3, 1);
  drawRect(x + 2, y, 1, 5);
}

void drawEight(int x, int y) {
  drawRect(x, y, 3, 5);
  _display.fillRect(y + 1, x + 1, 1, 1, SSD1306_BLACK);
  _display.fillRect(y + 3, x + 1, 1, 1, SSD1306_BLACK);
}

void drawNine(int x, int y) {
  drawRect(x, y, 3, 5);
  _display.fillRect(y, x, 2, 2, SSD1306_BLACK);
  _display.fillRect(y + 3, x + 1, 1, 1, SSD1306_BLACK);
}

void drawZero(int x, int y) {
  drawRect(x, y, 3, 5);
  _display.fillRect(y + 1, x + 1, 3, 1, SSD1306_BLACK);
}

void drawStartScreen() {
  int x = 8;
  int y = 70;
  // White background
  drawRect(0, 0, 64, 128);
  // J
  eraseRect(x, y+2, 2, 2);
  eraseRect(x+1, y+1, 2, 2);
  eraseRect(x+2, y, 4, 2);
  eraseRect(x+5, y+1, 2, 2);
  eraseRect(x+6, y+2, 2, 10);
  eraseRect(x, y+10, 8, 2);
  // M
  eraseRect(x+10, y, 2, 12);
  eraseRect(x+2+10, y+6, 1, 3);
  eraseRect(x+3+10, y+4, 2, 4);
  eraseRect(x+5+10, y+6, 1, 3);
  eraseRect(x+6+10, y, 2, 12);
  // P
  eraseRect(x+20, y, 2, 12);
  eraseRect(x+2+20, y+10, 4, 2);
  eraseRect(x+2+20, y+4, 4, 2);
  eraseRect(x+5+20, y+5, 2, 2);
  eraseRect(x+6+20, y+6, 2, 4);
  eraseRect(x+5+20, y+10, 2, 2);

  // 'CR' backdrop
  eraseRect(37, y - 1, 20, 14);
  // C
  drawRect(x+30, y+2, 2, 8);
  drawRect(x+2+30, y, 4, 2);
  drawRect(x+2+30, y+10, 4, 2);
  drawRect(x+6+30, y+8, 2, 2);
  drawRect(x+6+30, y+2, 2, 2);
  drawRect(x+1+30, y+1, 2, 2);
  drawRect(x+1+30, y+9, 2, 2);
  drawRect(x+5+30, y+9, 2, 2);
  drawRect(x+5+30, y+1, 2, 2);
  // R
  drawRect(x+10+30, y, 2, 12);
  drawRect(x+12+30, y+4, 4, 2);
  drawRect(x+12+30, y+10, 4, 2);
  drawRect(x+15+30, y+2, 2, 2);
  drawRect(x+16+30, y, 2, 2);
  drawRect(x+15+30, y+5, 2, 2);
  drawRect(x+15+30, y+9, 2, 2);
  drawRect(x+16+30, y+6, 2, 4);

  // Button
  drawButton();
}

void drawButton() {
  if (millis() % (2 * FRAME_DELAY) >= FRAME_DELAY) {
    eraseRect(28, 30, 8, 3);
    eraseRect(29, 33, 6, 3);
  } else {
    eraseRect(28, 30, 8, 3);
    eraseRect(29, 33, 6, 1);
    eraseRect(26, 35, 1, 1);
    eraseRect(38, 35, 1, 1);
  }
}

void drawEndScreen() {
  int x = 8;
  int y = 80;
  // White background
  drawRect(0, 0, 64, 128);
  // C
  eraseRect(x, y+2, 2, 8);
  eraseRect(x+2, y, 4, 2);
  eraseRect(x+2, y+10, 4, 2);
  eraseRect(x+6, y+8, 2, 2);
  eraseRect(x+6, y+2, 2, 2);
  eraseRect(x+1, y+1, 2, 2);
  eraseRect(x+1, y+9, 2, 2);
  eraseRect(x+5, y+9, 2, 2);
  eraseRect(x+5, y+1, 2, 2);
  // R
  eraseRect(x+10, y, 2, 12);
  eraseRect(x+12, y+4, 4, 2);
  eraseRect(x+12, y+10, 4, 2);
  eraseRect(x+15, y+2, 2, 2);
  eraseRect(x+16, y, 2, 2);
  eraseRect(x+15, y+5, 2, 2);
  eraseRect(x+15, y+9, 2, 2);
  eraseRect(x+16, y+6, 2, 4);
  // A
  eraseRect(x+20, y, 2, 10);
  eraseRect(x+26, y, 2, 10);
  eraseRect(x+22, y+4, 4, 2);
  eraseRect(x+22, y+10, 4, 2);
  eraseRect(x+21, y+9, 2, 2);
  eraseRect(x+25, y+9, 2, 2);
  // S
  eraseRect(x+30, y+2, 2, 2);
  eraseRect(x+30, y+7, 2, 3);
  eraseRect(x+31, y+1, 2, 2);
  eraseRect(x+32, y, 4, 2);
  eraseRect(x+35, y+1, 2, 2);
  eraseRect(x+36, y+2, 2, 3);
  eraseRect(x+35, y+4, 2, 2);
  eraseRect(x+32, y+5, 4, 2);
  eraseRect(x+31, y+6, 2, 2);
  eraseRect(x+31, y+9, 2, 2);
  eraseRect(x+32, y+10, 4, 2);
  eraseRect(x+35, y+9, 2, 2);
  eraseRect(x+36, y+8, 2, 2);
  // H
  eraseRect(x+40, y, 2, 12);
  eraseRect(x+42, y+4, 4, 2);
  eraseRect(x+46, y, 2, 12);

  // Horizontal strip
  eraseRect(0, 55, 64, 11);
  int temp = currentFloor;
  int len = 0;
  while (temp > 0) {
    len++;    
    temp = temp / 10;
  }
  displayScore(currentFloor, 31 - (len * 2), 58);

  // Button
  drawButton();
}

// Draws a rectangle in flipped coordinate system (screen vertical,
// origin in bottom left.
void drawRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h) {
  _display.fillRect(y0, x0, h, w, SSD1306_WHITE);
}

void eraseRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h) {
  _display.fillRect(y0, x0, h, w, SSD1306_BLACK);
}
