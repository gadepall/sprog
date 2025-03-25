#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// ------------------------
// Global Definitions
#define EXPR_SIZE 64
#define DEBOUNCE_TIME 30  // milliseconds

// ------------------------
// LCD Interface (HD44780, 4-bit mode)
// Arduino Digital 8-13 are used via PORTB bits 0-5:
// RS = PB0, E = PB1, D4 = PB2, D5 = PB3, D6 = PB4, D7 = PB5.
#define LCD_RS PB0
#define LCD_E  PB1
#define LCD_D4 PB2
#define LCD_D5 PB3
#define LCD_D6 PB4
#define LCD_D7 PB5

#define LCD_DELAY _delay_us(50)

static void lcd_pulseEnable(void) {
    PORTB |= (1 << LCD_E);
    LCD_DELAY;
    PORTB &= ~(1 << LCD_E);
    LCD_DELAY;
}

static void lcd_sendNibble(uint8_t nibble) {
    PORTB &= ~((1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7));
    if(nibble & 0x01) PORTB |= (1 << LCD_D4);
    if(nibble & 0x02) PORTB |= (1 << LCD_D5);
    if(nibble & 0x04) PORTB |= (1 << LCD_D6);
    if(nibble & 0x08) PORTB |= (1 << LCD_D7);
    lcd_pulseEnable();
}

static void lcd_send(uint8_t value, uint8_t mode) {
    if(mode)
        PORTB |= (1 << LCD_RS);
    else
        PORTB &= ~(1 << LCD_RS);
    lcd_sendNibble(value >> 4);
    lcd_sendNibble(value & 0x0F);
    _delay_ms(2);
}

static void lcd_command(uint8_t cmd) {
    lcd_send(cmd, 0);
}

static void lcd_data(uint8_t data) {
    lcd_send(data, 1);
}

static void lcd_clear(void) {
    lcd_command(0x01);
    _delay_ms(2);
}

static void lcd_setCursor(uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = {0x00, 0x40};
    lcd_command(0x80 | (col + row_offsets[row]));
}

static void lcd_print(const char *str) {
    while(*str)
        lcd_data(*str++);
}

static void lcd_init(void) {
    // Set PORTB bits 0-5 as outputs.
    DDRB |= (1 << LCD_RS) | (1 << LCD_E) | (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);
    _delay_ms(50);
    lcd_sendNibble(0x03);
    _delay_ms(5);
    lcd_sendNibble(0x03);
    _delay_us(150);
    lcd_sendNibble(0x03);
    lcd_sendNibble(0x02); // Set to 4-bit mode.
    lcd_command(0x28);    // 4-bit, 2-line.
    lcd_command(0x0C);    // Display on, no cursor.
    lcd_command(0x06);    // Entry mode.
    lcd_clear();
    lcd_print("Simple Calc");
    _delay_ms(2000);
    lcd_clear();
    lcd_setCursor(0,0);
    lcd_print("Enter:");
}

// ------------------------
// Digital I/O functions
// Mapping: Digital pins 0-7 → Port D; 8-13 → Port B; A0-A5 → pins 14-19 (Port C)
#define INPUT  0
#define OUTPUT 1

static void pinMode(uint8_t pin, uint8_t mode) {
    if(pin <= 7) {
        if(mode == OUTPUT) DDRD |= (1 << pin);
        else DDRD &= ~(1 << pin);
    } else if(pin <= 13) {
        uint8_t pb = pin - 8;
        if(mode == OUTPUT) DDRB |= (1 << pb);
        else DDRB &= ~(1 << pb);
    } else {
        uint8_t pc = pin - 14;
        if(mode == OUTPUT) DDRC |= (1 << pc);
        else DDRC &= ~(1 << pc);
    }
}

static uint8_t digitalRead(uint8_t pin) {
    if(pin <= 7)
        return (PIND & (1 << pin)) ? 1 : 0;
    else if(pin <= 13) {
        uint8_t pb = pin - 8;
        return (PINB & (1 << pb)) ? 1 : 0;
    } else {
        uint8_t pc = pin - 14;
        return (PINC & (1 << pc)) ? 1 : 0;
    }
}

