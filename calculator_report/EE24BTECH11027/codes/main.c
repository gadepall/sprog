#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define F_CPU 16000000UL // Define clock frequency for delay.h

// LCD pins: RS, E, D4, D5, D6, D7
#define LCD_RS PD7
#define LCD_EN PD6
#define LCD_D4 PD5
#define LCD_D5 PD4
#define LCD_D6 PD3
#define LCD_D7 PD2

// 10 number buttons (always same connections)
const int buttons[] = {6, 7, 8, 9, 10, 14, 15, 16, 17, 18}; // A0-A4 are 14-18
// Mode buttons:
const int shiftButton = 19;      // A5 is 19
const int extraModeButton = 13;  // Digital pin 13

// Normal digit labels for buttons 0-9
char normalMode[] = {'0','1','2','3','4','5','6','7','8','9'};

// Shift mode mapping (when A5 is pressed):
// For buttons 0-5, operators: order: +, -, *, /, =, backspace (represented by '<')
// For buttons 6-9, standard functions: sin, cos, e^, sqrt.
char shiftOps[]  = {'+', '-', '*', '/', '=', '<'};
const char* shiftFuncs[] = {"sin", "cos", "e^", "sqrt"};

// Extra mode mapping (when digital pin 13 is pressed):
// For buttons 0-4, extra functions: asin, acos, atan, log, ln
const char* extraFuncs[] = {"asin", "acos", "atan", "log", "ln"};

char input[100] = "";
uint8_t shiftActive = 0;
uint8_t extraActive = 0;
uint8_t lastShiftState = 1;      // HIGH is 1
uint8_t lastExtraState = 1;

// Function prototypes
void LCD_Command(unsigned char cmnd);
void LCD_Char(unsigned char data);
void LCD_Init();
void LCD_String(const char* str);
void LCD_Clear();
void handleSpecial(char op);
void updateLCD();
float evaluateFullExpression(const char* expr);
float evaluateExpression(const char* expr);
float mySin(float x);
float myCos(float x);
float myExp(float x);
float mySqrt(float x);
float myAsin(float x);
float myAcos(float x);
float myAtan(float x);
float myLn(float x);
float myLog10(float x);

// Simulate Arduino's pinMode function
void pinMode(int pin, int mode) {
    if (pin >= 0 && pin < 8) {
        if (mode == 2) { // INPUT_PULLUP
            DDRD &= ~(1 << pin);  // Set pin as input
            PORTD |= (1 << pin);  // Enable pull-up resistor
        } else if (mode == 0) { // INPUT
            DDRD &= ~(1 << pin);  // Set pin as input
            PORTD &= ~(1 << pin); // Disable pull-up resistor
        } else if (mode == 1) { // OUTPUT
            DDRD |= (1 << pin);   // Set pin as output
        }
    } else if (pin >= 14 && pin <= 19) {
        // Handle analog pins (A0-A5)
        if (mode == 2) { // INPUT_PULLUP
            DDRC &= ~(1 << (pin - 14));  // Set pin as input
            PORTC |= (1 << (pin - 14));  // Enable pull-up resistor
        } else if (mode == 0) { // INPUT
            DDRC &= ~(1 << (pin - 14));  // Set pin as input
            PORTC &= ~(1 << (pin - 14)); // Disable pull-up resistor
        } else if (mode == 1) { // OUTPUT
            DDRC |= (1 << (pin - 14));   // Set pin as output
        }
    }
}

// Simulate Arduino's digitalRead function
int digitalRead(int pin) {
    if (pin >= 0 && pin < 8) {
        return (PIND & (1 << pin)) ? 1 : 0;
    } else if (pin >= 14 && pin <= 19) {
        return (PINC & (1 << (pin - 14))) ? 1 : 0;
    }
    return 0;
}

void setup() {
    LCD_Init();
    // Setup number buttons (all 10) with internal pull-ups.
    for (int i = 0; i < 10; i++) {
        pinMode(buttons[i], 2); // INPUT_PULLUP
    }
    pinMode(shiftButton, 2); // INPUT_PULLUP
    pinMode(extraModeButton, 2); // INPUT_PULLUP

    LCD_String("Calculator Ready");
    _delay_ms(1000);
    LCD_Clear();
}

