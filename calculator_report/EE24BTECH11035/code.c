/*
 * Embedded C Calculator for AVR (using avr-gcc)
 *
 * This code is a port of an Arduino-style calculator to plain C.
 * It uses a simple recursive descent parser to evaluate expressions
 * entered via buttons and displays the result on an LCD.
 *
 * Math functions (sin, cos, exp, sqrt, ln, log) are implemented using
 * iterative (difference equation) methods.
 *
 * NOTE: You must supply an LCD driver (lcd.h) that provides lcd_init(),
 * lcd_clear(), and lcd_print(char*) functions. Also, adapt the button
 * reading functions to your hardware.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "lcd.h"   // Your LCD driver header

#ifndef PI
  #define PI 3.14159265358979323846
#endif

// -------------------------
// Hardware Definitions
// -------------------------
// (Define your port and pin mappings for buttons here.)
//
// For example, assume:
// - Buttons are connected to PORTD bits 0..7 (adjust as needed)
// - shiftButton is connected to a specific bit of PORTB, etc.
//
// Here we define placeholder macros. Adjust these to your actual hardware.
#define BUTTON_PORT   PIND
#define SHIFT_PORT    PINB
#define EXTRA_PORT    PINC

// Example pin masks (adjust according to your wiring)
#define BUTTON_MASK(i)    (1 << (i))  // buttons[0] uses bit0, etc.
#define SHIFT_BUTTON_MASK (1 << 0)    // shift button on PORTB bit0
#define EXTRA_BUTTON_MASK (1 << 0)    // extra mode button on PORTC bit0

// -------------------------
// Calculator Global Variables
// -------------------------
#define INPUT_SIZE 128
char input[INPUT_SIZE] = "";    // our input string buffer

// Instead of bool from Arduino, we use stdbool.h:
bool shiftActive = false;
bool extraActive = false;

// These variables hold the previous state of the buttons
bool lastShiftState = true;
bool lastExtraState = true;

// -------------------------
// Mapping for Keys
// -------------------------
char normalMode[10] = {'0','1','2','3','4','5','6','7','8','9'};
char shiftOps[6]  = {'+', '-', '*', '/', '=', '<'};
/* For functions, we store constant strings.
   When a function key is pressed, the corresponding function name (with an opening parenthesis)
   is appended to the input string.
*/
const char* shiftFuncs[4] = {"sin", "cos", "e^", "sqrt"};
const char* extraFuncs[8] = {"asin", "acos", "atan", "log", "ln", "(", ")", "^"};

// -------------------------
// Function Prototypes
// -------------------------
void updateLCD(void);
void handleSpecial(char op);
float evaluateExpression(const char* expr);

float parseExpression(const char* s, int *pos);
float parseTerm(const char* s, int *pos);
float parseFactor(const char* s, int *pos);
float parsePrimary(const char* s, int *pos);
void skipSpaces(const char* s, int *pos);

// Math functions using iterative/difference-equation methods:
float mySin(float x);
float myCos(float x);
float myExp(float x);
float mySqrt(float x);
float myLn(float x);
float myLog(float x);

// -------------------------
// Button Reading Functions (Placeholders)
// -------------------------
bool button_pressed(uint8_t pinMask) {
    // Check if the given button is pressed.
    // You must implement this to read from the appropriate port.
    // For example, if buttons are active low:
    return !(BUTTON_PORT & pinMask);
}

bool shift_button_pressed(void) {
    return !(SHIFT_PORT & SHIFT_BUTTON_MASK);
}

bool extra_button_pressed(void) {
    return !(EXTRA_PORT & EXTRA_BUTTON_MASK);
}

// -------------------------
// Math Functions Definitions
// -------------------------

// mySin: approximate sine (input in degrees) via Taylor series
float mySin(float x){
  float rad = x * PI / 180.0;
  float term = rad;  // first term
  float sum = rad;
  for (int n = 1; n < 10; n++){
    term = -term * rad * rad / ((2 * n) * (2 * n + 1));
    sum += term;
  }
  return sum;
}

