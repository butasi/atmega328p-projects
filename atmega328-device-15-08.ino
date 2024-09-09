#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED reset pin
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin Definitions
#define POT_NAV_PIN A0       // Potentiometer A0 for menu navigation
#define POT_CTRL_PIN A1      // Potentiometer A1 for control
#define BUTTON_OK_PIN 2
#define BUTTON_BACK_PIN 3
#define LED_PIN 13  // Feedback LED connected to pin 13

// Free GPIO pins for output/PWM
int outputPins[] = {A2, A3, A6, A7, 4, 5, 6, 7, 8, 9, 10, 11, 12}; // All pins except A0 and A1
int pwmPins[] = {9, 10}; // PWM pins available on ATmega328P

// State variables
int currentMenu = 0;
int currentSubMenu = 0;
bool inSubMenu = false;
bool pwmControlActive = false;
int selectedPin = -1;
int pwmValue = 0;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// Function Prototypes
void displayMenu();
void handleMainMenu(int potValue);
void handleSubMenu(int potValue);
void handleOkButton();
void handleBackButton();
void provideFeedback();
void executeMenuFunction();
void displayPinControl();
void togglePinState(int pin);
void pwmControl(int pin);
void displayPWMControl(int pwmValue);
void analogReadout(int potValue);
void oscilloscopeFunction();
void displayBarGraph(int value, int maxValue);

void setup() {
  Serial.begin(9600);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();

  // Initialize pin modes
  pinMode(POT_NAV_PIN, INPUT);
  pinMode(POT_CTRL_PIN, INPUT);
  pinMode(BUTTON_OK_PIN, INPUT_PULLUP);
  pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Initialize all output pins
  for (int i = 0; i < sizeof(outputPins) / sizeof(outputPins[0]); i++) {
    pinMode(outputPins[i], OUTPUT);
    digitalWrite(outputPins[i], LOW); // Start with all outputs turned off
  }

  pinMode(A0, INPUT); // A0 as input pin

  // Display the main menu
  displayMenu();
}

void loop() {
  int potNavValue = analogRead(POT_NAV_PIN);
  int potCtrlValue = analogRead(POT_CTRL_PIN);

  bool buttonOkPressed = digitalRead(BUTTON_OK_PIN) == LOW;
  bool buttonBackPressed = digitalRead(BUTTON_BACK_PIN) == LOW;

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonOkPressed) {
      handleOkButton();
      lastDebounceTime = millis();
    }
    if (buttonBackPressed) {
      handleBackButton();
      lastDebounceTime = millis();
    }
  }

  if (!inSubMenu) {
    handleMainMenu(potNavValue);
  } else if (inSubMenu) {
    handleSubMenu(potCtrlValue);
  }

  delay(100);
}

// Handle main menu navigation using POT_NAV_PIN
void handleMainMenu(int potValue) {
  int menuIndex = map(potValue, 0, 1023, 0, 3); // Map to menu items 0-3
  if (menuIndex != currentMenu) {
    currentMenu = menuIndex;
    displayMenu();
  }
}

// Handle sub-menu actions using POT_CTRL_PIN
void handleSubMenu(int potValue) {
  if (pwmControlActive) {
    pwmValue = map(potValue, 0, 1023, 0, 255);
    analogWrite(selectedPin, pwmValue);
    displayPWMControl(pwmValue);
  } else if (currentMenu == 0) { // Pin Control
    int pinIndex = map(potValue, 0, 1023, 0, sizeof(outputPins) / sizeof(outputPins[0]) - 1);
    if (pinIndex != currentSubMenu) {
      currentSubMenu = pinIndex;
      displayPinControl();
    }
  } else if (currentMenu == 2) { // Analog Read
    analogReadout(potValue);
  }
}

// Handle the OK button press
void handleOkButton() {
  provideFeedback();
  if (!inSubMenu) {
    inSubMenu = true;
    executeMenuFunction();
  } else if (!pwmControlActive && currentMenu == 0) {
    togglePinState(outputPins[currentSubMenu]);
    displayPinControl();
  }
}

