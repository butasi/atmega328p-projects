#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED Display Setup
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// LCD Display Setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the size to match your LCD (e.g., 16x2, 20x4)

// Button pins
#define UP_BUTTON 2
#define DOWN_BUTTON 3
#define LEFT_BUTTON 4
#define RIGHT_BUTTON 5

// Potentiometer pin
#define SPEED_POT A0

// LED pins
#define LED_1 9
#define LED_2 10
#define LED_3 11
#define LED_4 12
#define LED_5 13

// Game variables
int snakeX[64];
int snakeY[64];
int snakeLength;
int foodX, foodY;
int dirX, dirY;
int snakeSpeed;
int maxSpeed = 5;
unsigned long lastMoveTime;
int score = 0;

void setup() {
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;); // Don't proceed, loop forever
  }

  // Initialize LCD display
  lcd.init();
  lcd.backlight();

  // Initialize buttons
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);

  // Initialize LEDs
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_5, OUTPUT);

  // Initialize game
  resetGame();

  display.clearDisplay();
}

void loop() {
  // Read speed from potentiometer
  snakeSpeed = map(analogRead(SPEED_POT), 0, 1023, 1, maxSpeed);

  // Check button presses for direction change
  if (digitalRead(UP_BUTTON) == LOW && dirY == 0) {
    dirX = 0;
    dirY = -1;
  }
  if (digitalRead(DOWN_BUTTON) == LOW && dirY == 0) {
    dirX = 0;
    dirY = 1;
  }
  if (digitalRead(LEFT_BUTTON) == LOW && dirX == 0) {
    dirX = -1;
    dirY = 0;
  }
  if (digitalRead(RIGHT_BUTTON) == LOW && dirX == 0) {
    dirX = 1;
    dirY = 0;
  }

  // Move the snake based on speed and timing
  if (millis() - lastMoveTime > (200 / snakeSpeed)) {
    lastMoveTime = millis();
    moveSnake();
  }

  // Draw game elements on OLED
  display.clearDisplay();

  // Draw snake
  for (int i = 0; i < snakeLength; i++) {
    display.fillRect(snakeX[i], snakeY[i], 2, 2, SSD1306_WHITE);
  }

  // Draw food
  display.fillRect(foodX, foodY, 2, 2, SSD1306_WHITE);

  display.display();

  // Update game info on LCD
  lcd.setCursor(0, 0);
  lcd.print("Score: ");
  lcd.print(score);
  lcd.setCursor(0, 1);
  lcd.print("Speed: ");
  lcd.print(snakeSpeed);

  // Control LEDs based on game state
  controlLEDs();
}

void moveSnake() {
  // Move snake body
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  // Move snake head
  snakeX[0] += dirX * 2;
  snakeY[0] += dirY * 2;

  // Check for food collision
  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    snakeLength++;
    score += 10;
    generateFood();
  }

  // Check for wall collision
  if (snakeX[0] < 0 || snakeX[0] >= SCREEN_WIDTH || snakeY[0] < 0 || snakeY[0] >= SCREEN_HEIGHT) {
    resetGame();
  }

  // Check for self-collision
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      resetGame();
    }
  }
}

void resetGame() {
  snakeLength = 3;
  snakeX[0] = SCREEN_WIDTH / 2;
  snakeY[0] = SCREEN_HEIGHT / 2;
  dirX = 2;
  dirY = 0;
  score = 0;
  generateFood();
}

void generateFood() {
  foodX = random(0, SCREEN_WIDTH / 2) * 2;
  foodY = random(0, SCREEN_HEIGHT / 2) * 2;
}

void controlLEDs() {
  // Turn LEDs on/off based on snake length
  digitalWrite(LED_1, snakeLength > 5 ? HIGH : LOW);
  digitalWrite(LED_2, snakeLength > 10 ? HIGH : LOW);
  digitalWrite(LED_3, snakeLength > 15 ? HIGH : LOW);
  digitalWrite(LED_4, snakeLength > 20 ? HIGH : LOW);
  digitalWrite(LED_5, snakeLength > 25 ? HIGH : LOW);
}