// myCos: approximate cosine (input in degrees) via Taylor series
float myCos(float x){
  float rad = x * PI / 180.0;
  float term = 1;
  float sum = 1;
  for (int n = 1; n < 10; n++){
    term = -term * rad * rad / ((2 * n - 1) * (2 * n));
    sum += term;
  }
  return sum;
}

// myExp: approximate exponential using Euler's method (difference equation)
// Solve dy/dx = y, y(0)=1 using N steps.
float myExp(float x){
  int N = 100;          // number of steps; adjust for accuracy
  float h = x / N;      // step size
  float y = 1.0;        // initial condition
  for (int i = 0; i < N; i++){
    y = y * (1 + h);
  }
  return y;
}

// mySqrt: approximate square root using Newton's method.
float mySqrt(float x){
  if (x < 0) return -1;
  float guess = x / 2.0;
  for (int i = 0; i < 10; i++){
    guess = (guess + x / guess) / 2.0;
  }
  return guess;
}

// myLn: approximate natural logarithm using series expansion.
// Valid for x > 0. (Series converges better if x is near 1.)
float myLn(float x){
  if (x <= 0) return -1000000; // error value for non-positive x
  float y = (x - 1) / (x + 1);
  float sum = 0;
  for (int n = 0; n < 15; n++){
    sum += pow(y, 2 * n + 1) / (2 * n + 1);
  }
  return 2 * sum;
}

// myLog: approximate base-10 logarithm using myLn.
float myLog(float x){
  return myLn(x) / myLn(10.0);
}

// -------------------------
// Recursive Descent Parser Functions
// -------------------------
void skipSpaces(const char* s, int *pos) {
  while (s[*pos] == ' ' || s[*pos] == '\t')
    (*pos)++;
}

float parseExpression(const char* s, int *pos) {
  skipSpaces(s, pos);
  float value = parseTerm(s, pos);
  skipSpaces(s, pos);
  while (s[*pos] == '+' || s[*pos] == '-') {
    char op = s[(*pos)++];
    float term = parseTerm(s, pos);
    if (op == '+') value += term;
    else value -= term;
    skipSpaces(s, pos);
  }
  return value;
}

float parseTerm(const char* s, int *pos) {
  skipSpaces(s, pos);
  float value = parseFactor(s, pos);
  skipSpaces(s, pos);
  while (s[pos] == '' || s[*pos] == '/') {
    char op = s[(*pos)++];
    float factor = parseFactor(s, pos);
    if (op == '*') value *= factor;
    else value /= factor;
    skipSpaces(s, pos);
  }
  return value;
}

float parseFactor(const char* s, int *pos) {
  skipSpaces(s, pos);
  float base = parsePrimary(s, pos);
  skipSpaces(s, pos);
  if (s[*pos] == '^') {
    (*pos)++;
    float exponent = parseFactor(s, pos);
    base = pow(base, exponent);
  }
  return base;
}

float parsePrimary(const char* s, int *pos) {
  skipSpaces(s, pos);
  float value = 0;
  if (s[*pos] == '(') {
    (*pos)++; // skip '('
    value = parseExpression(s, pos);
    if (s[*pos] == ')') (*pos)++;
  }
  else if (s[*pos] == 'e') {
    value = 2.718281828459045;
    (*pos)++;
  }
  else if (strncmp(&s[*pos], "pi", 2) == 0) {
    value = 3.141592653589793;
    (*pos) += 2;
  }
  else {
    // Read a number from the string
    char buffer[20];
    int i = 0;
    while ((s[*pos] >= '0' && s[*pos] <= '9') || s[*pos] == '.') {
      buffer[i++] = s[(*pos)++];
    }
    buffer[i] = '\0';
    value = atof(buffer);
  }
  skipSpaces(s, pos);
  return value;
}

float evaluateExpression(const char* expr) {
  int pos = 0;
  return parseExpression(expr, &pos);
}

