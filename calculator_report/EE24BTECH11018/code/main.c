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

// Custom math functions to avoid math.h
float my_abs(float x) {
    return (x < 0.0f) ? -x : x;
}

int is_zero(float x) {
    return my_abs(x) < 0.0001f;
}

float my_sqrt(float x) {
    if (x <= 0.0f) return 0.0f;
    float result = x;
    for (int i = 0; i < 10; i++) {
        result = 0.5f * (result + x/result);
    }
    return result;
}

int is_integer(float x) {
    float diff = x - (int)x;
    return (my_abs(diff) < 0.0001f);
}

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
float compute_asin(float x);
float compute_acos(float x);
float compute_atan(float x);
float compute_exp(float x);
float compute_ln(float x);
float compute_log10(float x);
float compute_power(float base, float exponent);

// Global variables
volatile uint32_t milliseconds = 0;
uint8_t current_digit = 0;      // Current selected digit (0-9)
uint8_t decimal_point_mode = 0; // 0=digits, 1=decimal point
uint8_t current_operation = 0;  // 0=+, 1=-, 2=*, 3=/
int8_t current_function = 0;    // Function index (see function_names array)
uint32_t last_button_time = 0;
uint32_t digit_button_press_time = 0;
uint8_t input_mode = 0;         // 0=digit, 1=operation, 2=function
uint8_t parenthesis_depth = 0;

char expression[MAX_EXPR_LEN] = {0};
uint8_t expr_index = 0;

const char* operation_symbols = "+-*/";
const char* function_names[] = {"sin", "cos", "tan", "sin^-1", "cos^-1", "tan^-1", 
                               "x^2", "e^x", "^", "ln", "log", "e", "pi", "."};
const uint8_t function_count = 14; // Number of available functions

