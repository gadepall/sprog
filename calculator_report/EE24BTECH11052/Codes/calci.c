#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef PI
  #define PI 3.14159265358979323846
#endif

// TYPEDEFS
typedef uint8_t byte;

// ------------------
// LCD DRIVER ROUTINES
//
#define ClearBit(x,y) x &= ~_BV(y)
#define SetBit(x,y) x |= _BV(y)
#define LCD_RS 2
#define LCD_E 3
#define DAT4 4
#define DAT5 5
#define DAT6 6
#define DAT7 7
#define CLEARDISPLAY 0x01

void PulseEnableLine() {
    SetBit(PORTD, LCD_E);
    _delay_us(40);
    ClearBit(PORTD, LCD_E);
}

void SendNibble(byte data) {
    PORTD &= 0x03;
    if (data & _BV(4)) SetBit(PORTD, DAT4);
    if (data & _BV(5)) SetBit(PORTD, DAT5);
    if (data & _BV(6)) SetBit(PORTD, DAT6);
    if (data & _BV(7)) SetBit(PORTD, DAT7);
    PulseEnableLine();
}

void SendByte(byte data) {
    SendNibble(data);
    SendNibble(data << 4);
}

void LCD_Cmd(byte cmd) {
    ClearBit(PORTD, LCD_RS);
    SendByte(cmd);
}

void LCD_Char(byte ch) {
    SetBit(PORTD, LCD_RS);
    SendByte(ch);
}

void LCD_Init() {
    LCD_Cmd(0x33);
    LCD_Cmd(0x32);
    LCD_Cmd(0x28);
    LCD_Cmd(0x0C);
    LCD_Cmd(0x06);
    LCD_Cmd(0x01);
    _delay_ms(3);
}

void LCD_Clear() {
    LCD_Cmd(CLEARDISPLAY);
    _delay_ms(3);
}

void LCD_Message(const char *text) {
    while (*text)
        LCD_Char(*text++);
}

void LCD_Integer(int data) {
    char st[8] = "";
    itoa(data, st, 10);
    LCD_Message(st);
}

// Button and LCD pin definitions
const int buttons[] = {PB0, PB1, PB2, PB3, PB4, PC0, PC1, PC2, PC3, PC4};
const int shiftButton = PC5;
const int extraModeButton = PB5;