void loop() {
    // --- Check mode buttons ---
    // Check shift mode toggle (A5)
    int currentShiftState = digitalRead(shiftButton);
    if (lastShiftState == 1 && currentShiftState == 0) { // falling edge detected
        shiftActive = 1;
    }
    lastShiftState = currentShiftState;

    // Check extra mode toggle (digital pin 13)
    int currentExtraState = digitalRead(extraModeButton);
    if (lastExtraState == 1 && currentExtraState == 0) { // falling edge detected
        extraActive = 1;
    }
    lastExtraState = currentExtraState;

    // --- Process number buttons (0-9) ---
    for (int i = 0; i < 10; i++) {
        if (digitalRead(buttons[i]) == 0) { // button pressed
            _delay_ms(50); // debounce
            if (digitalRead(buttons[i]) == 0) {
                while (digitalRead(buttons[i]) == 0); // wait for release

                // Priority: extra mode > shift mode > normal mode.
                if (extraActive) {
                    // In extra mode, use extra functions for buttons 0-4.
                    if (i < 5) {
                        // Insert the extra function call text, e.g., "asin("
                        strcat(input, extraFuncs[i]);
                        strcat(input, "(");
                    } else {
                        // For buttons 5-9, remain as normal digits.
                        strncat(input, &normalMode[i], 1);
                    }
                    extraActive = 0; // reset extra mode after use.
                } else if (shiftActive) {
                    // In shift mode:
                    if (i < 6) {
                        // For buttons 0-5, use operators/functions from shiftOps.
                        handleSpecial(shiftOps[i]);
                    } else {
                        // For buttons 6-9, use standard function calls.
                        strcat(input, shiftFuncs[i - 6]);
                        strcat(input, "(");
                    }
                    shiftActive = 0; // reset shift mode after use.
                } else {
                    // Normal mode: append digit.
                    strncat(input, &normalMode[i], 1);
                }
                updateLCD();
            }
        }
    }
}

// Process special keys (operators and "=") from shift mode.
void handleSpecial(char op) {
    if (op == '<') { // Backspace
        if (strlen(input) > 0) input[strlen(input) - 1] = '\0';
    } else if (op == '=') {
        // Auto-close any open parenthesis.
        if (strstr(input, "(") != NULL && strstr(input, ")") == NULL) strcat(input, ")");
        char expr[100];
        strcpy(expr, input);
        float result = evaluateFullExpression(input);
        char resultStr[20];
        snprintf(resultStr, sizeof(resultStr), "=%.3f", result);
        strcat(input, resultStr);
        updateLCD();
        _delay_ms(3000);
        input[0] = '\0';
    } else {
        // Operators: +, -, *, /
        strncat(input, &op, 1);
    }
}

void updateLCD() {
    LCD_Clear();
    if (strlen(input) <= 16) {
        LCD_String(input);
    } else {
        char line1[17];
        strncpy(line1, input, 16);
        line1[16] = '\0';
        LCD_String(line1);
        LCD_Command(0xC0); // Move to the second line
        LCD_String(input + 16);
    }
}

// Evaluate full expression: process function calls then evaluate arithmetic.
float evaluateFullExpression(const char* expr) {
    char processedExpr[100];
    strcpy(processedExpr, expr);
    // Process functions here if needed
    return evaluateExpression(processedExpr);
}

// Basic arithmetic evaluator (left-to-right, no operator precedence).
float evaluateExpression(const char* expr) {
    float result = 0.0;
    char lastOp = '+';
    char number[20];
    int numIndex = 0;
    for (int i = 0; i < strlen(expr); i++) {
        char c = expr[i];
        if (isdigit(c) || c == '.') {
            number[numIndex++] = c;
        } else {
            number[numIndex] = '\0';
            numIndex = 0;
            float num = atof(number);
            switch (lastOp) {
                case '+': result += num; break;
                case '-': result -= num; break;
                case '*': result *= num; break;
                case '/': result = (num != 0.0) ? result / num : 0.0; break;
            }
            lastOp = c;
        }
    }
    if (numIndex > 0) {
        number[numIndex] = '\0';
        float num = atof(number);
        switch (lastOp) {
            case '+': result += num; break;
            case '-': result -= num; break;
            case '*': result *= num; break;
            case '/': result = (num != 0.0) ? result / num : 0.0; break;
        }
    }
    return result;
}

// ----- Function Approximations -----

// mySin: input in degrees; convert to radians.
//float mySin(float x) {
//    float rad = x * 3.14159265358979323846 / 180.0;
//    while (rad > 3.14159265358979323846) rad -= 2 * 3.14159265358979323846;
//    while (rad < -3.14159265358979323846) rad += 2 * 3.14159265358979323846;
//    float term = rad, sum = rad;
//    for (int n = 1; n < 10; n++) {
//        term = -term * rad * rad / ((2 * n) * (2 * n + 1));
//        sum += term;
//    }
//    return sum;
//}

#define H 0.001