// Handle the Back button press
void handleBackButton() {
  provideFeedback();
  if (pwmControlActive) {
    pwmControlActive = false; // Exit PWM control mode
  } else if (inSubMenu) {
    inSubMenu = false;
    displayMenu();
  }
}

// Provide feedback by flashing the LED
void provideFeedback() {
  analogWrite(LED_PIN, 180); // 50% brightness for feedback
  delay(100);
  analogWrite(LED_PIN, 0); // Turn off LED
}

// Display the main menu
void displayMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("Main Menu");

  display.setCursor(0, 10);
  display.print(currentMenu == 0 ? "-> " : "   ");
  display.println("Pin Control");

  display.setCursor(0, 20);
  display.print(currentMenu == 1 ? "-> " : "   ");
  display.println("PWM Control");

  display.setCursor(0, 30);
  display.print(currentMenu == 2 ? "-> " : "   ");
  display.println("Analog Read");

  display.setCursor(0, 40);
  display.print(currentMenu == 3 ? "-> " : "   ");
  display.println("Oscilloscope");

  display.display();
}

// Execute the selected menu function
void executeMenuFunction() {
  switch (currentMenu) {
    case 0:
      displayPinControl();
      break;
    case 1:
      pwmControlActive = true;
      selectedPin = pwmPins[0]; // Set to the first PWM pin by default
      displayPWMControl(pwmValue);
      break;
    case 2:
      analogReadout(analogRead(POT_NAV_PIN)); // Use POT_NAV_PIN for analog read adjustment
      break;
    case 3:
      oscilloscopeFunction();
      break;
  }
}

// Display the pin control interface
void displayPinControl() {
  int pin = outputPins[currentSubMenu];
  int value = digitalRead(pin);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Pin Control");

  display.setTextSize(2);
  display.setCursor(0, 9);
  display.print("Pin ");
  display.print(pin);
  display.print(": ");
  display.setCursor(45, 27);
  display.setTextSize(3);
  display.print(value ? "HIGH" : "LOW");

  display.display();
}

// Toggle the state of a pin
void togglePinState(int pin) {
  int currentState = digitalRead(pin);
  digitalWrite(pin, !currentState); // Toggle the pin state
}

// PWM Control interface
void displayPWMControl(int pwmValue) {
  int value = map(pwmValue, 0, 1023, 0, 1023);
  float voltage = (value / 1023.0) * 5.0;

  display.clearDisplay();
  display.setTextSize(2.4);
  display.setCursor(3, 12);
  display.print("PWM: ");
  display.setCursor(0, 39);
  display.setTextSize(1);
  display.print("Voltage: ");
  display.setCursor(48, 36);
  display.setTextSize(2);
  display.print(voltage, 2);
  display.print("V");
  display.setTextSize(4);
  display.setCursor(48, 3);
  display.print(pwmValue);
  displayBarGraph(pwmValue, 255);
  display.display();
}

// Analog readout function
void analogReadout(int potValue) {
  int value = map(potValue, 0, 1023, 0, 1023);
  float voltage = (value / 1023.0) * 5.0;

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Analog Read");

  display.setCursor(0, 10);
  display.print("Value: ");
  display.print(value);

  display.setCursor(0, 20);
  display.print("Voltage: ");
  display.print(voltage, 2);
  display.print("V");

  displayBarGraph(value, 1023);

  display.display();
}

// Simple oscilloscope function
void oscilloscopeFunction() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Oscilloscope Mode");

  int valueA0 = analogRead(A0);
  int valueA1 = analogRead(A1);

  display.setCursor(0, 10);
  display.print("A0: ");
  display.print(valueA0);

  display.setCursor(0, 20);
  display.print("A1: ");
  display.print(valueA1);

  display.display();
}

// Display a bar graph on the OLED
void displayBarGraph(int value, int maxValue) {
  int barLength = map(value, 0, maxValue, 0, SCREEN_WIDTH);

  display.fillRect(0, SCREEN_HEIGHT - 10, barLength, 10, SSD1306_WHITE);
  display.display();
}
