#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// Constants
#define PI 3.14159265358979323846f
#define E  2.71828182845904523536f
#define DEBOUNCE_DELAY 200 // ms
#define MAX_EXPR_LEN 64
#define MAX_STACK_SIZE 32

// LCD Pins (AVR ports)
#define LCD_RS_PORT PORTB
#define LCD_RS_PIN  4  // Arduino pin 12 = PB4
#define LCD_E_PORT  PORTB
#define LCD_E_PIN   3  // Arduino pin 11 = PB3
#define LCD_DATA_PORT PORTD
#define LCD_D4_PIN  5  // Arduino pin 5 = PD5
#define LCD_D5_PIN  4  // Arduino pin 4 = PD4
#define LCD_D6_PIN  3  // Arduino pin 3 = PD3
#define LCD_D7_PIN  2  // Arduino pin 2 = PD2

// Button pins on AVR ports
#define DIGIT_BTN_PIN     0  // Arduino pin A0 = PC0 (cycles through digits 0-9)
#define OPEN_PAREN_PIN    1  // Arduino pin A1 = PC1
#define CLOSE_PAREN_PIN   2  // Arduino pin A2 = PC2
#define OP_BTN_PIN        3  // Arduino pin A3 = PC3 (cycles operations)
#define FUNC_BTN_PIN      4  // Arduino pin A4 = PC4 (cycles functions)
#define SET_BTN_PIN       7  // Arduino pin 7 = PD7
#define EQUALS_BTN_PIN    0  // Arduino pin 8 = PB0
#define CLEAR_BTN_PIN     1  // Arduino pin 9 = PB1

// Custom math functions to replace math.h
float my_abs(float x);
int is_zero(float x);
int is_integer(float x);
float my_sqrt(float x);

// Function prototypes
void setup_timer(void);
void setup_buttons(void);
void setup_lcd(void);
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_string(const char *str);
void lcd_position(uint8_t row, uint8_t col);
void lcd_clear(void);

void update_display(void);
void append_to_expression(const char* str);
void backspace_expression(void);
float evaluate_expression(void);
int get_precedence(char op);
int is_operator(char c);

float compute_sin_cos(float x, float* sin_val, float* cos_val);
float compute_exp(float x);
float compute_ln(float x);
float compute_log10(float x);
float compute_power(float base, float exponent);
float compute_asin(float x);
float compute_acos(float x);
float compute_atan(float x);

// Global variables
volatile uint32_t milliseconds = 0;
uint8_t current_digit = 0;      // Current selected digit (0-9)
uint8_t current_operation = 0;  // 0=+, 1=-, 2=*, 3=/, 4=^
int8_t current_function = 0;    // 0=sin, 1=cos, 2=tan, 3=xÂ², 4=e^x, 5=ln, 6=log, 7=e, 8=., 9=sinâ»Â¹, 10=cosâ»Â¹, 11=tanâ»Â¹, 12=Ï€
uint32_t last_button_time = 0;
uint8_t input_mode = 0;         // 0=digit, 1=operation, 2=function
uint8_t parenthesis_depth = 0;
uint8_t decimal_entered = 0;    // Flag to prevent multiple decimal points

char expression[MAX_EXPR_LEN] = {0};
uint8_t expr_index = 0;

const char* operation_symbols = "+-*/^";
const char* function_names[] = {"sin", "cos", "tan", "x^2", "e^x", "ln", "log", "e", ".", "sin^-1", "cos^-1", "tan^-1", "pi"};

// Custom math functions implementation
float my_abs(float x) {
    return (x < 0.0f) ? -x : x;
}

int is_zero(float x) {
    return my_abs(x) < 0.0001f;
}

int is_integer(float x) {
    float diff = x - (int)x;
    return (my_abs(diff) < 0.0001f);
}

float my_sqrt(float x) {
    if (x <= 0.0f) return 0.0f;
    float result = x;
    // Newton's method
    for (int i = 0; i < 10; i++) {
        result = 0.5f * (result + x/result);
    }
    return result;
}

