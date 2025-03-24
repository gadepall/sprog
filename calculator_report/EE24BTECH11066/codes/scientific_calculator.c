#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
// Removed math.h

// LCD Pin Definitions
#define LCD_RS_PORT PORTD
#define LCD_RS_DDR DDRD
#define LCD_RS_PIN PD1  // Arduino pin 1

#define LCD_E_PORT PORTD
#define LCD_E_DDR DDRD
#define LCD_E_PIN PD0  // Arduino pin 0

#define LCD_D4_PORT PORTD
#define LCD_D4_DDR DDRD
#define LCD_D4_PIN PD5  // Arduino pin 5

#define LCD_D5_PORT PORTD
#define LCD_D5_DDR DDRD
#define LCD_D5_PIN PD4  // Arduino pin 4

#define LCD_D6_PORT PORTD
#define LCD_D6_DDR DDRD
#define LCD_D6_PIN PD3  // Arduino pin 3

#define LCD_D7_PORT PORTD
#define LCD_D7_DDR DDRD
#define LCD_D7_PIN PD2  // Arduino pin 2

// Button Pin Definitions
#define BTN0_PORT PORTB
#define BTN0_DDR DDRB
#define BTN0_PIN PB5  // Arduino pin 13
#define BTN0_PIN_REG PINB

#define BTN1_PORT PORTB
#define BTN1_DDR DDRB
#define BTN1_PIN PB4  // Arduino pin 12
#define BTN1_PIN_REG PINB

#define BTN2_PORT PORTB
#define BTN2_DDR DDRB
#define BTN2_PIN PB3  // Arduino pin 11
#define BTN2_PIN_REG PINB

#define BTN3_PORT PORTB
#define BTN3_DDR DDRB
#define BTN3_PIN PB2  // Arduino pin 10
#define BTN3_PIN_REG PINB

#define BTN4_PORT PORTB
#define BTN4_DDR DDRB
#define BTN4_PIN PB1  // Arduino pin 9
#define BTN4_PIN_REG PINB

#define BTN5_PORT PORTB
#define BTN5_DDR DDRB
#define BTN5_PIN PB0  // Arduino pin 8
#define BTN5_PIN_REG PINB

#define BTN6_PORT PORTD
#define BTN6_DDR DDRD
#define BTN6_PIN PD7  // Arduino pin 7
#define BTN6_PIN_REG PIND

#define BTN7_PORT PORTD
#define BTN7_DDR DDRD
#define BTN7_PIN PD6  // Arduino pin 6
#define BTN7_PIN_REG PIND

#define BTN8_PORT PORTC
#define BTN8_DDR DDRC
#define BTN8_PIN PC0  // Arduino pin A0
#define BTN8_PIN_REG PINC

#define BTN9_PORT PORTC
#define BTN9_DDR DDRC
#define BTN9_PIN PC1  // Arduino pin A1
#define BTN9_PIN_REG PINC

#define OP_BTN_PORT PORTC
#define OP_BTN_DDR DDRC
#define OP_BTN_PIN PC2  // Arduino pin A2
#define OP_BTN_PIN_REG PINC

#define TRIG_BTN_PORT PORTC
#define TRIG_BTN_DDR DDRC
#define TRIG_BTN_PIN PC3  // Arduino pin A3
#define TRIG_BTN_PIN_REG PINC

#define DECIMAL_BTN_PORT PORTC
#define DECIMAL_BTN_DDR DDRC
#define DECIMAL_BTN_PIN PC4  // Arduino pin A4
#define DECIMAL_BTN_PIN_REG PINC

#define ENTER_BTN_PORT PORTC
#define ENTER_BTN_DDR DDRC
#define ENTER_BTN_PIN PC5  // Arduino pin A5
#define ENTER_BTN_PIN_REG PINC

// LCD Commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// LCD Display Control Flags
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// LCD Entry Mode Flags
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// LCD Function Set Flags
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// Function counts for cycling
#define OPERATOR_COUNT 4  // +, -, *, /
#define TRIG_COUNT 6      // sin, cos, tan, asin, acos, atan

