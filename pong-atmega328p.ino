#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Button pins
#define RESET_BUTTON 2
#define PAUSE_BUTTON 3

// Potentiometer pins
#define PLAYER1_POT A0

// Game variables
int player1Y;
int player2Y;
int ballX, ballY;
int ballDirX, ballDirY;
int player1Score = 0;
int player2Score = 0;
bool paused = false;

void setup() {
  // Initialize display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;); // Don't proceed, loop forever
  }

  // Initialize buttons
  pinMode(RESET_BUTTON, INPUT_PULLUP);
  pinMode(PAUSE_BUTTON, INPUT_PULLUP);

  // Initialize game variables
  resetGame();

  display.clearDisplay();
}

void loop() {
  // Check if the game is paused
  if (digitalRead(PAUSE_BUTTON) == LOW) {
    paused = !paused;
    delay(300); // Debounce delay
  }

  if (!paused) {
    // Read potentiometer values for player 1's paddle position
    player1Y = map(analogRead(PLAYER1_POT), 0, 1023, 0, SCREEN_HEIGHT - 10);

    // AI control for player 2's paddle
    int aiSpeed = 2;  // Adjust AI speed here
    if (ballY < player2Y) {
      player2Y -= aiSpeed;
    } else if (ballY > player2Y + 10) {
      player2Y += aiSpeed;
    }

    // Constrain AI paddle within screen bounds
    player2Y = constrain(player2Y, 0, SCREEN_HEIGHT - 10);

    // Move the ball
    ballX += ballDirX;
    ballY += ballDirY;

    // Ball collision with top and bottom
    if (ballY <= 0 || ballY >= SCREEN_HEIGHT - 2) ballDirY *= -1;

    // Ball collision with player 1 paddle
    if (ballX <= 2 && ballY >= player1Y && ballY <= player1Y + 10) ballDirX *= -1;

    // Ball collision with player 2 paddle
    if (ballX >= SCREEN_WIDTH - 4 && ballY >= player2Y && ballY <= player2Y + 10) ballDirX *= -1;

    // Check for ball out of bounds and update score
    if (ballX < 0) {
      player2Score++;
      resetGame();
    } else if (ballX > SCREEN_WIDTH) {
      player1Score++;
      resetGame();
    }

    // Draw game elements
    display.clearDisplay();
    display.fillRect(0, player1Y, 2, 10, SSD1306_WHITE); // Player 1 paddle
    display.fillRect(SCREEN_WIDTH - 2, player2Y, 2, 10, SSD1306_WHITE); // AI paddle
    display.fillRect(ballX, ballY, 2, 2, SSD1306_WHITE); // Ball

    // Display scores
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(30, 0);
    display.print("P1: ");
    display.print(player1Score);
    display.setCursor(SCREEN_WIDTH - 60, 0);
    display.print("P2: ");
    display.print(player2Score);

    display.display();
    delay(30);
  }

  // Reset game if reset button is pressed
  if (digitalRead(RESET_BUTTON) == LOW) {
    player1Score = 0;
    player2Score = 0;
    resetGame();
    delay(300); // Debounce delay
  }
}

void resetGame() {
  // Initialize ball position
  ballX = SCREEN_WIDTH / 2;
  ballY = SCREEN_HEIGHT / 2;

  // Randomize ball direction
  ballDirX = random(0, 2) == 0 ? 1 : -1;
  ballDirY = random(0, 2) == 0 ? 1 : -1;
}