int main(void) {
    // Initialize hardware
    setup_timer();
    setup_buttons();
    setup_lcd();
    
    // Enable global interrupts
    sei();
    
    // Display startup message
    lcd_clear();
    lcd_string("Scientific Calc");
    _delay_ms(1000);
    lcd_clear();
    lcd_string("0");
    
    while(1) {
        // Check if enough time has passed since last button press (debouncing)
        if (milliseconds - last_button_time > DEBOUNCE_DELAY) {
            // Check digit button (cycles through 0-9)
            if (!(PINC & (1 << DIGIT_BTN_PIN))) {
                current_digit = (current_digit + 1) % 10;
                input_mode = 0;
                update_display();
                last_button_time = milliseconds;
            }
            
            // Check open parenthesis button
            if (!(PINC & (1 << OPEN_PAREN_PIN))) {
                append_to_expression("(");
                parenthesis_depth++;
                update_display();
                last_button_time = milliseconds;
            }
            
            // Check close parenthesis button
            if (!(PINC & (1 << CLOSE_PAREN_PIN))) {
                if (parenthesis_depth > 0) {
                    append_to_expression(")");
                    parenthesis_depth--;
                    update_display();
                }
                last_button_time = milliseconds;
            }
            
            // Check operation button (cycles through +, -, *, /, ^)
            if (!(PINC & (1 << OP_BTN_PIN))) {
                current_operation = (current_operation + 1) % 5; // Now includes ^ operator
                input_mode = 1;
                update_display();
                last_button_time = milliseconds;
            }
            
            // Check function button (cycles through all functions)
            if (!(PINC & (1 << FUNC_BTN_PIN))) {
                current_function = (current_function + 1) % 13; // Increased number of functions
                input_mode = 2;
                update_display();
                last_button_time = milliseconds;
            }
            
            // Check set button - commits current input to expression
            if (!(PIND & (1 << SET_BTN_PIN))) {
                if (input_mode == 0) {
                    // Add current digit
                    char digit_str[2];
                    digit_str[0] = '0' + current_digit;
                    digit_str[1] = '\0';
                    append_to_expression(digit_str);
                }
                else if (input_mode == 1) {
                    // Add operation
                    char op_str[2];
                    op_str[0] = operation_symbols[current_operation];
                    op_str[1] = '\0';
                    append_to_expression(op_str);
                }
                else if (input_mode == 2) {
                    // Add function or special value
                    switch (current_function) {
                        case 7: // e
                            append_to_expression("2.71828");
                            break;
                        case 8: // decimal point
                            if (!decimal_entered) {
                                append_to_expression(".");
                                decimal_entered = 1;
                            }
                            break;
                        case 12: // Ï€
                            append_to_expression("3.14159");
                            break;
                        default: // All other functions
                            append_to_expression(function_names[current_function]);
                            append_to_expression("(");
                            parenthesis_depth++;
                            break;
                    }
                }
                
                update_display();
                last_button_time = milliseconds;
            }
            
            // Check equals button - evaluates expression
            if (!(PINB & (1 << EQUALS_BTN_PIN))) {
                // Close any open parentheses
                while (parenthesis_depth > 0) {
                    append_to_expression(")");
                    parenthesis_depth--;
                }
                
                // Evaluate the expression
                float result = evaluate_expression();
                
                // Display result
                lcd_clear();
                lcd_string(expression);
                lcd_position(1, 0);
                lcd_string("= ");
                
                char result_str[14];
                dtostrf(result, 1, 4, result_str);
                
                // Trim trailing zeros
                char *p;
                p = result_str + strlen(result_str) - 1;
                while (p > result_str && *p == '0' && *(p-1) != '.') {
                    *p-- = '\0';
                }
                
                lcd_string(result_str);
                
                // Reset for next expression
                expr_index = 0;
                expression[0] = '\0';
                decimal_entered = 0;
                
                last_button_time = milliseconds;
            }
            
            // Check clear button - acts as backspace
            if (!(PINB & (1 << CLEAR_BTN_PIN))) {
                backspace_expression();
                update_display();
                last_button_time = milliseconds;
            }
        }
    }
    
    return 0;
}