// Math constants
#define M_PI 3.14159265358979323846
#define M_PI_180 0.01745329251994329577  // PI/180
#define M_180_PI 57.295779513082320877   // 180/PI
#define NAN __builtin_nan("")            // Using compiler's NAN

// Global Variables
char expression[32];
uint8_t exprIndex = 0;
uint8_t enteringNumber = 1;
int8_t operation = -1;
int8_t trigMode = -1;  // -1 means no trig function selected
uint8_t trigSelected = 0;
uint8_t operatorSelected = 0;
uint8_t resultDisplayed = 0; // Keeps track if a result is being displayed
float lastResult = 0.0;      // Store the last calculated result
int lastOperatorIndex = -1;  // Position of the last operator in expression
int lastTrigIndex = -1;      // Position of the last trig function in expression
int lastTrigLength = 0;      // Length of the last trig function string

// Math function prototypes
float custom_sin(float angle);
float custom_cos(float angle);
float custom_tan(float angle);
float custom_asin(float value);
float custom_acos(float value);
float custom_atan(float value);
float custom_pow(float base, int exponent);
float custom_factorial(int n);
int custom_isnan(float x);

// Function prototypes
void lcd_init(void);
void lcd_send_command(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_send_byte(uint8_t byte, uint8_t mode);
void lcd_send_nibble(uint8_t nibble);
void lcd_clear(void);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(const char *str);
void init_buttons(void);
void update_lcd(void);
void append_operator(void);
void append_trig_function(void);
void process_enter(void);
void balance_parentheses(void);
float evaluate_expression(const char *expr);
void reset_calculator(void);
uint8_t check_button(volatile uint8_t *port_reg, uint8_t pin);
void append_char(char c);
void append_string(const char *str);
int find_char(const char *str, char c);
int find_char_from(const char *str, char c, int start_pos);
int get_string_length(const char *str);
float string_to_float(const char *str);
void float_to_string(float num, int precision, char *buffer);
void substring(const char *source, char *dest, int start, int length);
int handle_bracket_expression(const char *expr, float *result);
int find_last_occurrence(const char *str, char c, int end_pos);
int find_matching_parenthesis(const char *str, int open_pos);

// Custom math implementations
float custom_sin(float angle) {
    // Convert angle from degrees to radians
    float rad = angle * M_PI_180;
    
    // Taylor series approximation of sin(x)
    float result = rad;
    float term = rad;
    int i;
    
    for (i = 1; i <= 7; i++) {  // Using 7 terms for reasonable accuracy
        term = -term * rad * rad / ((2 * i) * (2 * i + 1));
        result += term;
    }
    
    return result;
}

float custom_cos(float angle) {
    // Convert angle from degrees to radians
    float rad = angle * M_PI_180;
    
    // Taylor series approximation of cos(x)
    float result = 1.0;
    float term = 1.0;
    int i;
    
    for (i = 1; i <= 7; i++) {  // Using 7 terms for reasonable accuracy
        term = -term * rad * rad / ((2 * i - 1) * (2 * i));
        result += term;
    }
    
    return result;
}

float custom_tan(float angle) {
    float sin_val = custom_sin(angle);
    float cos_val = custom_cos(angle);
    
    if (cos_val == 0.0) {
        return NAN;  // Division by zero
    }
    
    return sin_val / cos_val;
}

float custom_pow(float base, int exponent) {
    float result = 1.0;
    int i;
    
    for (i = 0; i < exponent; i++) {
        result *= base;
    }
    
    return result;
}

float custom_factorial(int n) {
    float result = 1.0;
    int i;
    
    for (i = 2; i <= n; i++) {
        result *= i;
    }
    
    return result;
}

// Arcsin implementation using Taylor series
float custom_asin(float value) {
    // Check domain [-1, 1]
    if (value < -1.0 || value > 1.0) {
        return NAN;
    }
    
    // For values close to 1 or -1, use the identity asin(x) = π/2 - asin(√(1-x²))
    if (value > 0.7 || value < -0.7) {
        float sign = (value > 0) ? 1.0 : -1.0;
        return sign * (M_PI/2 - custom_asin(custom_pow(1.0 - value*value, 0.5)));
    }
    
    // Taylor series for arcsin
    float result = value;
    float term = value;
    float x_squared = value * value;
    
    for (int i = 1; i <= 5; i++) {
        float coef = (2 * i - 1.0) / (2 * i);
        term *= x_squared * coef;
        result += term / (2 * i + 1);
    }
    
    // Convert from radians to degrees
    return result * M_180_PI;
}

// Arccos implementation using asin
float custom_acos(float value) {
    // Check domain [-1, 1]
    if (value < -1.0 || value > 1.0) {
        return NAN;
    }
    
    // Use the identity: acos(x) = 90° - asin(x)
    return 90.0 - custom_asin(value);
}

// Arctan implementation using Taylor series
float custom_atan(float value) {
    // For large values, use the identity atan(x) = π/2 - atan(1/x)
    if (value > 1.0) {
        return 90.0 - custom_atan(1.0 / value);
    } else if (value < -1.0) {
        return -90.0 - custom_atan(1.0 / value);
    }
    
    // Taylor series for arctan
    float result = value;
    float term = value;
    float x_squared = value * value;
    
    for (int i = 1; i <= 7; i++) {
        term = -term * x_squared;
        result += term / (2 * i + 1);
    }
    
    // Convert from radians to degrees
    return result * M_180_PI;
}

int custom_isnan(float x) {
    // Check if a float is NaN
    return x != x;
}

// LCD Functions
void lcd_init(void) {
    // Set up the LCD pins as outputs
    LCD_RS_DDR |= (1 << LCD_RS_PIN);
    LCD_E_DDR |= (1 << LCD_E_PIN);
    LCD_D4_DDR |= (1 << LCD_D4_PIN);
    LCD_D5_DDR |= (1 << LCD_D5_PIN);
    LCD_D6_DDR |= (1 << LCD_D6_PIN);
    LCD_D7_DDR |= (1 << LCD_D7_PIN);
    
    // Wait for LCD to initialize
    _delay_ms(50);
    
    // Initialize in 4-bit mode
    LCD_RS_PORT &= ~(1 << LCD_RS_PIN);  // RS = 0 for command
    LCD_E_PORT &= ~(1 << LCD_E_PIN);    // E = 0
    
    // Initialization sequence
    lcd_send_nibble(0x03);
    _delay_ms(5);
    lcd_send_nibble(0x03);
    _delay_ms(5);
    lcd_send_nibble(0x03);
    _delay_ms(1);
    lcd_send_nibble(0x02);
    _delay_ms(1);
    
    // Function set: 4-bit mode, 2 lines, 5x8 dots
    lcd_send_command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);
    
    // Display control: Display on, cursor off, blink off
    lcd_send_command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
    
    // Entry mode set: Increment cursor, no display shift
    lcd_send_command(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
    
    // Clear display
    lcd_clear();
}

void lcd_send_command(uint8_t cmd) {
    lcd_send_byte(cmd, 0);
}

void lcd_send_data(uint8_t data) {
    lcd_send_byte(data, 1);
}

void lcd_send_byte(uint8_t byte, uint8_t mode) {
    // Set RS pin based on mode (1 = data, 0 = command)
    if (mode) {
        LCD_RS_PORT |= (1 << LCD_RS_PIN);
    } else {
        LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
    }
    
    // Send high nibble
    lcd_send_nibble(byte >> 4);
    
    // Send low nibble
    lcd_send_nibble(byte & 0x0F);
    
    // Delay for command execution
    _delay_us(50);
}

void lcd_send_nibble(uint8_t nibble) {
    // Set data pins
    if (nibble & 0x01) LCD_D4_PORT |= (1 << LCD_D4_PIN);
    else LCD_D4_PORT &= ~(1 << LCD_D4_PIN);
    
    if (nibble & 0x02) LCD_D5_PORT |= (1 << LCD_D5_PIN);
    else LCD_D5_PORT &= ~(1 << LCD_D5_PIN);
    
    if (nibble & 0x04) LCD_D6_PORT |= (1 << LCD_D6_PIN);
    else LCD_D6_PORT &= ~(1 << LCD_D6_PIN);
    
    if (nibble & 0x08) LCD_D7_PORT |= (1 << LCD_D7_PIN);
    else LCD_D7_PORT &= ~(1 << LCD_D7_PIN);
    
    // Toggle E pin to send data
    LCD_E_PORT |= (1 << LCD_E_PIN);
    _delay_us(1);
    LCD_E_PORT &= ~(1 << LCD_E_PIN);
    _delay_us(1);
}

void lcd_clear(void) {
    lcd_send_command(LCD_CLEARDISPLAY);
    _delay_ms(2); // This command takes a long time
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = {0x00, 0x40};
    lcd_send_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_send_data(*str++);
    }
}