static void digitalWrite(uint8_t pin, uint8_t val) {
    if(pin <= 7) {
        if(val) PORTD |= (1 << pin); else PORTD &= ~(1 << pin);
    } else if(pin <= 13) {
        uint8_t pb = pin - 8;
        if(val) PORTB |= (1 << pb); else PORTB &= ~(1 << pb);
    } else {
        uint8_t pc = pin - 14;
        if(val) PORTC |= (1 << pc); else PORTC &= ~(1 << pc);
    }
}

// ------------------------
// Helper Functions for C String Handling

// Remove leading and trailing whitespace from a null-terminated string.
void trim(char *str) {
    char *start = str;
    while(*start && isspace((unsigned char)*start))
        start++;
    if(start != str)
        memmove(str, start, strlen(start)+1);
    size_t len = strlen(str);
    while(len > 0 && isspace((unsigned char)str[len-1])) {
        str[len-1] = '\0';
       len--;
    }
}

// Check if 'str' starts with 'prefix'.
bool startsWith(const char *str, const char *prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

// Check if 'str' ends with character 'c'.
bool endsWith(const char *str, char c) {
    size_t len = strlen(str);
    return (len > 0 && str[len-1] == c);
}

// Remove outer matching parentheses if they enclose the entire expression.
void removeOuterParentheses(char *expr) {
    size_t len = strlen(expr);
    while(len > 1 && expr[0]=='(' && expr[len-1]==')') {
        int count = 0;
        bool proper = true;
        for (size_t i = 0; i < len; i++) {
            if(expr[i]=='(') count++;
            else if(expr[i]==')') count--;
            if(count==0 && i < len-1) { proper = false; break; }
        }
        if(proper) {
            memmove(expr, expr+1, len-2);
            expr[len-2] = '\0';
            trim(expr);
            len = strlen(expr);
        } else break;
    }
}

// ------------------------
// Expression Evaluator and Parser (Pure C Version)
// Using a recursive descent parser.
double parseExpression(const char *expr); // Forward declaration.

double evaluateExpression(char *expr) {
    trim(expr);
    removeOuterParentheses(expr);
    // Handle function calls.
    if(startsWith(expr, "sin(") && endsWith(expr, ')')) {
        char inside[EXPR_SIZE];
        size_t len = strlen(expr);
        size_t n = (len > 5) ? len - 5 : 0;
        if(n >= EXPR_SIZE) n = EXPR_SIZE - 1;
        strncpy(inside, expr+4, n);
        inside[n] = '\0';
        double arg = evaluateExpression(inside);
        double x = arg * M_PI / 180.0;
        double dt = 0.0001, y = 0.0, dy = 1.0;
        for(double t = 0; t < x; t += dt){
            dy += -y * dt;
            y += dy * dt;
        }
        return y;
    }
    else if(startsWith(expr, "cos(") && endsWith(expr, ')')) {
        char inside[EXPR_SIZE];
        size_t len = strlen(expr);
        size_t n = (len > 5) ? len - 5 : 0;
        if(n >= EXPR_SIZE) n = EXPR_SIZE - 1;
        strncpy(inside, expr+4, n);
        inside[n] = '\0';
        double arg = evaluateExpression(inside);
        double x = arg * M_PI / 180.0;
        double dt = 0.0001, y = 1.0, dy = 0.0;
        for(double t = 0; t < x; t += dt){
            dy += -y * dt;
            y += dy * dt;
        }
        return y;
    }
    else if(startsWith(expr, "tan(") && endsWith(expr, ')')) {
        char inside[EXPR_SIZE];
        size_t len = strlen(expr);
        size_t n = (len > 5) ? len - 5 : 0;
        if(n >= EXPR_SIZE) n = EXPR_SIZE - 1;
        strncpy(inside, expr+4, n);
        inside[n] = '\0';
        double arg = evaluateExpression(inside);
        double x = arg * M_PI / 180.0;
        double dt = 0.0001;
        double ySin = 0.0, dySin = 1.0;
        for(double t = 0; t < x; t += dt){
            dySin += -ySin * dt;
            ySin += dySin * dt;
        }
        double yCos = 1.0, dyCos = 0.0;
        for(double t = 0; t < x; t += dt){
            dyCos += -yCos * dt;
            yCos += dyCos * dt;
        }
        if(yCos == 0) return NAN;
        return ySin / yCos;
    }
    else if(startsWith(expr, "exp(") && endsWith(expr, ')')) {
        char inside[EXPR_SIZE];
        size_t len = strlen(expr);
        size_t n = (len > 5) ? len - 5 : 0;
        if(n >= EXPR_SIZE) n = EXPR_SIZE - 1;
        strncpy(inside, expr+4, n);
        inside[n] = '\0';
        double arg = evaluateExpression(inside);
        double dt = 0.0001, y = 1.0;
        if(arg >= 0) {
            for(double t = 0; t < arg; t += dt)
                y += y * dt;
        } else {
            for(double t = 0; t > arg; t -= dt)
                y -= y * dt;
        }
        return y;
    }
    else if(startsWith(expr, "ln(") && endsWith(expr, ')')) {
        char inside[EXPR_SIZE];
        size_t len = strlen(expr);
        size_t n = (len > 4) ? len - 4 : 0;
        if(n >= EXPR_SIZE) n = EXPR_SIZE - 1;
        strncpy(inside, expr+3, n);
        inside[n] = '\0';
        double arg = evaluateExpression(inside);
        if(arg <= 0) return NAN;
        double dt = 0.0001, sum = 0.0;
        if(arg >= 1) {
            for(double t = 1; t < arg; t += dt)
                sum += (1.0/t) * dt;
        } else {
            for(double t = arg; t < 1; t += dt)
                sum -= (1.0/t) * dt;
        }
        return sum;
    }
    else if(startsWith(expr, "sqrt(") && endsWith(expr, ')')) {
        char inside[EXPR_SIZE];
        size_t len = strlen(expr);
        size_t n = (len > 6) ? len - 6 : 0;
        if(n >= EXPR_SIZE) n = EXPR_SIZE - 1;
        strncpy(inside, expr+5, n);
        inside[n] = '\0';
        double arg = evaluateExpression(inside);
        if(arg < 0) return NAN;
        double guess = arg / 2.0;
        for(int i = 0; i < 100; i++)
            guess = 0.5 * (guess + arg/guess);
        return guess;
    }
    else if(startsWith(expr, "log(") && endsWith(expr, ')')) {
        char inside[EXPR_SIZE];
        size_t len = strlen(expr);
        size_t n = (len > 5) ? len - 5 : 0;
        if(n >= EXPR_SIZE) n = EXPR_SIZE - 1;
        strncpy(inside, expr+4, n);
        inside[n] = '\0';
        double arg = evaluateExpression(inside);
        if(arg <= 0) return NAN;
        double dt = 0.0001, lnVal = 0.0;
        if(arg >= 1) {
            for(double t = 1; t < arg; t += dt)
                lnVal += (1.0/t) * dt;
        } else {
            for(double t = arg; t < 1; t += dt)
                lnVal -= (1.0/t) * dt;
        }
        double ln10 = 0.0;
        for(double t = 1; t < 10; t += dt)
            ln10 += (1.0/t) * dt;
        return lnVal / ln10;
    }
    else if(startsWith(expr, "abs(") && endsWith(expr, ')')) {
        char inside[EXPR_SIZE];
        size_t len = strlen(expr);
        size_t n = (len > 5) ? len - 5 : 0;
        if(n >= EXPR_SIZE) n = EXPR_SIZE - 1;
        strncpy(inside, expr+4, n);
        inside[n] = '\0';
        double arg = evaluateExpression(inside);
        return (arg < 0) ? -arg : arg;
    }
    // --- New inverse trigonometric functions ---
    else if(startsWith(expr, "asin(") && endsWith(expr, ')')) {
        char inside[EXPR_SIZE];
        size_t len = strlen(expr);
        size_t n = (len > 6) ? len - 6 : 0;  // "asin(" is 5 characters + ')'
        if(n >= EXPR_SIZE) n = EXPR_SIZE - 1;
        strncpy(inside, expr+5, n);
        inside[n] = '\0';
        double arg = evaluateExpression(inside);
        double result = asin(arg);      // in radians
        result = result * 180.0 / M_PI;   // convert to degrees
        return result;
    }
    else if(startsWith(expr, "acos(") && endsWith(expr, ')')) {
        char inside[EXPR_SIZE];
        size_t len = strlen(expr);
        size_t n = (len > 6) ? len - 6 : 0;
        if(n >= EXPR_SIZE) n = EXPR_SIZE - 1;
        strncpy(inside, expr+5, n);
        inside[n] = '\0';
        double arg = evaluateExpression(inside);
        double result = acos(arg);
        result = result * 180.0 / M_PI;
        return result;
    }
    else if(startsWith(expr, "atan(") && endsWith(expr, ')')) {
        char inside[EXPR_SIZE];
        size_t len = strlen(expr);
        size_t n = (len > 6) ? len - 6 : 0;
        if(n >= EXPR_SIZE) n = EXPR_SIZE - 1;
        strncpy(inside, expr+5, n);
        inside[n] = '\0';
        double arg = evaluateExpression(inside);
        double result = atan(arg);
        result = result * 180.0 / M_PI;
        return result;
    }
    // Otherwise, use the recursive descent parser.
    return parseExpression(expr);
}

double parseExpression(const char *expr) {
    char buf[EXPR_SIZE];
    strncpy(buf, expr, EXPR_SIZE);
    buf[EXPR_SIZE - 1] = '\0';
    int len = strlen(buf);
    // Trim leading spaces.
    while(len > 0 && isspace(buf[0])) { memmove(buf, buf+1, len); len = strlen(buf); }
    // Trim trailing spaces.
    while(len > 0 && isspace(buf[len-1])) { buf[len-1] = '\0'; len--; }
    if(len == 0) return 0;
    
    int pos = -1, paren = 0;
    // Look for '+' or '-' outside parentheses (scanning right-to-left).
    for (int i = len - 1; i >= 0; i--) {
        char c = buf[i];
        if(c == ')') paren++;
        else if(c == '(') paren--;
        if(paren == 0 && (c=='+' || c=='-')) { pos = i; break; }
    }
    if(pos != -1) {
        char left[EXPR_SIZE], right[EXPR_SIZE];
        strncpy(left, buf, pos);
        left[pos] = '\0';
        strncpy(right, buf + pos + 1, EXPR_SIZE - pos - 1);
        right[EXPR_SIZE - pos - 1] = '\0';
        double l = evaluateExpression(left);
        double r = evaluateExpression(right);
        return (buf[pos]=='+' ? l + r : l - r);
    }
    
    // Look for '*' or '/'.
    paren = 0; pos = -1;
    for (int i = len - 1; i >= 0; i--) {
        char c = buf[i];
        if(c == ')') paren++;
        else if(c == '(') paren--;
        if(paren == 0 && (c=='*' || c=='/')) { pos = i; break; }
    }
    if(pos != -1) {
        char left[EXPR_SIZE], right[EXPR_SIZE];
        strncpy(left, buf, pos);
        left[pos] = '\0';
        strncpy(right, buf + pos + 1, EXPR_SIZE - pos - 1);
        right[EXPR_SIZE - pos - 1] = '\0';
        double l = evaluateExpression(left);
        double r = evaluateExpression(right);
        if(buf[pos]=='*') return l * r;
        else return (r != 0 ? l / r : NAN);
    }
    
    // Look for '^' (right-associative).
    paren = 0; pos = -1;
    for (int i = len - 1; i >= 0; i--) {
        char c = buf[i];
        if(c == ')') paren++;
        else if(c == '(') paren--;
        if(paren == 0 && c=='^') { pos = i; break; }
    }
    if(pos != -1) {
        char left[EXPR_SIZE], right[EXPR_SIZE];
        strncpy(left, buf, pos);
        left[pos] = '\0';
        strncpy(right, buf + pos + 1, EXPR_SIZE - pos - 1);
        right[EXPR_SIZE - pos - 1] = '\0';
        double l = evaluateExpression(left);
        double r = evaluateExpression(right);
        return pow(l, r);
    }
    
    // No operator found; convert to number.
    char *endptr;
    double val = strtod(buf, &endptr);
    return val;
}

// ------------------------
// Button Definitions and Global State

typedef struct {
    int pin;
    char value; // 'C', '=' or digit '0'-'9'
} Button;

// Map A0-A5 to pins 14-19.
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19

Button buttons[] = {
    {2, '0'}, {3, '1'}, {4, '2'}, {5, '3'},
    {6, '4'}, {7, '5'}, {A0, '6'}, {A1, '7'},
    {A2, '8'}, {A3, '9'}, {A4, 'C'}, {A5, '='}
};
const int numButtons = sizeof(buttons) / sizeof(buttons[0]);

// Shift buttons on digital pins 0 and 1.
const int shiftAPin = 0;  // Arithmetic Shift.
const int shiftSPin = 1;  // Scientific Shift.

// Updated scientific mapping arrays with a new third page for inverse functions.
const char *sciMappingPage0[4] = {"sin(", "cos(", "tan(", "exp("};
const char *sciMappingPage1[4] = {"ln(", "sqrt(", "log(", "abs("};
const char *sciMappingPage2[4] = {"asin(", "acos(", "atan(", ""};

const char arithMappingPage0[4] = {"+", "-", "", "/"};
const char *arithMappingPage1[4] = {"^", "%", "(", ")"};
const char *arithMappingPage2[1] = {"BS"};  // Backspace

// Global calculator state.
char expression[EXPR_SIZE] = "";
int mode = 0;           // 0 = normal, 1 = arithmetic shift, 2 = scientific shift.
bool resultDisplayed = false;
int arithShiftPage = 0; // 0, 1, or 2.
int sciShiftPage = 0;   // 0, 1, or 2.

// ------------------------
// Main Function
int main(void) {
    // Initialize LCD.
    lcd_init();
    lcd_clear();
    lcd_setCursor(0,0);
    lcd_print("Enter:");
    
    // Configure numeric/function buttons as input with pull-ups.
    for (int i = 0; i < numButtons; i++) {
        pinMode(buttons[i].pin, INPUT);
        digitalWrite(buttons[i].pin, 1);
    }
    // Configure shift buttons.
    pinMode(shiftSPin, INPUT);
    digitalWrite(shiftSPin, 1);
    pinMode(shiftAPin, INPUT);
    digitalWrite(shiftAPin, 1);
    
    while(1) {
        // Check numeric/function buttons.
        for (int i = 0; i < numButtons; i++) {
            if(digitalRead(buttons[i].pin) == 0) { // Active low.
                _delay_ms(DEBOUNCE_TIME);
                if(digitalRead(buttons[i].pin) == 0) {
                    char ch = buttons[i].value;
                    while(digitalRead(buttons[i].pin) == 0)
                        _delay_ms(10);
                    
                    if(ch == 'C') {
                        expression[0] = '\0';
                        mode = 0;
                        resultDisplayed = false;
                        lcd_clear();
                        lcd_setCursor(0,0);
                        lcd_print("Enter:");
                    }
                    else if(ch == '=') {
                        lcd_clear();
                        double result = evaluateExpression(expression);
                        char resStr[10];
                        dtostrf(result, 6, 4, resStr);
                        lcd_setCursor(0,0);
                        lcd_print(resStr);
                        strcpy(expression, resStr);
                        resultDisplayed = true;
                        mode = 0;
                    }
                    else {
                        if(resultDisplayed && mode == 0) {
                            expression[0] = '\0';
                            resultDisplayed = false;
                        }
                        int digitVal = ch - '0';
                        if(mode == 1) { // Arithmetic shift mode.
                            if(digitVal == 8) {
                                arithShiftPage = (arithShiftPage + 1) % 3;
                                lcd_clear();
                                if(arithShiftPage == 0)
                                    lcd_print("0:+ 1:- 2:* 3:/");
                                else if(arithShiftPage == 1)
                                    lcd_print("0:^ 1:% 2:( 3:)");
                                else if(arithShiftPage == 2)
                                    lcd_print("0:BS");
                                lcd_setCursor(0,1);
                                lcd_print("8:Next 9:Prev");
                                _delay_ms(200);
                                continue;
                            } else if(digitVal == 9) {
                                arithShiftPage = (arithShiftPage - 1 + 3) % 3;
                                lcd_clear();
                                if(arithShiftPage == 0)
                                    lcd_print("0:+ 1:- 2:* 3:/");
                                else if(arithShiftPage == 1)
                                    lcd_print("0:^ 1:% 2:( 3:)");
                                else if(arithShiftPage == 2)
                                    lcd_print("0:BS");
                                lcd_setCursor(0,1);
                                lcd_print("8:Next 9:Prev");
                                _delay_ms(200);
                                continue;
                            } else if(arithShiftPage == 0 && digitVal < 4) {
                                strcat(expression, arithMappingPage0[digitVal]);
                                mode = 0;
                                lcd_clear();
                                lcd_setCursor(0,0);
                                lcd_print(expression);
                            } else if(arithShiftPage == 1 && digitVal < 4) {
                                strcat(expression, arithMappingPage1[digitVal]);
                                mode = 0;
                                lcd_clear();
                                lcd_setCursor(0,0);
                                lcd_print(expression);
                            } else if(arithShiftPage == 2 && digitVal == 0) {
                                int l = strlen(expression);
                                if(l > 0)
                                    expression[l-1] = '\0';
                                mode = 0;
                                lcd_clear();
                                lcd_setCursor(0,0);
                                lcd_print(expression);
                                continue;
                            } else {
                                lcd_clear();
                                lcd_setCursor(0,0);
                                lcd_print("Invalid Op");
                                _delay_ms(1000);
                                continue;
                            }
                        }
                        else if(mode == 2) { // Scientific shift mode.
                            if(digitVal == 8) {
                                sciShiftPage = (sciShiftPage + 1) % 3;
                                lcd_clear();
                                if(sciShiftPage == 0) {
                                    lcd_print("0:sin 1:cos");
                                    lcd_setCursor(0,1);
                                    lcd_print("2:tan 3:exp");
                                } else if(sciShiftPage == 1) {
                                    lcd_print("0:ln 1:sqrt");
                                    lcd_setCursor(0,1);
                                    lcd_print("2:log 3:abs");
                                } else if(sciShiftPage == 2) {
                                    lcd_print("0:asin 1:acos");
                                    lcd_setCursor(0,1);
                                    lcd_print("2:atan 3:    ");
                                }
                                _delay_ms(200);
                                continue;
                            } else if(digitVal == 9) {
                                sciShiftPage = (sciShiftPage - 1 + 3) % 3;
                                lcd_clear();
                                if(sciShiftPage == 0) {
                                    lcd_print("0:sin 1:cos");
                                    lcd_setCursor(0,1);
                                    lcd_print("2:tan 3:exp");
                                } else if(sciShiftPage == 1) {
                                    lcd_print("0:ln 1:sqrt");
                                    lcd_setCursor(0,1);
                                    lcd_print("2:log 3:abs");
                                } else if(sciShiftPage == 2) {
                                    lcd_print("0:asin 1:acos");
                                    lcd_setCursor(0,1);
                                    lcd_print("2:atan 3:    ");
                                }
                                _delay_ms(200);
                                continue;
                            } else if(digitVal < 4) {
                                if(sciShiftPage == 0)
                                    strcat(expression, sciMappingPage0[digitVal]);
                                else if(sciShiftPage == 1)
                                    strcat(expression, sciMappingPage1[digitVal]);
                                else if(sciShiftPage == 2)
                                    strcat(expression, sciMappingPage2[digitVal]);
                                mode = 0;
                                lcd_clear();
                                lcd_setCursor(0,0);
                                lcd_print(expression);
                            } else {
                                lcd_clear();
                                lcd_setCursor(0,0);
                                lcd_print("Invalid Op");
                                _delay_ms(1000);
                                continue;
                            }
                        }
                        else { // Normal mode.
                            char buf[2] = { ch, '\0' };
                            strcat(expression, buf);
                            lcd_clear();
                            lcd_setCursor(0,0);
                            lcd_print(expression);
                        }
                    }
                    _delay_ms(200);
                }
            }
        }
        
        // Check shift buttons.
        if(digitalRead(shiftAPin) == 0) {
            _delay_ms(DEBOUNCE_TIME);
            if(digitalRead(shiftAPin) == 0) {
                if(mode == 1) {
                    mode = 0;
                    lcd_clear();
                    lcd_setCursor(0,0);
                    lcd_print(expression);
                } else {
                    mode = 1;
                    arithShiftPage = 0;
                    lcd_clear();
                    lcd_print("0:+ 1:- 2:* 3:/");
                    lcd_setCursor(0,1);
                    lcd_print("8:Next 9:Prev");
                }
                while(digitalRead(shiftAPin)==0)
                    _delay_ms(10);
                _delay_ms(200);
            }
        }
        if(digitalRead(shiftSPin) == 0) {
            _delay_ms(DEBOUNCE_TIME);
            if(digitalRead(shiftSPin) == 0) {
                if(mode == 2) {
                    mode = 0;
                    lcd_clear();
                    lcd_setCursor(0,0);
                    lcd_print(expression);
                } else {
                    mode = 2;
                    sciShiftPage = 0;
                    lcd_clear();
                    lcd_print("0:sin 1:cos");
                    lcd_setCursor(0,1);
                    lcd_print("2:tan 3:exp");
                }
                while(digitalRead(shiftSPin)==0)
                    _delay_ms(10);
                _delay_ms(200);
            }
        }
    }
    
    return 0;
} 