// Initialize timer for millisecond counting
void setup_timer(void) {
    TCCR0A = (1 << WGM01); // CTC mode
    TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64
    OCR0A = 249; // 16MHz/64/250 = 1kHz (1ms)
    TIMSK0 = (1 << OCIE0A); // Enable compare match interrupt
}

// Timer0 compare match ISR
ISR(TIMER0_COMPA_vect) {
    milliseconds++;
}

// Initialize buttons
void setup_buttons(void) {
    // Set button pins as inputs
    DDRC &= ~((1 << DIGIT_BTN_PIN) | (1 << OPEN_PAREN_PIN) | (1 << CLOSE_PAREN_PIN) | 
              (1 << OP_BTN_PIN) | (1 << FUNC_BTN_PIN));
    DDRD &= ~(1 << SET_BTN_PIN);
    DDRB &= ~((1 << EQUALS_BTN_PIN) | (1 << CLEAR_BTN_PIN));
    
    // Enable pull-up resistors
    PORTC |= (1 << DIGIT_BTN_PIN) | (1 << OPEN_PAREN_PIN) | (1 << CLOSE_PAREN_PIN) | 
             (1 << OP_BTN_PIN) | (1 << FUNC_BTN_PIN);
    PORTD |= (1 << SET_BTN_PIN);
    PORTB |= (1 << EQUALS_BTN_PIN) | (1 << CLEAR_BTN_PIN);
}

// Initialize LCD
void setup_lcd(void) {
    // Set LCD pins as outputs
    DDRB |= (1 << LCD_RS_PIN) | (1 << LCD_E_PIN); // RS, E
    DDRD |= (1 << LCD_D4_PIN) | (1 << LCD_D5_PIN) | (1 << LCD_D6_PIN) | (1 << LCD_D7_PIN); // D4-D7
    
    _delay_ms(50); // Wait for LCD to power up
    
    // Send init sequence
    lcd_command(0x33);
    _delay_ms(5);
    lcd_command(0x32);
    _delay_ms(1);
    
    // Configure LCD
    lcd_command(0x28); // 4-bit mode, 2 lines, 5x8 font
    lcd_command(0x0C); // Display on, cursor off, blink off
    lcd_command(0x06); // Increment cursor, no shift
    lcd_command(0x01); // Clear display
    _delay_ms(2);
}

// Send command to LCD
void lcd_command(uint8_t cmd) {
    // RS = 0 for command
    LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
    
    // Send high nibble
    LCD_DATA_PORT &= ~((1 << LCD_D4_PIN) | (1 << LCD_D5_PIN) | (1 << LCD_D6_PIN) | (1 << LCD_D7_PIN));
    if (cmd & 0x10) LCD_DATA_PORT |= (1 << LCD_D4_PIN);
    if (cmd & 0x20) LCD_DATA_PORT |= (1 << LCD_D5_PIN);
    if (cmd & 0x40) LCD_DATA_PORT |= (1 << LCD_D6_PIN);
    if (cmd & 0x80) LCD_DATA_PORT |= (1 << LCD_D7_PIN);
    
    // Pulse enable
    LCD_E_PORT |= (1 << LCD_E_PIN);
    _delay_us(1);
    LCD_E_PORT &= ~(1 << LCD_E_PIN);
    _delay_us(100);
    
    // Send low nibble
    LCD_DATA_PORT &= ~((1 << LCD_D4_PIN) | (1 << LCD_D5_PIN) | (1 << LCD_D6_PIN) | (1 << LCD_D7_PIN));
    if (cmd & 0x01) LCD_DATA_PORT |= (1 << LCD_D4_PIN);
    if (cmd & 0x02) LCD_DATA_PORT |= (1 << LCD_D5_PIN);
    if (cmd & 0x04) LCD_DATA_PORT |= (1 << LCD_D6_PIN);
    if (cmd & 0x08) LCD_DATA_PORT |= (1 << LCD_D7_PIN);
    
    // Pulse enable
    LCD_E_PORT |= (1 << LCD_E_PIN);
    _delay_us(1);
    LCD_E_PORT &= ~(1 << LCD_E_PIN);
    _delay_us(100);
}