void init_buttons(void) {
    // Set up number buttons as inputs with pull-ups
    BTN0_DDR &= ~(1 << BTN0_PIN);
    BTN1_DDR &= ~(1 << BTN1_PIN);
    BTN2_DDR &= ~(1 << BTN2_PIN);
    BTN3_DDR &= ~(1 << BTN3_PIN);
    BTN4_DDR &= ~(1 << BTN4_PIN);
    BTN5_DDR &= ~(1 << BTN5_PIN);
    BTN6_DDR &= ~(1 << BTN6_PIN);
    BTN7_DDR &= ~(1 << BTN7_PIN);
    BTN8_DDR &= ~(1 << BTN8_PIN);
    BTN9_DDR &= ~(1 << BTN9_PIN);
    
    BTN0_PORT |= (1 << BTN0_PIN);
    BTN1_PORT |= (1 << BTN1_PIN);
    BTN2_PORT |= (1 << BTN2_PIN);
    BTN3_PORT |= (1 << BTN3_PIN);
    BTN4_PORT |= (1 << BTN4_PIN);
    BTN5_PORT |= (1 << BTN5_PIN);
    BTN6_PORT |= (1 << BTN6_PIN);
    BTN7_PORT |= (1 << BTN7_PIN);
    BTN8_PORT |= (1 << BTN8_PIN);
    BTN9_PORT |= (1 << BTN9_PIN);
    
    // Set up function buttons as inputs with pull-ups
    OP_BTN_DDR &= ~(1 << OP_BTN_PIN);
    TRIG_BTN_DDR &= ~(1 << TRIG_BTN_PIN);
    ENTER_BTN_DDR &= ~(1 << ENTER_BTN_PIN);
    DECIMAL_BTN_DDR &= ~(1 << DECIMAL_BTN_PIN);
    OP_BTN_PORT |= (1 << OP_BTN_PIN);
    TRIG_BTN_PORT |= (1 << TRIG_BTN_PIN);
    DECIMAL_BTN_PORT |= (1 << DECIMAL_BTN_PIN);
    ENTER_BTN_PORT |= (1 << ENTER_BTN_PIN);
}