float mySin(float x_deg) {
    float x = x_deg * PI / 180.0;  // Convert degrees to radians
    float y = 0.0, v = 1.0;  // Initial conditions: sin(0) = 0, cos(0) = 1
    float h = H;

    for (int i = 0; i < (int)(x / h); i++) {
        float k1_y = h * v;
        float k1_v = -h * y;

        float k2_y = h * (v + k1_v / 2);
        float k2_v = -h * (y + k1_y / 2);

        float k3_y = h * (v + k2_v / 2);
        float k3_v = -h * (y + k2_y / 2);

        float k4_y = h * (v + k3_v);
        float k4_v = -h * (y + k3_y);

        y += (k1_y + 2*k2_y + 2*k3_y + k4_y) / 6;
        v += (k1_v + 2*k2_v + 2*k3_v + k4_v) / 6;
    }

    return y;  // Return sin(x)
}

// myCos: input in degrees.
//float myCos(float x) {
//    float rad = x * 3.14159265358979323846 / 180.0;
//    while (rad > 3.14159265358979323846) rad -= 2 * 3.14159265358979323846;
//    while (rad < -3.14159265358979323846) rad += 2 * 3.14159265358979323846;
//    float term = 1, sum = 1;
//    for (int n = 1; n < 10; n++) {
//        term = -term * rad * rad / ((2 * n - 1) * (2 * n));
//        sum += term;
//    }
//    return sum;
//}

float myCos(float x_deg) {
    float x = x_deg * PI / 180.0;  // Convert degrees to radians
    float y = 0.0, v = 1.0;  // Initial conditions: sin(0) = 0, cos(0) = 1
    float h = H;

    for (int i = 0; i < (int)(x / h); i++) {
        float k1_y = h * v;
        float k1_v = -h * y;

        float k2_y = h * (v + k1_v / 2);
        float k2_v = -h * (y + k1_y / 2);

        float k3_y = h * (v + k2_v / 2);
        float k3_v = -h * (y + k2_y / 2);

        float k4_y = h * (v + k3_v);
        float k4_v = -h * (y + k3_y);

        y += (k1_y + 2*k2_y + 2*k3_y + k4_y) / 6;
        v += (k1_v + 2*k2_v + 2*k3_v + k4_v) / 6;
    }

    return v;  // Return cos(x), since v = cos(x)
}
// myExp: Taylor series for e^(x).
//float myExp(float x) {
//    float term = 1, sum = 1;
//    for (int n = 1; n < 15; n++) {
//        term = term * x / n;
//        sum += term;
//    }
//    return sum;
//}

float myExp(float x) {
    float y = 1.0, h = 0.01;
    for (float i = 0; i < x; i += h) {
        float k1 = h * y;
        float k2 = h * (y + k1 / 2);
        float k3 = h * (y + k2 / 2);
        float k4 = h * (y + k3);
        y += (k1 + 2*k2 + 2*k3 + k4) / 6;
    }
    return y;
}

//// mySqrt: Newton's method.
//float mySqrt(float x) {
//    if (x < 0) return -1;
//    float guess = x / 2.0;
//    for (int i = 0; i < 10; i++) {
//        guess = (guess + x / guess) / 2.0;
//    }
//    return guess;
//}

float mySqrt(float x) {
    float y = 1.0, h = 0.01;
    for (float i = 1; i < x; i += h) {
        float k1 = h / (2 * sqrt(i));
        float k2 = h / (2 * sqrt(i + k1/2));
        float k3 = h / (2 * sqrt(i + k2/2));
        float k4 = h / (2 * sqrt(i + k3));
        y += (k1 + 2*k2 + 2*k3 + k4) / 6;
    }
    return y;
}

// myAsin: Approximate arcsin(x) using a simple series (|x|<=1).
//float myAsin(float x) {
//    if (x > 1.0) x = 1.0;
//    if (x < -1.0) x = -1.0;
//    float term = x, sum = x, x2 = x * x;
//    term *= x2 / 6.0;
//    sum += term;
//    term *= x2 * 3.0 / 20.0;
//    sum += term;
//    term *= x2 * 5.0 / 28.0;
//    sum += term;
//    return sum;
//}

float myAsin(float x) {
    float y = 0.0, h = 0.01;
    for (float i = 0; i < x; i += h) {
        float k1 = h / sqrt(1 - i*i);
        float k2 = h / sqrt(1 - (i + k1/2)*(i + k1/2));
        float k3 = h / sqrt(1 - (i + k2/2)*(i + k2/2));
        float k4 = h / sqrt(1 - (i + k3)*(i + k3));
        y += (k1 + 2*k2 + 2*k3 + k4) / 6;
    }
    return y;
}