// Send data to LCD
void lcd_data(uint8_t data) {
    // RS = 1 for data
    LCD_RS_PORT |= (1 << LCD_RS_PIN);
    
    // Send high nibble
    LCD_DATA_PORT &= ~((1 << LCD_D4_PIN) | (1 << LCD_D5_PIN) | (1 << LCD_D6_PIN) | (1 << LCD_D7_PIN));
    if (data & 0x10) LCD_DATA_PORT |= (1 << LCD_D4_PIN);
    if (data & 0x20) LCD_DATA_PORT |= (1 << LCD_D5_PIN);
    if (data & 0x40) LCD_DATA_PORT |= (1 << LCD_D6_PIN);
    if (data & 0x80) LCD_DATA_PORT |= (1 << LCD_D7_PIN);
    
    // Pulse enable
    LCD_E_PORT |= (1 << LCD_E_PIN);
    _delay_us(1);
    LCD_E_PORT &= ~(1 << LCD_E_PIN);
    _delay_us(100);
    
    // Send low nibble
    LCD_DATA_PORT &= ~((1 << LCD_D4_PIN) | (1 << LCD_D5_PIN) | (1 << LCD_D6_PIN) | (1 << LCD_D7_PIN));
    if (data & 0x01) LCD_DATA_PORT |= (1 << LCD_D4_PIN);
    if (data & 0x02) LCD_DATA_PORT |= (1 << LCD_D5_PIN);
    if (data & 0x04) LCD_DATA_PORT |= (1 << LCD_D6_PIN);
    if (data & 0x08) LCD_DATA_PORT |= (1 << LCD_D7_PIN);
    
    // Pulse enable
    LCD_E_PORT |= (1 << LCD_E_PIN);
    _delay_us(1);
    LCD_E_PORT &= ~(1 << LCD_E_PIN);
    _delay_us(100);
}

// Send string to LCD
void lcd_string(const char *str) {
    while(*str) {
        lcd_data(*str++);
    }
}

// Set cursor position on LCD
void lcd_position(uint8_t row, uint8_t col) {
    uint8_t address;
    if(row == 0) {
        address = 0x80 + col;
    } else {
        address = 0xC0 + col;
    }
    lcd_command(address);
}

// Clear LCD
void lcd_clear(void) {
    lcd_command(0x01);
    _delay_ms(2);
}

// Update LCD display based on calculator state
void update_display(void) {
    lcd_clear();
    char buffer[17];
    
    // First line: Show expression
    if (expr_index > 0) {
        // If expression is longer than 16 chars, show the last 16
        if (expr_index > 16) {
            lcd_string(&expression[expr_index - 16]);
        } else {
            lcd_string(expression);
        }
    } else {
        lcd_string("0");
    }
    
    // Second line: Show current selection based on input mode
    lcd_position(1, 0);
    switch(input_mode) {
        case 0: // Digit mode
            sprintf(buffer, "Digit: %d", current_digit);
            lcd_string(buffer);
            break;
        case 1: // Operation mode
            sprintf(buffer, "Op: %c", operation_symbols[current_operation]);
            lcd_string(buffer);
            break;
        case 2: // Function mode
            sprintf(buffer, "Func: %s", function_names[current_function]);
            lcd_string(buffer);
            break;
    }
    
    // Show parenthesis depth if any are open
    if (parenthesis_depth > 0) {
        lcd_position(1, 10);
        sprintf(buffer, "P:%d", parenthesis_depth);
        lcd_string(buffer);
    }
}

// Append text to the expression
void append_to_expression(const char* str) {
    while (*str && expr_index < MAX_EXPR_LEN - 1) {
        expression[expr_index++] = *str++;
    }
    expression[expr_index] = '\0';
    
    // Reset decimal flag if a non-decimal character is entered
    if (strcmp(str, ".") != 0) {
        if (strchr(str, '.') == NULL) {
            decimal_entered = 0;
        }
    }
}