uint8_t check_button(volatile uint8_t *port_reg, uint8_t pin) {
    return !(*port_reg & (1 << pin));
}

void update_lcd(void) {
    lcd_clear();
    lcd_print(expression);
}

void append_operator(void) {
    const char operators[] = {'+', '-', '*', '/'};
    
    // If result is displayed and operator is pressed, continue calculation with result
    if (resultDisplayed) {
        char result_str[16];
        float_to_string(lastResult, 3, result_str);
        
        // Start a new expression with the result
        exprIndex = 0;
        append_string(result_str);
        resultDisplayed = 0;
    }
    
    // If an operator was already selected at the end of expression
    if (operatorSelected && lastOperatorIndex == exprIndex - 1) {
        // Remove the last operator
        exprIndex--;
        expression[exprIndex] = '\0';
    }
    
    // Append new operator
    append_char(operators[operation]);
    lastOperatorIndex = exprIndex - 1;
    
    update_lcd();
}

void append_trig_function(void) {
    const char *trigFunctions[] = {"sin(", "cos(", "tan(", "asin(", "acos(", "atan("};
    
    // If result is displayed and trig is pressed, continue calculation with result
    if (resultDisplayed) {
        char result_str[16];
        float_to_string(lastResult, 3, result_str);
        
        // Start a new expression with the result
        exprIndex = 0;
        append_string(result_str);
        resultDisplayed = 0;
        
        // If we're continuing from a result, we need an operator
        // Add a plus sign by default
        append_char('+');
        lastOperatorIndex = exprIndex - 1;
    }
    
    // If trig was already selected, remove the previous function
    if (trigSelected && lastTrigIndex >= 0) {
        // Remove previous trig function and all characters after it
        exprIndex = lastTrigIndex;
        expression[exprIndex] = '\0';
    }
    
    // Get length of new trig function
    lastTrigLength = strlen(trigFunctions[trigMode]);
    
    // Store position where trig function starts
    lastTrigIndex = exprIndex;
    
    // Append new trig function
    append_string(trigFunctions[trigMode]);
    
    update_lcd();
}