int main(void) {
    // Initialize hardware
    setup_timer();
    setup_buttons();
    setup_lcd();
    
    // Enable global interrupts
    sei();
    
    // Display startup message
    lcd_clear();
    lcd_string("Calculator");
    _delay_ms(1000);
    lcd_clear();
    lcd_string("0");
    
    while(1) {
        // Check if enough time has passed since last button press (debouncing)
        if (milliseconds - last_button_time > DEBOUNCE_DELAY) {
            // Check digit button (cycles through 0-9 and decimal point)
            if (!(PINC & (1 << DIGIT_BTN_PIN))) {
                // Track how long button is pressed for decimal point toggle
                if (digit_button_press_time == 0) {
                    digit_button_press_time = milliseconds;
                }
                
                // Long press (over 1 second) toggles decimal point mode
                if (milliseconds - digit_button_press_time > 1000) {
                    decimal_point_mode = !decimal_point_mode;
                    digit_button_press_time = milliseconds; // Reset to prevent multiple toggles
                    input_mode = 0;
                    update_display();
                } 
                else {
                    // Regular press cycles through digits
                    if (!decimal_point_mode) {
                        current_digit = (current_digit + 1) % 10;
                    }
                    input_mode = 0;
                    update_display();
                }
                
                last_button_time = milliseconds;
            } 
            else {
                // Button released, reset press timer
                digit_button_press_time = 0;
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
            
            // Check operation button (cycles through +, -, *, /)
            if (!(PINC & (1 << OP_BTN_PIN))) {
                current_operation = (current_operation + 1) % 4;
                input_mode = 1;
                update_display();
                last_button_time = milliseconds;
            }
            
            // Check function button (cycles through functions)
            if (!(PINC & (1 << FUNC_BTN_PIN))) {
                current_function = (current_function + 1) % function_count;
                input_mode = 2;
                update_display();
                last_button_time = milliseconds;
            }
            
            // Check set button - commits current input to expression
            if (!(PIND & (1 << SET_BTN_PIN))) {
                if (input_mode == 0) {
                    // Add current digit or decimal point
                    if (decimal_point_mode) {
                        append_to_expression(".");
                        decimal_point_mode = 0; // Reset after use
                    } else {
                        char digit_str[2];
                        digit_str[0] = '0' + current_digit;
                        digit_str[1] = '\0';
                        append_to_expression(digit_str);
                    }
                }
                else if (input_mode == 1) {
                    // Add operation
                    char op_str[2];
                    op_str[0] = operation_symbols[current_operation];
                    op_str[1] = '\0';
                    append_to_expression(op_str);
                }
                else if (input_mode == 2) {
                    // Add function
                    if (current_function == 11) { // e constant
                        append_to_expression("2.718");
                    } 
                    else if (current_function == 12) { // pi
                        append_to_expression("3.142");
                    }
                    else if (current_function == 13) { // decimal point
                        append_to_expression(".");
                    }
                    else {
                        append_to_expression(function_names[current_function]);
                        append_to_expression("(");
                        parenthesis_depth++;
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
                
                // Reset for next expression but keep result as current number
                expr_index = 0;
                expression[0] = '\0';
                
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
            if (decimal_point_mode) {
                lcd_string("Decimal point");
            } else {
                sprintf(buffer, "Digit: %d", current_digit);
                lcd_string(buffer);
            }
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
        int remaining_space = 16 - strlen(buffer);
        lcd_position(1, 16 - 4);
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
}

// Backspace: remove last character or function from expression
void backspace_expression(void) {
    if (expr_index > 0) {
        // Check if last character is closing parenthesis
        if (expression[expr_index - 1] == ')') {
            parenthesis_depth++;
        }
        // Check if last character is opening parenthesis
        else if (expression[expr_index - 1] == '(') {
            parenthesis_depth--;
            
            // Check if there's a function name before the parenthesis
            for (int i = 0; i < function_count; i++) {
                int func_len = strlen(function_names[i]);
                if (expr_index >= func_len + 1 && 
                    strncmp(&expression[expr_index - func_len - 1], function_names[i], func_len) == 0) {
                    // Remove the whole function name
                    expr_index -= func_len;
                    break;
                }
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
        case 'a':     // asin
        case 'b':     // acos
        case 'd':     // atan
        case 'l':     // ln
        case 'g':     // log
        case 'q':     // square
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
            float number = 0.0f;
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
        // Regular trig functions
        if (strncmp(&expr_copy[i], "sin(", 4) == 0 && expr_copy[i+3] != '^') {
            operator_stack[operator_stack_top++] = 's';
            i += 3; // Skip to opening parenthesis
            continue;
        }
        if (strncmp(&expr_copy[i], "cos(", 4) == 0 && expr_copy[i+3] != '^') {
            operator_stack[operator_stack_top++] = 'c';
            i += 3;
            continue;
        }
        if (strncmp(&expr_copy[i], "tan(", 4) == 0 && expr_copy[i+3] != '^') {
            operator_stack[operator_stack_top++] = 't';
            i += 3;
            continue;
        }
        
        // Inverse trig functions
        if (strncmp(&expr_copy[i], "sin^-1(", 7) == 0) {
            operator_stack[operator_stack_top++] = 'a'; // asin
            i += 6;
            continue;
        }
        if (strncmp(&expr_copy[i], "cos^-1(", 7) == 0) {
            operator_stack[operator_stack_top++] = 'b'; // acos
            i += 6;
            continue;
        }
        if (strncmp(&expr_copy[i], "tan^-1(", 7) == 0) {
            operator_stack[operator_stack_top++] = 'd'; // atan
            i += 6;
            continue;
        }
        
        // Other functions
        if (strncmp(&expr_copy[i], "x^2(", 4) == 0) {
            operator_stack[operator_stack_top++] = 'q'; // square
            i += 3;
            continue;
        }
        if (strncmp(&expr_copy[i], "e^x(", 4) == 0) {
            operator_stack[operator_stack_top++] = 'e'; // e^x
            i += 3;
            continue;
        }
        if (strncmp(&expr_copy[i], "ln(", 3) == 0) {
            operator_stack[operator_stack_top++] = 'l'; // ln
            i += 2;
            continue;
        }
        if (strncmp(&expr_copy[i], "log(", 4) == 0) {
            operator_stack[operator_stack_top++] = 'g'; // log10
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
                
                // Apply unary function operators
                if (op == 's' || op == 'c' || op == 't' || op == 'a' || 
                    op == 'b' || op == 'd' || op == 'l' || op == 'g' || 
                    op == 'q' || op == 'e') {
                    
                    float arg = value_stack[--value_stack_top];
                    float result = 0.0f;
                    
                    // Apply the appropriate function
                    switch(op) {
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
                                result = 99999.0f; // Overflow
                            }
                            break;
                        }
                        case 'a': // asin
                            result = compute_asin(arg);
                            break;
                        case 'b': // acos
                            result = compute_acos(arg);
                            break;
                        case 'd': // atan
                            result = compute_atan(arg);
                            break;
                        case 'l': // ln
                            result = compute_ln(arg);
                            break;
                        case 'g': // log10
                            result = compute_log10(arg);
                            break;
                        case 'q': // x^2
                            result = arg * arg;
                            break;
                        case 'e': // e^x
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
                                result = 99999.0f; // Overflow
                            }
                            break;
                        case '^': result = compute_power(a, b); break;
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
            // Handle right-associative ^ operator
            while (operator_stack_top > 0 && 
                   operator_stack[operator_stack_top - 1] != '(' &&
                   ((expr_copy[i] != '^' && 
                     get_precedence(operator_stack[operator_stack_top - 1]) >= get_precedence(expr_copy[i])) ||
                    (expr_copy[i] == '^' && 
                     get_precedence(operator_stack[operator_stack_top - 1]) > get_precedence(expr_copy[i])))) {
                
                char op = operator_stack[--operator_stack_top];
                
                // Apply binary operators
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
                            result = 99999.0f; // Overflow
                        }
                        break;
                    case '^': result = compute_power(a, b); break;
                }
                
                value_stack[value_stack_top++] = result;
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
        
        // Apply binary operators
        if (op != '(' && value_stack_top >= 2) {
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
                        result = 99999.0f; // Overflow
                    }
                    break;
                case '^': result = compute_power(a, b); break;
            }
            
            value_stack[value_stack_top++] = result;
        }
    }
    
    // Final result should be on top of value stack
    return (value_stack_top > 0) ? value_stack[0] : 0.0f;
}

// Implementation of mathematical functions using Euler's method
float compute_sin_cos(float x, float* sin_val, float* cos_val) {
    float s = 0.0f;  // sin(0)
    float c = 1.0f;  // cos(0)
    float h = 0.00001f; // smaller step size for better accuracy
    
    // Normalize x to [0, 2π)
    while(x >= 2*PI) x -= 2*PI;
    while(x < 0) x += 2*PI;
    
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

// Implementation of arc sine using numerical methods
float compute_asin(float x) {
    // Check bounds (asin only defined for [-1, 1])
    if (x < -1.0f) x = -1.0f;
    if (x > 1.0f) x = 1.0f;
    
    // Use Newton's method to solve sin(y) = x for y
    float y = x; // Initial guess
    float h = 0.0001f;
    
    for (int i = 0; i < 50; i++) { // Maximum 50 iterations
        float sin_y, cos_y;
        compute_sin_cos(y, &sin_y, &cos_y);
        
        // sin(y) - x should be 0
        float error = sin_y - x;
        if (my_abs(error) < 0.0001f) break;
        
        // Update using Newton's method: y = y - sin(y) - x / cos(y)
        if (!is_zero(cos_y)) {
            y = y - error / cos_y;
        }
    }
    
    return y;
}

// Implementation of arc cosine using asin
float compute_acos(float x) {
    return PI/2 - compute_asin(x);
}

// Implementation of arc tangent using numerical methods
float compute_atan(float x) {
    // Special cases
    if (is_zero(x)) return 0.0f;
    if (x > 0 && my_abs(x - 1.0f) < 0.0001f) return PI/4;
    if (x < 0 && my_abs(x + 1.0f) < 0.0001f) return -PI/4;
    
    // Use Newton's method to solve tan(y) = x for y
    float y = x / (1.0f + my_sqrt(1.0f + x*x)); // Initial guess
    
    for (int i = 0; i < 50; i++) { // Maximum 50 iterations
        float sin_y, cos_y;
        compute_sin_cos(y, &sin_y, &cos_y);
        
        float tan_y = 0.0f;
        if (!is_zero(cos_y)) {
            tan_y = sin_y / cos_y;
        } else {
            continue; // Avoid division by zero
        }
        
        // tan(y) - x should be 0
        float error = tan_y - x;
        if (my_abs(error) < 0.0001f) break;
        
        // Update using Newton's method: y = y - (tan(y) - x) / (1 + tan(y)^2)
        y = y - error / (1.0f + tan_y * tan_y);
    }
    
    return y;
}

// Implementation of natural logarithm using numerical integration
float compute_ln(float x) {
    // Check domain
    if (x <= 0.0f) return -99999.0f; // Error value
    
    // ln(x) = ∫ (1/t) dt from 1 to x
    float result = 0.0f;
    float h = 0.001f; // Step size for integration
    
    if (x >= 1.0f) {
        // Integrate from 1 to x
        for (float t = 1.0f; t < x; t += h) {
            result += h / t;
        }
    } else {
        // For x < 1, use ln(x) = -ln(1/x)
        for (float t = x; t < 1.0f; t += h) {
            result -= h / t;
        }
    }
    
    return result;
}

// Base-10 logarithm using ln
float compute_log10(float x) {
    // log10(x) = ln(x) / ln(10)
    return compute_ln(x) / compute_ln(10.0f);
}

// Exponential function using Euler's method
float compute_exp(float x) {
    float result = 1.0f;  // e^0 = 1
    float h = 0.01f;      // Step size
    
    // Solve ODE: dy/dx = y with y(0) = 1
    if (x >= 0) {
        for (float t = 0.0f; t < x; t += h) {
            result += h * result;
        }
    } else {
        // For negative x, use e^(-x) = 1/e^x
        x = -x;
        for (float t = 0.0f; t < x; t += h) {
            result += h * result;
        }
        result = 1.0f / result;
    }
    
    return result;
}

// Power function a^b using Euler's method
float compute_power(float base, float exponent) {
    // Special cases
    if (is_zero(exponent)) return 1.0f;
    if (is_zero(base)) return 0.0f;
    
    // For integer exponents, use multiplication (more accurate)
    if (is_integer(exponent)) {
        int exp_int = (int)exponent;
        float result = 1.0f;
        
        if (exp_int >= 0) {
            for (int i = 0; i < exp_int; i++) {
                result *= base;
            }
        } else {
            for (int i = 0; i < -exp_int; i++) {
                result *= base;
            }
            result = 1.0f / result;
        }
        
        return result;
    }
    
    // For non-integer exponents, use a^b = e^(b*ln(a))
    return compute_exp(exponent * compute_ln(base));
}