// Backspace: remove last character or function from expression
void backspace_expression(void) {
    if (expr_index > 0) {
        // Check if we're removing a decimal point
        if (expression[expr_index - 1] == '.') {
            decimal_entered = 0;
        }
        
        // Check if last character is closing parenthesis
        if (expression[expr_index - 1] == ')') {
            parenthesis_depth++;
        }
        // Check if last character is opening parenthesis
        else if (expression[expr_index - 1] == '(') {
            parenthesis_depth--;
            
            // Check if there's a function name before the parenthesis
            const char* func_names[] = {"sin", "cos", "tan", "ln", "log", "sin^-1", "cos^-1", "tan^-1"};
            for (int i = 0; i < 8; i++) {
                int func_len = strlen(func_names[i]);
                if (expr_index >= func_len + 1 && 
                    strncmp(&expression[expr_index - func_len - 1], func_names[i], func_len) == 0) {
                    // Remove the whole function name
                    expr_index -= func_len;
                    break;
                }
            }
            
            // Check for x^2 and e^x separately
            if (expr_index >= 4 && strncmp(&expression[expr_index - 4], "x^2", 3) == 0) {
                expr_index -= 3;
            }
            else if (expr_index >= 4 && strncmp(&expression[expr_index - 4], "e^x", 3) == 0) {
                expr_index -= 3;
            }
        }
        
        // Remove last character
        expr_index--;
        expression[expr_index] = '\0';
    }
}

// Is this character an operator?
int is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

// Get operator precedence
int get_precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;  // Higher precedence for power
        case 's':     // sin
        case 'c':     // cos
        case 't':     // tan
        case 'l':     // ln
        case 'g':     // log
        case 'a':     // asin
        case 'b':     // acos
        case 'd':     // atan
        case 'q':     // x^2
        case 'e':     // e^x
            return 4;  // Highest precedence for functions
        default:
            return 0;
    }
}