void process_enter(void) {
    if (exprIndex == 0) return;
    balance_parentheses();
    
    float result = evaluate_expression(expression);
    lastResult = result;  // Store the result for future calculations
    
    if (custom_isnan(result)) {
        exprIndex = 0;
        append_string("Undefined");
    } else {
        char result_str[16];
        float_to_string(result, 3, result_str);
        
        exprIndex = 0;
        append_string(result_str);
    }
    
    resultDisplayed = 1;
    trigSelected = 0;  // Reset trig selection state
    operatorSelected = 0;  // Reset operator selection state
    lastTrigIndex = -1;  // Reset trig index
    lastOperatorIndex = -1;  // Reset operator index
    
    lcd_clear();
    lcd_print("Result:");
    lcd_set_cursor(0, 1);
    lcd_print(expression);
}

void balance_parentheses(void) {
    int openCount = 0;
    for (int i = 0; i < exprIndex; i++) {
        if (expression[i] == '(') openCount++;
        else if (expression[i] == ')') openCount--;
    }
    while (openCount > 0 && exprIndex < sizeof(expression) - 1) {
        expression[exprIndex++] = ')';
        expression[exprIndex] = '\0';
        openCount--;
    }
}

void append_char(char c) {
    if (exprIndex < sizeof(expression) - 1) {
        expression[exprIndex++] = c;
        expression[exprIndex] = '\0';
    }
}

void append_string(const char *str) {
    while (*str && exprIndex < sizeof(expression) - 1) {
        expression[exprIndex++] = *str++;
    }
    expression[exprIndex] = '\0';
}

int find_char(const char *str, char c) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == c) return i;
        i++;
    }
    return -1;
}

int find_char_from(const char *str, char c, int start_pos) {
    int i = start_pos;
    while (str[i] != '\0') {
        if (str[i] == c) return i;
        i++;
    }
    return -1;
}

int find_last_occurrence(const char *str, char c, int end_pos) {
    for (int i = end_pos; i >= 0; i--) {
        if (str[i] == c) {
            return i;
        }
    }
    return -1;
}

int find_matching_parenthesis(const char *str, int open_pos) {
    int count = 1;
    for (int i = open_pos + 1; str[i] != '\0'; i++) {
        if (str[i] == '(') {
            count++;
        } else if (str[i] == ')') {
            count--;
            if (count == 0) {
                return i;
            }
        }
    }
    return -1; // No matching parenthesis found
}