// -------------------------
// LCD Update Function
// -------------------------
void updateLCD(void) {
  lcd_clear();
  // For simplicity, assume lcd_print prints to the first line.
  // If your LCD supports two lines, you can split the string.
  lcd_print(input);
}

// -------------------------
// Button/Key Handling Functions
// -------------------------
void handleSpecial(char op) {
  if (op == '<') {
    // Backspace: if input ends with a function name (e.g., "sin(") remove it entirely.
    const char* functions[] = {"sin(", "cos(", "e^(", "sqrt(", "asin(", "acos(", "atan(", "log(", "ln("};
    bool removed = false;
    for (int i = 0; i < 9; i++) {
      size_t flen = strlen(functions[i]);
      if (strlen(input) >= flen && strcmp(&input[strlen(input) - flen], functions[i]) == 0) {
        input[strlen(input) - flen] = '\0';
        removed = true;
        break;
      }
    }
    if (!removed && strlen(input) > 0) {
      input[strlen(input) - 1] = '\0';
    }
  }
  else if (op == '=') {
    char expr[INPUT_SIZE];
    strcpy(expr, input);
    float result = evaluateExpression(expr);
    char resultStr[32];
    dtostrf(result, 0, 3, resultStr);  // convert float to string with 3 decimal places
    strcat(input, "=");
    strcat(input, resultStr);
    updateLCD();
    _delay_ms(3000);
    input[0] = '\0';  // clear input
  }
  else {
    size_t len = strlen(input);
    if (len < INPUT_SIZE - 2) {
      input[len] = op;
      input[len+1] = '\0';
    }
  }
  updateLCD();
}

// -------------------------
// Main Function
// -------------------------
int main(void) {
  // Initialize LCD (your lcd_init() must be provided in lcd.h)
  lcd_init();
  lcd_clear();
  lcd_print("Calculator Ready");
  _delay_ms(1000);
  lcd_clear();
  
  // Initialize button ports here (set as input with pull-ups)
  // You must write these initializations according to your hardware.
  // For example:
  // DDRD &= ~(0xFF); // set PORTD pins as input for buttons
  // PORTD |= 0xFF;   // enable pull-ups on PORTD
  // (Similar for SHIFT and EXTRA buttons.)
  
  // Main loop:
  while (1) {
    // Read shift button state
    bool currentShiftState = shift_button_pressed();
    if (lastShiftState && !currentShiftState) {
      shiftActive = true;
    }
    lastShiftState = currentShiftState;
    
    // Read extra mode button state
    bool currentExtraState = extra_button_pressed();
    if (lastExtraState && !currentExtraState) {
      extraActive = true;
    }
    lastExtraState = currentExtraState;
    
    // Process numeric buttons (assuming 10 buttons, mapped to bits 0..9 on BUTTON_PORT)
    // You must adjust this loop to match how your buttons are wired.
    for (uint8_t i = 0; i < 10; i++) {
      if (button_pressed(BUTTON_MASK(i))) {
        _delay_ms(50);
        if (button_pressed(BUTTON_MASK(i))) {
          while (button_pressed(BUTTON_MASK(i)));  // wait until released
          
          if (extraActive) {
            if (i < 5) {
              strcat(input, extraFuncs[i]);
              strcat(input, "(");
            }
            else if (i == 5 || i == 6) {
              strcat(input, extraFuncs[i]);
            }
            else if (i == 7) {
              strcat(input, "^");
            }
            else {
              size_t len = strlen(input);
              input[len] = normalMode[i];
              input[len+1] = '\0';
            }
            extraActive = false;
          }
          else if (shiftActive) {
            if (i < 6) {
              handleSpecial(shiftOps[i]);
            }
            else {
              strcat(input, shiftFuncs[i - 6]);
              strcat(input, "(");
            }
            shiftActive = false;
          }
          else {
            size_t len = strlen(input);
            input[len] = normalMode[i];
            input[len+1] = '\0';
          }
          updateLCD();
        }
      }
    }
    
    // You may wish to add a small delay here to reduce CPU usage.
    _delay_ms(10);
  }
  
  return 0;
}