// Evaluate expression with proper operator precedence
float evaluate_expression(void) {
    // Copy expression so we can modify it
    char expr_copy[MAX_EXPR_LEN];
    strcpy(expr_copy, expression);
    
    // Stacks for Shunting Yard algorithm
    float value_stack[MAX_EXPR_LEN];
    int value_stack_top = 0;
    
    char operator_stack[MAX_EXPR_LEN];
    int operator_stack_top = 0;
    
    // Parse the expression
    int i = 0;
    while (i < strlen(expr_copy)) {
        // Skip spaces
        if (expr_copy[i] == ' ') {
            i++;
            continue;
        }
        
        // Parse numbers
        if (isdigit(expr_copy[i]) || (expr_copy[i] == '.' && i+1 < strlen(expr_copy) && isdigit(expr_copy[i+1]))) {
            float number = 0.0;
            int decimal_places = 0;
            int decimal_point = 0;
            
            while (i < strlen(expr_copy) && 
                  (isdigit(expr_copy[i]) || expr_copy[i] == '.')) {
                if (expr_copy[i] == '.') {
                    decimal_point = 1;
                } else {
                    if (decimal_point) {
                        decimal_places++;
                        float decimal_value = (expr_copy[i] - '0');
                        for (int j = 0; j < decimal_places; j++) {
                            decimal_value /= 10.0f;
                        }
                        number += decimal_value;
                    } else {
                        number = number * 10.0f + (expr_copy[i] - '0');
                    }
                }
                i++;
            }
            
            value_stack[value_stack_top++] = number;
            continue;
        }
        
        // Parse functions
        if (strncmp(&expr_copy[i], "sin^-1", 6) == 0 || strncmp(&expr_copy[i], "asin", 4) == 0) {
            operator_stack[operator_stack_top++] = 'a'; // asin
            i += (expr_copy[i] == 'a') ? 4 : 6;
            continue;
        }
        if (strncmp(&expr_copy[i], "cos^-1", 6) == 0 || strncmp(&expr_copy[i], "acos", 4) == 0) {
            operator_stack[operator_stack_top++] = 'b'; // acos
            i += (expr_copy[i] == 'a') ? 4 : 6;
            continue;
        }
        if (strncmp(&expr_copy[i], "tan^-1", 6) == 0 || strncmp(&expr_copy[i], "atan", 4) == 0) {
            operator_stack[operator_stack_top++] = 'd'; // atan
            i += (expr_copy[i] == 'a') ? 4 : 6;
            continue;
        }
        if (strncmp(&expr_copy[i], "sin", 3) == 0) {
            operator_stack[operator_stack_top++] = 's'; // sin
            i += 3;
            continue;
        }
        if (strncmp(&expr_copy[i], "cos", 3) == 0) {
            operator_stack[operator_stack_top++] = 'c'; // cos
            i += 3;
            continue;
        }
        if (strncmp(&expr_copy[i], "tan", 3) == 0) {
            operator_stack[operator_stack_top++] = 't'; // tan
            i += 3;
            continue;
        }
        if (strncmp(&expr_copy[i], "ln", 2) == 0) {
            operator_stack[operator_stack_top++] = 'l'; // ln
            i += 2;
            continue;
        }
        if (strncmp(&expr_copy[i], "log", 3) == 0) {
            operator_stack[operator_stack_top++] = 'g'; // log
            i += 3;
            continue;
        }
        if (strncmp(&expr_copy[i], "x^2", 3) == 0) {
            operator_stack[operator_stack_top++] = 'q'; // square
            i += 3;
            continue;
        }
        if (strncmp(&expr_copy[i], "e^x", 3) == 0) {
            operator_stack[operator_stack_top++] = 'e'; // e^x
            i += 3;
            continue;
        }
        
        // Handle parentheses
        if (expr_copy[i] == '(') {
            operator_stack[operator_stack_top++] = '(';
            i++;
            continue;
        }
        
        if (expr_copy[i] == ')') {
            // Pop operators until we find a matching open parenthesis
            while (operator_stack_top > 0 && operator_stack[operator_stack_top - 1] != '(') {
                char op = operator_stack[--operator_stack_top];
                
                // Apply function operators
                if (op == 's' || op == 'c' || op == 't' || op == 'l' || op == 'g' || 
                    op == 'a' || op == 'b' || op == 'd' || op == 'q' || op == 'e') {
                    
                    float arg = value_stack[--value_stack_top];
                    float result = 0.0f;
                    
                    switch (op) {
                        case 's': { // sin
                            float sin_val, cos_val;
                            compute_sin_cos(arg, &sin_val, &cos_val);
                            result = sin_val;
                            break;
                        }
                        case 'c': { // cos
                            float sin_val, cos_val;
                            compute_sin_cos(arg, &sin_val, &cos_val);
                            result = cos_val;
                            break;
                        }
                        case 't': { // tan
                            float sin_val, cos_val;
                            compute_sin_cos(arg, &sin_val, &cos_val);
                            if (!is_zero(cos_val)) {
                                result = sin_val / cos_val;
                            } else {
                                result = 99999.0f; // Undefined
                            }
                            break;
                        }
                        case 'l': // ln (natural log)
                            result = compute_ln(arg);
                            break;
                        case 'g': // log (base 10)
                            result = compute_log10(arg);
                            break;
                        case 'a': // asin (sin^-1)
                            result = compute_asin(arg);
                            break;
                        case 'b': // acos (cos^-1)
                            result = compute_acos(arg);
                            break;
                        case 'd': // atan (tan^-1)
                            result = compute_atan(arg);
                            break;
                        case 'q': // x^2 (square)
                            result = arg * arg;
                            break;
                        case 'e': // e^x (exponential)
                            result = compute_exp(arg);
                            break;
                    }
                    
                    value_stack[value_stack_top++] = result;
                } 
                // Apply binary operators
                else {
                    float b = value_stack[--value_stack_top];
                    float a = value_stack[--value_stack_top];
                    float result = 0.0f;
                    
                    switch (op) {
                        case '+': result = a + b; break;
                        case '-': result = a - b; break;
                        case '*': result = a * b; break;
                        case '/': 
                            if (!is_zero(b)) {
                                result = a / b;
                            } else {
                                result = 99999.0f; // Represent infinity
                            }
                            break;
                        case '^': 
                            result = compute_power(a, b);
                            break;
                    }
                    
                    value_stack[value_stack_top++] = result;
                }
            }
            
            // Pop the opening parenthesis
            if (operator_stack_top > 0) {
                operator_stack_top--;
            }
            
            i++;
            continue;
        }
        
        // Parse operators
        if (is_operator(expr_copy[i])) {
            // Pop operators with higher or equal precedence, except for right-associative ^ operator
            while (operator_stack_top > 0 && 
                   operator_stack[operator_stack_top - 1] != '(' &&
                   ((expr_copy[i] != '^' && 
                     get_precedence(operator_stack[operator_stack_top - 1]) >= get_precedence(expr_copy[i])) ||
                    (expr_copy[i] == '^' && 
                     get_precedence(operator_stack[operator_stack_top - 1]) > get_precedence(expr_copy[i])))) {
                
                char op = operator_stack[--operator_stack_top];
                
                if (value_stack_top >= 2) { // Ensure we have enough operands
                    float b = value_stack[--value_stack_top];
                    float a = value_stack[--value_stack_top];
                    float result = 0.0f;
                    
                    switch (op) {
                        case '+': result = a + b; break;
                        case '-': result = a - b; break;
                        case '*': result = a * b; break;
                        case '/': 
                            if (!is_zero(b)) {
                                result = a / b;
                            } else {
                                result = 99999.0f; // Represent infinity
                            }
                            break;
                        case '^': 
                            result = compute_power(a, b);
                            break;
                    }
                    
                    value_stack[value_stack_top++] = result;
                }
            }
            
            // Push the current operator
            operator_stack[operator_stack_top++] = expr_copy[i];
            i++;
            continue;
        }
        
        // Skip unrecognized characters
        i++;
    }
    
    // Process remaining operators
    while (operator_stack_top > 0) {
        char op = operator_stack[--operator_stack_top];
        
        // Handle function operators
        if (op == 's' || op == 'c' || op == 't' || op == 'l' || op == 'g' || 
            op == 'a' || op == 'b' || op == 'd' || op == 'q' || op == 'e') {
            
            if (value_stack_top >= 1) { // Ensure we have an operand
                float arg = value_stack[--value_stack_top];
                float result = 0.0f;
                
                switch (op) {
                    case 's': { // sin
                        float sin_val, cos_val;
                        compute_sin_cos(arg, &sin_val, &cos_val);
                        result = sin_val;
                        break;
                    }
                    case 'c': { // cos
                        float sin_val, cos_val;
                        compute_sin_cos(arg, &sin_val, &cos_val);
                        result = cos_val;
                        break;
                    }
                    case 't': { // tan
                        float sin_val, cos_val;
                        compute_sin_cos(arg, &sin_val, &cos_val);
                        if (!is_zero(cos_val)) {
                            result = sin_val / cos_val;
                        } else {
                            result = 99999.0f; // Undefined
                        }
                        break;
                    }
                    case 'l': // ln (natural log)
                        result = compute_ln(arg);
                        break;
                    case 'g': // log (base 10)
                        result = compute_log10(arg);
                        break;
                    case 'a': // asin (sin^-1)
                        result = compute_asin(arg);
                        break;
                    case 'b': // acos (cos^-1)
                        result = compute_acos(arg);
                        break;
                    case 'd': // atan (tan^-1)
                        result = compute_atan(arg);
                        break;
                    case 'q': // x^2 (square)
                        result = arg * arg;
                        break;
                    case 'e': // e^x (exponential)
                        result = compute_exp(arg);
                        break;
                }
                
                value_stack[value_stack_top++] = result;
            }
        }
        // Handle binary operators
        else if (op != '(') { // Skip any remaining open parentheses
            if (value_stack_top >= 2) { // Ensure we have enough operands
                float b = value_stack[--value_stack_top];
                float a = value_stack[--value_stack_top];
                float result = 0.0f;
                
                switch (op) {
                    case '+': result = a + b; break;
                    case '-': result = a - b; break;
                    case '*': result = a * b; break;
                    case '/': 
                        if (!is_zero(b)) {
                            result = a / b;
                        } else {
                            result = 99999.0f; // Represent infinity
                        }
                        break;
                    case '^': 
                        result = compute_power(a, b);
                        break;
                }
                
                value_stack[value_stack_top++] = result;
            }
        }
    }
    
    // Final result should be on top of value stack
    return (value_stack_top > 0) ? value_stack[value_stack_top - 1] : 0;
}