int get_string_length(const char *str) {
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

void substring(const char *source, char *dest, int start, int length) {
    int i;
    for (i = 0; i < length && source[start + i] != '\0'; i++) {
        dest[i] = source[start + i];
    }
    dest[i] = '\0';
}

float string_to_float(const char *str) {
    return atof(str);
}

void float_to_string(float num, int precision, char *buffer) {
    dtostrf(num, 4, precision, buffer);
}

// Handle a bracket expression and return the position after the closing bracket
int handle_bracket_expression(const char *expr, float *result) {
    int close_pos = find_matching_parenthesis(expr, 0);
    if (close_pos <= 0) {
        *result = NAN;
        return -1;
    }
    
    // Extract content inside brackets
    char subExpr[32];
    substring(expr, subExpr, 1, close_pos - 1);
    
    // Evaluate the expression inside the brackets
    *result = evaluate_expression(subExpr);
    
    return close_pos + 1;
}

// BODMAS compliant expression evaluation
float evaluate_expression(const char *expr) {
    // Skip leading spaces
    int start = 0;
    while (expr[start] == ' ') start++;

    // Empty expression
    if (expr[start] == '\0') return 0.0;

    // Check for trigonometric functions
    if (strncmp(&expr[start], "sin(", 4) == 0) {
        float value;
        int pos = handle_bracket_expression(&expr[start + 3], &value);
        if (pos < 0) return NAN;
        
        float sinResult = custom_sin(value);
        
        // Check if there's more to evaluate
        if (expr[start + 3 + pos] != '\0') {
            char newExpr[32];
            char resultStr[16];
            float_to_string(sinResult, 6, resultStr);
            strcpy(newExpr, resultStr);
            strcat(newExpr, &expr[start + 3 + pos]);
            return evaluate_expression(newExpr);
        }
        return sinResult;
    }
    
    if (strncmp(&expr[start], "cos(", 4) == 0) {
        float value;
        int pos = handle_bracket_expression(&expr[start + 3], &value);
        if (pos < 0) return NAN;
        
        float cosResult = custom_cos(value);
        
        // Check if there's more to evaluate
        if (expr[start + 3 + pos] != '\0') {
            char newExpr[32];
            char resultStr[16];
            float_to_string(cosResult, 6, resultStr);
            strcpy(newExpr, resultStr);
            strcat(newExpr, &expr[start + 3 + pos]);
            return evaluate_expression(newExpr);
        }
        return cosResult;
    }
    
    if (strncmp(&expr[start], "tan(", 4) == 0) {
        float value;
        int pos = handle_bracket_expression(&expr[start + 3], &value);
        if (pos < 0) return NAN;
        
        float tanResult = custom_tan(value);
        
        // Check if there's more to evaluate
        if (expr[start + 3 + pos] != '\0') {
            char newExpr[32];
            char resultStr[16];
            float_to_string(tanResult, 6, resultStr);
            strcpy(newExpr, resultStr);
            strcat(newExpr, &expr[start + 3 + pos]);
            return evaluate_expression(newExpr);
        }
        return tanResult;
    }
    
    if (strncmp(&expr[start], "asin(", 5) == 0) {
        float value;
        int pos = handle_bracket_expression(&expr[start + 4], &value);
        if (pos < 0) return NAN;
        
        float asinResult = custom_asin(value);
        
        // Check if there's more to evaluate
        if (expr[start + 4 + pos] != '\0') {
            char newExpr[32];
            char resultStr[16];
            float_to_string(asinResult, 6, resultStr);
            strcpy(newExpr, resultStr);
            strcat(newExpr, &expr[start + 4 + pos]);
            return evaluate_expression(newExpr);
        }
        return asinResult;
    }
    
    if (strncmp(&expr[start], "acos(", 5) == 0) {
        float value;
        int pos = handle_bracket_expression(&expr[start + 4], &value);
        if (pos < 0) return NAN;
        
        float acosResult = custom_acos(value);
        
        // Check if there's more to evaluate
        if (expr[start + 4 + pos] != '\0') {
            char newExpr[32];
            char resultStr[16];
            float_to_string(acosResult, 6, resultStr);
            strcpy(newExpr, resultStr);
            strcat(newExpr, &expr[start + 4 + pos]);
            return evaluate_expression(newExpr);
        }
        return acosResult;
    }
    
    if (strncmp(&expr[start], "atan(", 5) == 0) {
        float value;
        int pos = handle_bracket_expression(&expr[start + 4], &value);
        if (pos < 0) return NAN;
        
        float atanResult = custom_atan(value);
        
        // Check if there's more to evaluate
        if (expr[start + 4 + pos] != '\0') {
            char newExpr[32];
            char resultStr[16];
            float_to_string(atanResult, 6, resultStr);
            strcpy(newExpr, resultStr);
            strcat(newExpr, &expr[start + 4 + pos]);
            return evaluate_expression(newExpr);
        }
        return atanResult;
    }

    // Look for addition and subtraction
    float left = 0.0;
    int pos = 0;
    int foundAddSub = 0;
    
    // First, try to find the rightmost + or - that is not inside parentheses
    int paren_level = 0;
    int last_add_pos = -1;
    int last_sub_pos = -1;
    
    for (int i = start; expr[i] != '\0'; i++) {
        if (expr[i] == '(') paren_level++;
        else if (expr[i] == ')') paren_level--;
        else if (paren_level == 0) {
            if (expr[i] == '+') last_add_pos = i;
            else if (expr[i] == '-' && i > start && (expr[i-1] < '0' || expr[i-1] > '9') && expr[i-1] != ')') last_sub_pos = i;
        }
    }
    
    int op_pos = (last_add_pos > last_sub_pos) ? last_add_pos : last_sub_pos;
    
    if (op_pos >= 0) {
        // We found an addition or subtraction operator
        char left_expr[32];
        substring(expr, left_expr, start, op_pos - start);
        left = evaluate_expression(left_expr);
        
        char right_expr[32];
        strcpy(right_expr, &expr[op_pos + 1]);
        float right = evaluate_expression(right_expr);
        
        if (expr[op_pos] == '+') return left + right;
        else return left - right;
    }
    
    // Look for multiplication and division
    paren_level = 0;
    int last_mul_pos = -1;
    int last_div_pos = -1;
    
    for (int i = start; expr[i] != '\0'; i++) {
        if (expr[i] == '(') paren_level++;
        else if (expr[i] == ')') paren_level--;
        else if (paren_level == 0) {
            if (expr[i] == '*') last_mul_pos = i;
            else if (expr[i] == '/') last_div_pos = i;
        }
    }
    
    op_pos = (last_mul_pos > last_div_pos) ? last_mul_pos : last_div_pos;
    
    if (op_pos >= 0) {
        // We found a multiplication or division operator
        char left_expr[32];
        substring(expr, left_expr, start, op_pos - start);
        left = evaluate_expression(left_expr);
        
        char right_expr[32];
        strcpy(right_expr, &expr[op_pos + 1]);
        float right = evaluate_expression(right_expr);
        
        if (expr[op_pos] == '*') return left * right;
        else {
            if (right == 0.0) return NAN;  // Division by zero
            return left / right;
        }
    }
    
    // Check for parentheses (bracket expression)
    if (expr[start] == '(') {
        float result;
        int end_pos = handle_bracket_expression(&expr[start], &result);
        if (end_pos < 0) return NAN;
        
        // Check if there's more to evaluate after the parentheses
        if (expr[start + end_pos] != '\0') {
            char newExpr[32];
            char resultStr[16];
            float_to_string(result, 6, resultStr);
            strcpy(newExpr, resultStr);
            strcat(newExpr, &expr[start + end_pos]);
            return evaluate_expression(newExpr);
        }
        return result;
    }
    
    // If we reach here, it's a number
    return string_to_float(&expr[start]);
}

void reset_calculator(void) {
    exprIndex = 0;
    expression[0] = '\0';
    enteringNumber = 1;
    operation = -1;
    trigMode = -1;
    trigSelected = 0;
    operatorSelected = 0;
    resultDisplayed = 0;
    lastResult = 0.0;
    lastOperatorIndex = -1;
    lastTrigIndex = -1;
    lastTrigLength = 0;
    
    lcd_clear();
    lcd_print("Ready");
    _delay_ms(500);
    lcd_clear();
}

int main(void) {
    // Initialize LCD
    lcd_init();
    
    // Initialize button inputs
    init_buttons();
    
    // Display welcome message
    lcd_print("Calculator");
    lcd_set_cursor(0, 1);
    lcd_print("Ready");
    _delay_ms(1000);
    lcd_clear();
    
    // Reset calculator state
    reset_calculator();
    
    // Main loop
    while (1) {
        // Check number buttons
        if (check_button(&BTN0_PIN_REG, BTN0_PIN)) {
            if (resultDisplayed) {
                reset_calculator();
            }
            append_char('0');
            enteringNumber = 1;
            update_lcd();
            _delay_ms(200);
        }
        
        if (check_button(&BTN1_PIN_REG, BTN1_PIN)) {
            if (resultDisplayed) {
                reset_calculator();
            }
            append_char('1');
            enteringNumber = 1;
            update_lcd();
            _delay_ms(200);
        }
        
        if (check_button(&BTN2_PIN_REG, BTN2_PIN)) {
            if (resultDisplayed) {
                reset_calculator();
            }
            append_char('2');
            enteringNumber = 1;
            update_lcd();
            _delay_ms(200);
        }
        
        if (check_button(&BTN3_PIN_REG, BTN3_PIN)) {
            if (resultDisplayed) {
                reset_calculator();
            }
            append_char('3');
            enteringNumber = 1;
            update_lcd();
            _delay_ms(200);
        }
        
        if (check_button(&BTN4_PIN_REG, BTN4_PIN)) {
            if (resultDisplayed) {
                reset_calculator();
            }
            append_char('4');
            enteringNumber = 1;
            update_lcd();
            _delay_ms(200);
        }
        
        if (check_button(&BTN5_PIN_REG, BTN5_PIN)) {
            if (resultDisplayed) {
                reset_calculator();
            }
            append_char('5');
            enteringNumber = 1;
            update_lcd();
            _delay_ms(200);
        }
        
        if (check_button(&BTN6_PIN_REG, BTN6_PIN)) {
            if (resultDisplayed) {
                reset_calculator();
            }
            append_char('6');
            enteringNumber = 1;
            update_lcd();
            _delay_ms(200);
        }
        
        if (check_button(&BTN7_PIN_REG, BTN7_PIN)) {
            if (resultDisplayed) {
                reset_calculator();
            }
            append_char('7');
            enteringNumber = 1;
            update_lcd();
            _delay_ms(200);
        }
        
        if (check_button(&BTN8_PIN_REG, BTN8_PIN)) {
            if (resultDisplayed) {
                reset_calculator();
            }
            append_char('8');
            enteringNumber = 1;
            update_lcd();
            _delay_ms(200);
        }
        
        if (check_button(&BTN9_PIN_REG, BTN9_PIN)) {
            if (resultDisplayed) {
                reset_calculator();
            }
            append_char('9');
            enteringNumber = 1;
            update_lcd();
            _delay_ms(200);
        }
        
        // Check decimal button
        if (check_button(&DECIMAL_BTN_PIN_REG, DECIMAL_BTN_PIN)) {
            if (resultDisplayed) {
                reset_calculator();
            }
            // Check if there's already a decimal point in the current number
            int numStart = lastOperatorIndex + 1;
            if (numStart < 0) numStart = 0;
            
            int foundDecimal = 0;
            for (int i = numStart; i < exprIndex; i++) {
                if (expression[i] == '.') {
                    foundDecimal = 1;
                    break;
                }
            }
            
            if (!foundDecimal) {
                append_char('.');
                enteringNumber = 1;
                update_lcd();
            }
            _delay_ms(200);
        }
        
        // Check operator button
        if (check_button(&OP_BTN_PIN_REG, OP_BTN_PIN)) {
            if (exprIndex > 0) {  // Only allow operators if we have something in the expression
                operation = (operation + 1) % OPERATOR_COUNT;
                operatorSelected = 1;
                append_operator();
                enteringNumber = 0;
            }
            _delay_ms(200);
        }
        
        // Check trig button
        if (check_button(&TRIG_BTN_PIN_REG, TRIG_BTN_PIN)) {
            trigMode = (trigMode + 1) % TRIG_COUNT;
            trigSelected = 1;
            append_trig_function();
            enteringNumber = 0;
            _delay_ms(200);
        }
        
        // Check enter button
        if (check_button(&ENTER_BTN_PIN_REG, ENTER_BTN_PIN)) {
            if (exprIndex > 0) {  // Only calculate if we have an expression
                process_enter();
            }
            _delay_ms(200);
        }
    }
    
    return 0;
}
