#include <Arduino.h>
#include <LiquidCrystal.h>

// LCD Pins
LiquidCrystal lcd(12, 13, A0, A1, A2, A3);

// Declare function prototypes
void updateClock();
void displayTime();
void displayDigit(int controlPin, int number);
void updateStopwatch();
void updateTimer();
void handleButtons();
void resetStopwatch();
void resetTimer();
void displayDateAndDay();
int calculateDayIndex(int day, int month, int year);

// Constants
const int segmentPins[] = {2, 3, 4, 5};  // ABCD inputs for 7447
const int controlPins[] = {6, 7, 8, 9, 10, 11}; // Common pins for six displays (hour tens, hour ones, etc.)
const int buttonPins[] = {A4, A5,  1}; // Buttons: clock, stopwatch, timer

// Time variables
int hours = 12, minutes = 0, seconds = 0;
int stopwatchHours = 0, stopwatchMinutes = 0, stopwatchSeconds = 0;
int timerHours = 0, timerMinutes = 0, timerSeconds = 30; // Set timer start value here

// Date Variables (Input the date manually here)
int day = 19, month = 3, year = 2025;  // Hardcoded date for LCD
String daysOfWeek = "SMTWTFS";         // Days of the week

// Mode variables
enum Mode { CLOCK, STOPWATCH, TIMER };
Mode currentMode = CLOCK;
bool isPaused = false;

void setup() {
  // Set segment pins as output
  for (int i = 0; i < 4; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], LOW); // Ensure all segment pins start low
  }

  // Set control pins as output
  for (int i = 0; i < 6; i++) {
    pinMode(controlPins[i], OUTPUT);
    digitalWrite(controlPins[i], LOW); // Ensure all control pins start low
  }

  // Set button pins as input with pull-down resistors
  for (int i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // LCD Initialization
  lcd.begin(16, 2);
  displayDateAndDay(); // Display the initial date and day
}

void loop() {
  static unsigned long previousMillis = 0;
  const unsigned long interval = 1000; // 1-second interval

  // Handle button presses
  handleButtons();

  // Update the clock independently of the current mode
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    updateClock();

    // Update the active mode (if not paused)
    if (!isPaused) {
      switch (currentMode) {
        case STOPWATCH:
          updateStopwatch();
          break;
        case TIMER:
          updateTimer();
          break;
        default:
          break; // CLOCK mode doesn't need additional updates here
      }
    }
  }

  // Continuously refresh the displays
  displayTime();
}

void updateClock() {
  seconds++;
  if (seconds >= 60) {
    seconds = 0;
    minutes++;
    if (minutes >= 60) {
      minutes = 0;
      hours++;
      if (hours >= 24) {
        hours = 0;
      }
    }
  }
}

void updateStopwatch() {
  stopwatchSeconds++;
  if (stopwatchSeconds >= 60) {
    stopwatchSeconds = 0;
    stopwatchMinutes++;
    if (stopwatchMinutes >= 60) {
      stopwatchMinutes = 0;
      stopwatchHours++;
    }
  }
}

void updateTimer() {
  if (timerHours > 0 || timerMinutes > 0 || timerSeconds > 0) {
    if (timerSeconds == 0) {
      timerSeconds = 59;
      if (timerMinutes == 0) {
        timerMinutes = 59;
        timerHours--;
      } else {
        timerMinutes--;
      }
    } else {
      timerSeconds--;
    }
  }
}

void handleButtons() {
  static bool button1Pressed = false, button2Pressed = false, button3Pressed = false;

  if (digitalRead(buttonPins[0]) == LOW && !button1Pressed) { // Button 1: Show clock
    currentMode = CLOCK;
    isPaused = false;
    resetStopwatch();
    resetTimer();
    button1Pressed = true;
  }
  if (digitalRead(buttonPins[1]) == LOW && !button2Pressed) { // Button 2: Start stopwatch
    currentMode = STOPWATCH;
    isPaused = false;
    button2Pressed = true;
  }
  if (digitalRead(buttonPins[2]) == LOW && !button3Pressed) { // Button 3: Start timer
    currentMode = TIMER;
    isPaused = false;
    button3Pressed = true;
  }

  // Debouncing: Reset button press flags when buttons are released
  if (digitalRead(buttonPins[0]) == HIGH) button1Pressed = false;
  if (digitalRead(buttonPins[1]) == HIGH) button2Pressed = false;
  if (digitalRead(buttonPins[2]) == HIGH) button3Pressed = false;
}