// Implementation of mathematical functions using Euler's method
float compute_sin_cos(float x, float* sin_val, float* cos_val) {
    float s = 0.0f;  // sin(0)
    float c = 1.0f;  // cos(0)
    float h = 0.0001f; // smaller step size for better accuracy
    
    // Normalize x to [0, 2Ï€)
    while (x >= 2*PI) x -= 2*PI;
    while (x < 0) x += 2*PI;
    
    for (float t = 0.0f; t < x; t += h) {
        float s_new = s + h * c;
        float c_new = c - h * s;
        s = s_new;
        c = c_new;
    }
    
    *sin_val = s;
    *cos_val = c;
    return s;
}

float compute_exp(float x) {
    float result = 1.0f;  // e^0
    float h = 0.01f;      // step size
    
    if (x >= 0) {
        for (float t = 0.0f; t < x; t += h) {
            result += h * result;
        }
    } else {
        x = -x;
        for (float t = 0.0f; t < x; t += h) {
            result -= h * result;
        }
        result = 1.0f / result;
    }
    
    return result;
}

float compute_ln(float x) {
    if (x <= 0) return -99999.0f; // Error value for ln(negative)
    
    float result = 0.0f;  // ln(1)
    float h = 0.001f;     // step size
    float current = 1.0f;
    
    if (x > 1.0f) {
        while (current < x) {
            result += h * (1.0f / current);
            current += h * current;
        }
    } else { // 0 < x < 1
        while (current > x) {
            current -= h * current;
            result -= h * (1.0f / current);
        }
    }
    
    return result;
}