// myAcos: Use arccos(x) = PI/2 - arcsin(x).
float myAcos(float x) {
    return 3.14159265358979323846 / 2 - myAsin(x);
}

//// myAtan: Taylor series approximation.
//float myAtan(float x) {
//    if (fabs(x) > 1.0) {
//        if (x > 0) return 3.14159265358979323846 / 2 - myAtan(1.0 / x);
//        else return -3.14159265358979323846 / 2 - myAtan(1.0 / x);
//    }
//    float term = x, sum = x, x2 = x * x;
//    for (int n = 1; n < 6; n++) {
//        term *= -x2;
//        sum += term / (2 * n + 1);
//    }
//    return sum;
//}

float myAtan(float x) {
    float y = 0.0, h = 0.01;
    for (float i = 0; i < x; i += h) {
        float k1 = h / (1 + i*i);
        float k2 = h / (1 + (i + k1/2)*(i + k1/2));
        float k3 = h / (1 + (i + k2/2)*(i + k2/2));
        float k4 = h / (1 + (i + k3)*(i + k3));
        y += (k1 + 2*k2 + 2*k3 + k4) / 6;
    }
    return y;
}

// myLn: Natural logarithm approximation (for x>0).
//float myLn(float x) {
//    if (x <= 0) return -1000000;
//    float y = (x - 1) / (x + 1), sum = 0;
//    for (int n = 0; n < 10; n++) {
//        sum += pow(y, 2 * n + 1) / (2 * n + 1);
//    }
//    return 2 * sum;
//}

float myLn(float x) {
    float y = 0.0, h = 0.01;
    for (float i = 1; i < x; i += h) {
        float k1 = h / i;
        float k2 = h / (i + k1/2);
        float k3 = h / (i + k2/2);
        float k4 = h / (i + k3);
        y += (k1 + 2*k2 + 2*k3 + k4) / 6;
    }
    return y;
}


// myLog10: Base-10 logarithm.
float myLog10(float x) {
    return myLn(x) / myLn(10.0);
}

// LCD Functions
void LCD_Command(unsigned char cmnd) {
    PORTD = (PORTD & 0x0F) | (cmnd & 0xF0); // Send upper nibble
    PORTD &= ~(1 << LCD_RS); // RS = 0 for command
    PORTD |= (1 << LCD_EN); // Enable high
    _delay_us(1); // Wait for 1 microsecond
    PORTD &= ~(1 << LCD_EN); // Enable low
    _delay_us(100); // Wait for 100 microseconds

    PORTD = (PORTD & 0x0F) | (cmnd << 4); // Send lower nibble
    PORTD |= (1 << LCD_EN); // Enable high
    _delay_us(1); // Wait for 1 microsecond
    PORTD &= ~(1 << LCD_EN); // Enable low
    _delay_us(100); // Wait for 100 microseconds
}

void LCD_Char(unsigned char data) {
    PORTD = (PORTD & 0x0F) | (data & 0xF0); // Send upper nibble
    PORTD |= (1 << LCD_RS); // RS = 1 for data
    PORTD |= (1 << LCD_EN); // Enable high
    _delay_us(1); // Wait for 1 microsecond
    PORTD &= ~(1 << LCD_EN); // Enable low
    _delay_us(100); // Wait for 100 microseconds

    PORTD = (PORTD & 0x0F) | (data << 4); // Send lower nibble
    PORTD |= (1 << LCD_EN); // Enable high
    _delay_us(1); // Wait for 1 microsecond
    PORTD &= ~(1 << LCD_EN); // Enable low
    _delay_us(100); // Wait for 100 microseconds
}

void LCD_Init() {
    DDRD = 0xFF; // Set PORTD as output
    _delay_ms(20); // Wait for LCD to power up

    // Initialization sequence for 4-bit mode
    LCD_Command(0x02); // 4-bit mode
    LCD_Command(0x28); // 2 lines, 5x7 matrix
    LCD_Command(0x0C); // Display on, cursor off
    LCD_Command(0x06); // Increment cursor (shift cursor to right)
    LCD_Command(0x01); // Clear display
    _delay_ms(2); // Wait for clear display command to complete
}

void LCD_String(const char* str) {
    for (int i = 0; str[i] != 0; i++) {
        LCD_Char(str[i]);
    }
}

void LCD_Clear() {
    LCD_Command(0x01); // Clear display
    _delay_ms(2); // Wait for clear display command to complete
}

int main() {
    setup();
    while (1) {
        loop();
    }
    return 0;
}