char normalMode[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
char shiftOps[] = {'+', '-', '*', '/', '=', '<'};
const char *shiftFuncs[] = {"sin", "cos", "e^", "sqrt"};
const char *extraFuncs[] = {"asin", "acos", "atan", "log", "ln", "(", ")", "^"};

char input[32] = "";
uint8_t shiftActive = 0;
uint8_t extraActive = 0;
uint8_t lastShiftState = 1;
uint8_t lastExtraState = 1;

// Function prototypes
void updateLCD();
void handleSpecial(char op);
float parseExpression(const char *s, int *pos);
float parseTerm(const char *s, int *pos);
float parseFactor(const char *s, int *pos);
float parsePrimary(const char *s, int *pos);
void skipSpaces(const char *s, int *pos);
float evaluateExpression(const char *expr);

// Custom math functions without math.h
float my_sqrt(float x);
float my_sin(float x);
float my_cos(float x);
float my_tan(float x);
float my_asin(float x);
float my_acos(float x);
float my_atan(float x);
float my_exp(float x);
float my_log(float x);
float my_ln(float x);
float my_pow(float base, float exponent);

// Square root using Babylonian method
float my_sqrt(float x) {
    if (x < 0) return 0;
    if (x == 0 || x == 1) return x;
    
    float guess = x / 2;
    float prev_guess;
    
    do {
        prev_guess = guess;
        guess = (guess + x / guess) / 2;
    } while (prev_guess - guess > 0.0001 || guess - prev_guess > 0.0001);
    
    return guess;
}

// Sine function using Taylor series expansion
float my_sin(float x) {
    // Normalize x to [-PI, PI]
    while (x > PI) x -= 2 * PI;
    while (x < -PI) x += 2 * PI;
    
    float result = 0;
    float term = x;
    int n = 1;
    
    for (int i = 0; i < 10; i++) {
        result += term;
        term *= -x * x / ((2 * n) * (2 * n + 1));
        n++;
    }
    
    return result;
}

// Cosine function using Taylor series expansion
float my_cos(float x) {
    // Normalize x to [-PI, PI]
    while (x > PI) x -= 2 * PI;
    while (x < -PI) x += 2 * PI;
    
    float result = 0;
    float term = 1;
    int n = 1;
    
    for (int i = 0; i < 10; i++) {
        result += term;
        term *= -x * x / ((2 * n - 1) * (2 * n));
        n++;
    }
    
    return result;
}

// Tangent function
float my_tan(float x) {
    float cosx = my_cos(x);
    if (cosx == 0) return 0; // Avoid division by zero
    return my_sin(x) / cosx;
}

// Arcsine function using Taylor series
float my_asin(float x) {
    if (x < -1 || x > 1) return 0;
    
    float result = x;
    float term = x;
    float x_squared = x * x;
    int n = 1;
    
    for (int i = 1; i < 20; i++) {
        term *= x_squared * (2 * n - 1) * (2 * n - 1) / (2 * n * (2 * n + 1));
        result += term;
        n++;
    }
    
    return result;
}

// Arccosine function using arcsine identity
float my_acos(float x) {
    return PI / 2 - my_asin(x);
}

// Arctangent function using Taylor series
float my_atan(float x) {
    if (x < -1 || x > 1) {
        return (x > 0 ? 1 : -1) * PI / 2 - my_atan(1 / x);
    }
    
    float result = x;
    float term = x;
    float x_squared = x * x;
    int n = 1;
    
    for (int i = 1; i < 20; i++) {
        term *= -x_squared;
        result += term / (2 * n + 1);
        n++;
    }
    
    return result;
}

// Exponential function using Taylor series
float my_exp(float x) {
    float result = 1;
    float term = 1;
    
    for (int i = 1; i < 20; i++) {
        term *= x / i;
        result += term;
    }
    
    return result;
}

// Base-10 logarithm using natural log
float my_log(float x) {
    if (x <= 0) return 0;
    return my_ln(x) / 2.302585092994046; // ln(10)
}

// Natural logarithm using Taylor series
float my_ln(float x) {
    if (x <= 0) return 0;
    
    // First normalize x to be near 1
    float result = 0;
    while (x >= 1.5) {
        x /= 2.718281828459045; // e
        result += 1;
    }
    while (x <= 0.5) {
        x *= 2.718281828459045;
        result -= 1;
    }
    
    x -= 1;
    float term = x;
    float x_pow = x;
    int n = 2;
    
    for (int i = 1; i < 20; i++) {
        result += term;
        x_pow *= -x;
        term = x_pow / n;
        n++;
    }
    
    return result;
}

// Power function using exponential and logarithm
float my_pow(float base, float exponent) {
    if (base <= 0) return 0;
    return my_exp(exponent * my_ln(base));
}

void handleSpecial(char op) {
    if (op == '<') {
        if (strlen(input) > 0) {
            uint8_t removed = 0;
            const char *functions[] = {"sin(", "cos(", "e^(", "sqrt(", "asin(", "acos(", "atan(", "log(", "ln("};
            for (uint8_t i = 0; i < 9; i++) {
                if (strstr(input, functions[i]) == input + strlen(input) - strlen(functions[i])) {
                    input[strlen(input) - strlen(functions[i])] = '\0';
                    removed = 1;
                    break;
                }
            }
            if (!removed) {
                input[strlen(input) - 1] = '\0';
            }
        }
    } else if (op == '=') {
        char expr[32];
        strcpy(expr, input);
        float result = evaluateExpression(input);
        char resultStr[16];
        dtostrf(result, 3, 3, resultStr);
        strcat(input, "=");
        strcat(input, resultStr);
        updateLCD();
        _delay_ms(3000);
        input[0] = '\0';
    } else {
        strncat(input, &op, 1);
    }
    updateLCD();
}

void setup() {
    LCD_Init();
    for (int i = 0; i < 10; i++) {
        if (buttons[i] >= 8) {
            DDRC &= ~_BV(buttons[i] - 8);
            PORTC |= _BV(buttons[i] - 8);
        } else {
            DDRB &= ~_BV(buttons[i]);
            PORTB |= _BV(buttons[i]);
        }
    }
    DDRC &= ~_BV(shiftButton - 8);
    PORTC |= _BV(shiftButton - 8);
    DDRB &= ~_BV(extraModeButton);
    PORTB |= _BV(extraModeButton);
    LCD_Message("Calculator Ready");
    _delay_ms(1000);
    LCD_Clear();
}

void loop() {
    uint8_t currentShiftState = (PINC & _BV(shiftButton - 8)) ? 1 : 0;
    if (lastShiftState == 1 && currentShiftState == 0) {
        shiftActive = 1;
    }
    lastShiftState = currentShiftState;

    uint8_t currentExtraState = (PINB & _BV(extraModeButton)) ? 1 : 0;
    if (lastExtraState == 1 && currentExtraState == 0) {
        extraActive = 1;
    }
    lastExtraState = currentExtraState;

    for (int i = 0; i < 10; i++) {
        uint8_t buttonState = (buttons[i] >= 8) ? (PINC & _BV(buttons[i] - 8)) : (PINB & _BV(buttons[i]));
        if (buttonState == 0) {
            _delay_ms(50);
            buttonState = (buttons[i] >= 8) ? (PINC & _BV(buttons[i] - 8)) : (PINB & _BV(buttons[i]));
            if (buttonState == 0) {
                while (((buttons[i] >= 8) ? (PINC & _BV(buttons[i] - 8)) : (PINB & _BV(buttons[i]))) == 0);

                if (extraActive) {
                    if (i < 5) {
                        strcat(input, extraFuncs[i]);
                        strcat(input, "(");
                    } else if (i == 5 || i == 6) {
                        strcat(input, extraFuncs[i]);
                    } else if (i == 7) {
                        strcat(input, "^");
                    } else {
                        strncat(input, &normalMode[i], 1);
                    }
                    extraActive = 0;
                } else if (shiftActive) {
                    if (i < 6) {
                        handleSpecial(shiftOps[i]);
                    } else {
                        strcat(input, shiftFuncs[i - 6]);
                        strcat(input, "(");
                    }
                    shiftActive = 0;
                } else {
                    strncat(input, &normalMode[i], 1);
                }
                updateLCD();
            }
        }
    }
}

void updateLCD() {
    LCD_Clear();
    if (strlen(input) <= 16) {
        LCD_Message(input);
    } else {
        char line1[17];
        strncpy(line1, input, 16);
        line1[16] = '\0';
        LCD_Message(line1);
        LCD_Cmd(0xC0); // Move cursor to second line
        char line2[17];
        strncpy(line2, input + 16, 16);
        line2[16] = '\0';
        LCD_Message(line2);
    }
}

void skipSpaces(const char *s, int *pos) {
    while (s[*pos] == ' ' || s[*pos] == '\t') (*pos)++;
}

float parseExpression(const char *s, int *pos) {
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

float parseTerm(const char *s, int *pos) {
    skipSpaces(s, pos);
    float value = parseFactor(s, pos);
    skipSpaces(s, pos);
    while (s[*pos] == '*' || s[*pos] == '/') {
        char op = s[(*pos)++];
        float factor = parseFactor(s, pos);
        if (op == '*') value *= factor;
        else value /= factor;
        skipSpaces(s, pos);
    }
    return value;
}

float parseFactor(const char *s, int *pos) {
    skipSpaces(s, pos);
    float base = parsePrimary(s, pos);
    skipSpaces(s, pos);
    if (s[*pos] == '^') {
        (*pos)++;
        float exponent = parseFactor(s, pos);
        base = my_pow(base, exponent);
    }
    return base;
}

float parsePrimary(const char *s, int *pos) {
    skipSpaces(s, pos);
    float value = 0;

    if (s[*pos] == '(') {
        (*pos)++;
        value = parseExpression(s, pos);
        if (s[*pos] == ')') (*pos)++;
    } else if (s[*pos] == 'e') { // Euler's number 'e'
        value = 2.718281828459045;
        (*pos)++;
    } else if (strncmp(&s[*pos], "pi", 2) == 0) { // Recognize 'pi'
        value = 3.141592653589793;
        (*pos) += 2;
    } else if (strncmp(&s[*pos], "sin(", 4) == 0) {
        (*pos) += 4;
        value = my_sin(parseExpression(s, pos));
        if (s[*pos] == ')') (*pos)++;
    } else if (strncmp(&s[*pos], "cos(", 4) == 0) {
        (*pos) += 4;
        value = my_cos(parseExpression(s, pos));
        if (s[*pos] == ')') (*pos)++;
    } else if (strncmp(&s[*pos], "tan(", 4) == 0) {
        (*pos) += 4;
        value = my_tan(parseExpression(s, pos));
        if (s[*pos] == ')') (*pos)++;
    } else if (strncmp(&s[*pos], "asin(", 5) == 0) {
        (*pos) += 5;
        value = my_asin(parseExpression(s, pos));
        if (s[*pos] == ')') (*pos)++;
    } else if (strncmp(&s[*pos], "acos(", 5) == 0) {
        (*pos) += 5;
        value = my_acos(parseExpression(s, pos));
        if (s[*pos] == ')') (*pos)++;
    } else if (strncmp(&s[*pos], "atan(", 5) == 0) {
        (*pos) += 5;
        value = my_atan(parseExpression(s, pos));
        if (s[*pos] == ')') (*pos)++;
    } else if (strncmp(&s[*pos], "log(", 4) == 0) {
        (*pos) += 4;
        value = my_log(parseExpression(s, pos));
        if (s[*pos] == ')') (*pos)++;
    } else if (strncmp(&s[*pos], "ln(", 3) == 0) {
        (*pos) += 3;
        value = my_ln(parseExpression(s, pos));
        if (s[*pos] == ')') (*pos)++;
    } else if (strncmp(&s[*pos], "sqrt(", 5) == 0) {
        (*pos) += 5;
        value = my_sqrt(parseExpression(s, pos));
        if (s[*pos] == ')') (*pos)++;
    } else if (strncmp(&s[*pos], "e^(", 3) == 0) {
        (*pos) += 3;
        value = my_exp(parseExpression(s, pos));
        if (s[*pos] == ')') (*pos)++;
    } else { // Handle numbers
        char buffer[20];
        int i = 0;
        while ((s[*pos] >= '0' && s[*pos] <= '9') || s[*pos] == '.') {
            buffer[i++] = s[(*pos)++];
        }
        buffer[i] = '\0';
        value = atof(buffer);
    }
    return value;
}

float evaluateExpression(const char *expr) {
    int pos = 0;
    return parseExpression(expr, &pos);
}

int main(void) {
    setup();
    while (1) {
        loop();
    }
    return 0;
}