float compute_log10(float x) {
    // log10(x) = ln(x) / ln(10)
    return compute_ln(x) / compute_ln(10.0f);
}

float compute_power(float base, float exponent) {
    // Special cases
    if (exponent == 0.0f) return 1.0f;
    if (base == 0.0f) return 0.0f;
    
    // Integer exponents - use direct multiplication for better accuracy
    if (is_integer(exponent)) {
        if (exponent > 0) {
            float result = 1.0f;
            for(int i = 0; i < (int)exponent; i++) {
                result *= base;
            }
            return result;
        } else {
            float result = 1.0f;
            for(int i = 0; i < (int)-exponent; i++) {
                result *= base;
            }
            return 1.0f / result;
        }
    }
    
    // For fractional exponents: a^b = e^(b*ln(a))
    return compute_exp(exponent * compute_ln(base));
}

// Inverse trigonometric functions - implemented using numerical methods

float compute_asin(float x) {
    // Make sure x is in valid range [-1, 1]
    if (x < -1.0f) x = -1.0f;
    if (x > 1.0f) x = 1.0f;
    
    // Special cases
    if (x == 0.0f) return 0.0f;
    if (x == 1.0f) return PI/2;
    if (x == -1.0f) return -PI/2;
    
    // Binary search between -Ï€/2 and Ï€/2
    float low = -PI/2;
    float high = PI/2;
    float mid, sin_mid, cos_mid;
    
    for (int i = 0; i < 20; i++) { // 20 iterations for precision
        mid = (low + high) / 2;
        compute_sin_cos(mid, &sin_mid, &cos_mid);
        
        if (sin_mid < x) {
            low = mid;
        } else {
            high = mid;
        }
    }
    
    return (low + high) / 2;
}

float compute_acos(float x) {
    // acos(x) = Ï€/2 - asin(x)
    return PI/2 - compute_asin(x);
}

float compute_atan(float x) {
    // For small x, atan(x) â‰ˆ x
    if (-0.1f < x && x < 0.1f) return x;
    
    // Use identity: atan(x) = Ï€/2 - atan(1/x) for |x| > 1
    if (x > 1.0f) return PI/2 - compute_atan(1.0f/x);
    if (x < -1.0f) return -PI/2 - compute_atan(1.0f/x);
    
    // For -1 â‰¤ x â‰¤ 1, use numerical integration of 1/(1+tÂ²) from 0 to x
    float result = 0.0f;
    float h = 0.001f; // step size
    
    for (float t = 0.0f; t < my_abs(x); t += h) {
        result += h / (1.0f + t*t);
    }
    
    return (x >= 0) ? result : -result;
}