void resetStopwatch() {
  stopwatchHours = 0;
  stopwatchMinutes = 0;
  stopwatchSeconds = 0;
}

void resetTimer() {
  timerHours = 0;
  timerMinutes = 0;
  timerSeconds = 30; // Default start value
}

void displayTime() {
  int displayNumbers[6] = {0};

  switch (currentMode) {
    case CLOCK: // Show current time
      displayNumbers[0] = hours / 10;
      displayNumbers[1] = hours % 10;
      displayNumbers[2] = minutes / 10;
      displayNumbers[3] = minutes % 10;
      displayNumbers[4] = seconds / 10;
      displayNumbers[5] = seconds % 10;
      break;
    case STOPWATCH: // Show stopwatch time
      displayNumbers[0] = stopwatchHours / 10;
      displayNumbers[1] = stopwatchHours % 10;
      displayNumbers[2] = stopwatchMinutes / 10;
      displayNumbers[3] = stopwatchMinutes % 10;
      displayNumbers[4] = stopwatchSeconds / 10;
      displayNumbers[5] = stopwatchSeconds % 10;
      break;
    case TIMER: // Show timer countdown
      displayNumbers[0] = timerHours / 10;
      displayNumbers[1] = timerHours % 10;
      displayNumbers[2] = timerMinutes / 10;
      displayNumbers[3] = timerMinutes % 10;
      displayNumbers[4] = timerSeconds / 10;
      displayNumbers[5] = timerSeconds % 10;
      break;
  }

  // Refresh all six digits rapidly
  for (int i = 0; i < 6; i++) {
    displayDigit(controlPins[i], displayNumbers[i]);
    delay(1); // Short delay to stabilize display
  }
}

void displayDigit(int controlPin, int number) {
  // Clear all control pins
  for (int i = 0; i < 6; i++) {
    digitalWrite(controlPins[i], LOW);
  }

  // Clear all segment pins
  for (int i = 0; i < 4; i++) {
    digitalWrite(segmentPins[i], LOW);
  }

  // Set the active control pin
  digitalWrite(controlPin, HIGH);

  // Set the ABCD pins to the binary equivalent of the number
  for (int i = 0; i < 4; i++) {
    digitalWrite(segmentPins[i], (number >> i) & 1);
  }
}

void displayDateAndDay() {
  // Clear the LCD and set the cursor for the top row
  lcd.clear();
  lcd.setCursor(0, 0);

  // Display the date in DD/MM/YYYY format
  lcd.print(day < 10 ? "0" : ""); lcd.print(day);
  lcd.print("/");
  lcd.print(month < 10 ? "0" : ""); lcd.print(month);
  lcd.print("/");
  lcd.print(year);

  // Display the bottom row (days of the week)
  lcd.setCursor(0, 1);
  lcd.print("S M T W T F S");

  // Highlight the current day of the week
  int dayIndex = calculateDayIndex(day, month, year); // Calculate the day of the week
  lcd.setCursor(dayIndex * 2, 1); // Each day is 2 characters wide on the LCD
  lcd.blink(); // Highlight the current day
}

int calculateDayIndex(int day, int month, int year) {
  // Zeller's Congruence for calculating day of the week
  if (month < 3) {
    month += 12;
    year--;
  }

  int k = year % 100;          // Year of the century
  int j = year / 100;          // Zero-based century
  int dayOfWeek = (day + (13 * (month + 1)) / 5 + k + (k / 4) + (j / 4) - (2 * j)) % 7;

  return (dayOfWeek + 5) % 7; // Adjust to match SMTWTFS (Sunday = 0, Monday = 1, ...)
}
