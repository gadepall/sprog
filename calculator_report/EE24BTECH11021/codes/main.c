#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
// #include <stdlib.h>
// #include <string.h>
// #include <math.h>
#include <ctype.h>
#include <stdio.h>

#define h 0.01
#define PI 3.14159265359
#define E 2.7182818

// RK4 for First-Order Differential Equations
double rk4_1(double x, double y, double x0, double (*function)(double, double)) {
    double k1, k2, k3, k4;
    while (x < x0) {
        k1 = h * function(x, y);
        k2 = h * function(x + h / 2, y + k1 / 2);
        k3 = h * function(x + h / 2, y + k2 / 2);
        k4 = h * function(x + h, y + k3);
        y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        x += h;
    }
    return y;
}

// RK4 for Second-Order Differential Equations
double rk4_2(double x, double y1, double y2, double x0,
             double (*f1)(double, double, double),
             double (*f2)(double, double, double)) {
    double k1_1, k2_1, k3_1, k4_1;
    double k1_2, k2_2, k3_2, k4_2;
    while (x < x0) {
        k1_1 = h * f1(x, y1, y2);
        k1_2 = h * f2(x, y1, y2);

        k2_1 = h * f1(x + h / 2, y1 + k1_1 / 2, y2 + k1_2 / 2);
        k2_2 = h * f2(x + h / 2, y1 + k1_1 / 2, y2 + k1_2 / 2);

        k3_1 = h * f1(x + h / 2, y1 + k2_1 / 2, y2 + k2_2 / 2);
        k3_2 = h * f2(x + h / 2, y1 + k2_1 / 2, y2 + k2_2 / 2);

        k4_1 = h * f1(x + h, y1 + k3_1, y2 + k3_2);
        k4_2 = h * f2(x + h, y1 + k3_1, y2 + k3_2);

        y1 += (k1_1 + 2 * k2_1 + 2 * k3_1 + k4_1) / 6;
        y2 += (k1_2 + 2 * k2_2 + 2 * k3_2 + k4_2) / 6;
        x += h;
    }
    return y1;
}

// #define MAX_EXPR 32  // Maximum input expression size

// // Stack for operators
// char op_stack[MAX_EXPR];
// int op_top = -1;

// // Stack for operands
// double val_stack[MAX_EXPR];
// int val_top = -1;

// // Operator precedence function
// int precedence(char op) {
//     if (op == '+' || op == '-') return 1;
//     if (op == '*' || op == '/') return 2;
//     if (op == '^') return 3;  // Exponentiation
//     return 0;
// }

// // Apply operator on two numbers
// double apply_operator(char op, double a, double b) {
//     switch (op) {
//         case '+': return a + b;
//         case '-': return a - b;
//         case '*': return a * b;
//         case '/': return (b != 0) ? (a / b) : 0;  // Prevent division by zero
//         case '^': return pow(a, b);
//         default: return 0;
//     }
// }

// // Push an operator onto the stack
// void push_op(char op) { op_stack[++op_top] = op; }

// // Pop an operator from the stack
// char pop_op() { return op_stack[op_top--]; }

// // Push a value onto the stack
// void push_val(double val) { val_stack[++val_top] = val; }

// // Pop a value from the stack
// double pop_val() { return val_stack[val_top--]; }

// // Convert infix to postfix and evaluate
// double evaluate_expression(const char *expr) {
//     int i = 0;
//     while (expr[i] != '\0') {
//         if (isdigit(expr[i]) || expr[i] == '.') {
//             char num[10];
//             int j = 0;
//             while (isdigit(expr[i]) || expr[i] == '.') {
//                 num[j++] = expr[i++];
//             }
//             num[j] = '\0';
//             push_val(atof(num));  // Convert number and push to stack
//             i--;
//         } else if (expr[i] == '(') {
//             push_op(expr[i]);
//         } else if (expr[i] == ')') {
//             while (op_top >= 0 && op_stack[op_top] != '(') {
//                 double b = pop_val();
//                 double a = pop_val();
//                 push_val(apply_operator(pop_op(), a, b));
//             }
//             pop_op();  // Pop '('
//         } else if (strchr("+-*/^", expr[i])) {
//             while (op_top >= 0 && precedence(op_stack[op_top]) >= precedence(expr[i])) {
//                 double b = pop_val();
//                 double a = pop_val();
//                 push_val(apply_operator(pop_op(), a, b));
//             }
//             push_op(expr[i]);
//         }
//         i++;
//     }

//     // Process remaining operators
//     while (op_top >= 0) {
//         double b = pop_val();
//         double a = pop_val();
//         push_val(apply_operator(pop_op(), a, b));
//     }

//     return pop_val();
// }

// // Function to handle BODMAS operations and display result on LCD
// void handle_bodmas(const char *expr) {
//     char result_str[16];  // To hold the result as a string
//     double result = evaluate_expression(expr);  // Evaluate the expression

//     // Format the result into a string (limited to 2 decimal places)
//     snprintf(result_str, sizeof(result_str), "%.2f", result);

//     // // Display the result on LCD
//     // lcd_clear();
//     // lcd_gotoxy(0, 0);
//     // lcd_print("Result: ");
//     // lcd_print(result_str);
// }

// Function Definitions
double function_exp(double x, double y) { return y; }
double function_ln(double x, double y) { return 1 / x; }
double function_log10(double x, double y) { return 1 / (2.3025 * x); }
double function_sqrt(double x, double y) { return 1 / (2 * y); }
double function_arctan(double x, double y) { return 1 / (1 + x * x); }

// Corrected arcsin function
double function_arcsin(double x, double y) {
    return 1.0 / sqrt(1 - x * x);  // Instead of calling rk4_1 inside itself
}

// Functions for sin & cos using second-order ODE
double f1_sin_cos(double x, double y1, double y2) { return y2; }
double f2_sin_cos(double x, double y1, double y2) { return -y1; }

double compute_scientific_functions(double value, int mode) {
    if (mode == 1) return rk4_1(0, 1, value, function_exp);  // e^x
    if (mode == 2)  // sin(x)
    {
        if (value == 0) return 0;
        return (value > 0) ? rk4_2(0, 0, 1, value, f1_sin_cos, f2_sin_cos)
                           : -rk4_2(0, 0, 1, -value, f1_sin_cos, f2_sin_cos);
    }
    if (mode == 3)  // cos(x)
    {
        if (value == 0) return 1;
        return rk4_2(0, 1, 0, fabs(value), f1_sin_cos, f2_sin_cos);
    }
    if (mode == 4)  // tan(x)
    {
        double cosval = compute_scientific_functions(value, 3);
        if (cosval == 0) {
            printf("Infinity");
            return 0;
        }
        return compute_scientific_functions(value, 2) / cosval;
    }
    if (mode == 5)  // ln(x)
    {
        if (value <= 0) {
            printf("Math Error.");
            return 0;
        }
        return rk4_1(1, 0, value, function_ln);
    }
    if (mode == 6)  // log10(x)
    {
        if (value <= 0) {
            printf("Math Error.");
            return 0;
        }
        return rk4_1(1, 0, value, function_log10);
    }
    if (mode == 7)  // sqrt(x)
    {
        if (value < 0) {
            printf("Math Error.");
            return 0;
        }
        return (value == 0) ? 0 : rk4_1(1, 1, value, function_sqrt);
    }
    if (mode == 8)  // arcsin(x)
    {
        if (value < -1 || value > 1) {
            printf("Math Error:");
            return 0;
        }
        return (value == 1) ? PI / 2
                            : (value == -1) ? -PI / 2
                                            : rk4_1(0, 0, value, function_arcsin);
    }
    if (mode == 9)  // arccos(x)
    {
        if (value < -1 || value > 1) {
            printf("Math Error:");
            return 0;
        }
        return PI / 2 - compute_scientific_functions(value, 8);
    }
    if (mode == 10)  // arctan(x)
    {
        return rk4_1(0, 0, value, function_arctan);
    }

    return 0;  // Invalid mode
}


// Define LCD pins
#define RS 2      // Register Select pin
#define EN 3      // Enable pin
#define D4 4       // Data pin 4
#define D5 5       // Data pin 5
#define D6 6       // Data pin 6
#define D7 12       // Data pin 7

// Define keypad pins
#define ROW1 8  
#define ROW2 9    
#define ROW3 10     
#define ROW4 11     

#define COL1 A5     // D8
#define COL2 A4    // D9
#define COL3 A3    // D10
#define COL4 A2    // D6
#define COL5 A1     // D7

// Define macros for pin manipulation
#define SetBit(PORT, BIT) ((PORT) |= (1 << (BIT)))
#define ClearBit(PORT, BIT) ((PORT) &= ~(1 << (BIT)))
#define ReadBit(PORT, BIT) ((PORT) & (1 << (BIT)))

// Convert Arduino pin numbers to AVR port and pin
#define RS_PORT PORTD
#define RS_PIN 2    // Arduino pin 2 is PD2
#define EN_PORT PORTD
#define EN_PIN 3    // Arduino pin 3 is PD3
#define D4_PORT PORTD
#define D4_PIN 4    // Arduino pin 4 is PD4
#define D5_PORT PORTD
#define D5_PIN 5    // Arduino pin 5 is PD5
#define D6_PORT PORTD
#define D6_PIN 6    // Arduino pin 6 is PD6
#define D7_PORT PORTB
#define D7_PIN 4    // Arduino pin 12 is PB4


// Define direction registers
#define RS_DDR DDRD
#define EN_DDR DDRD
#define D4_DDR DDRD
#define D5_DDR DDRD
#define D6_DDR DDRD
#define D7_DDR DDRB

// Keypad row port and pin definitions
#define ROW1_PORT PORTB
#define ROW1_PIN 0     // Arduino pin 8 is PB0
#define ROW2_PORT PORTB
#define ROW2_PIN 1     // Arduino pin 9 is PB1
#define ROW3_PORT PORTB
#define ROW3_PIN 2     // Arduino pin 10 is PB2
#define ROW4_PORT PORTB
#define ROW4_PIN 3     // Arduino pin 11 is PB3

// Keypad row direction registers
#define ROW1_DDR DDRB
#define ROW2_DDR DDRB
#define ROW3_DDR DDRB
#define ROW4_DDR DDRB


// Keypad column port and pin definitions
#define COL1_PORT PORTC
#define COL1_PIN 5     // A5 is PC5
#define COL2_PORT PORTC
#define COL2_PIN 4     // A4 is PC4
#define COL3_PORT PORTC
#define COL3_PIN 3     // A3 is PC3
#define COL4_PORT PORTC
#define COL4_PIN 2     // A2 is PC2
#define COL5_PORT PORTC
#define COL5_PIN 1     // A1 is PC1


// Keypad column direction registers
#define COL1_DDR DDRC
#define COL2_DDR DDRC
#define COL3_DDR DDRC
#define COL4_DDR DDRC
#define COL5_DDR DDRC


// Keypad column input registers
#define COL1_PIN_REG PINC
#define COL2_PIN_REG PINC
#define COL3_PIN_REG PINC
#define COL4_PIN_REG PINC
#define COL5_PIN_REG PINC


// LCD commands
#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_ENTRY_MODE 0x06
#define LCD_DISPLAY_ON 0x0C
#define LCD_FUNCTION_4BIT_2LINE 0x28
#define LCD_SET_CURSOR 0x80

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>

// LCD Commands
#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_ENTRY_MODE 0x06
#define LCD_DISPLAY_ON 0x0C
#define LCD_FUNCTION_4BIT_2LINE 0x28
#define LCD_SET_CURSOR 0x80

// Define LCD control pins
#define RS PB2
#define EN PB3
#define D4 PB4
#define D5 PB5
#define D6 PB6
#define D7 PB7

#define MAX_EXPR 32

char op_stack[MAX_EXPR];
int op_top = -1;
double val_stack[MAX_EXPR];
int val_top = -1;

void lcd_send_command(uint8_t cmd) {
    PORTB &= ~(1 << RS);
    PORTB &= ~(1 << EN);
    PORTB = (PORTB & 0x0F) | (cmd & 0xF0);
    PORTB |= (1 << EN);
    _delay_ms(1);
    PORTB &= ~(1 << EN);
    PORTB = (PORTB & 0x0F) | ((cmd << 4) & 0xF0);
    PORTB |= (1 << EN);
    _delay_ms(1);
    PORTB &= ~(1 << EN);
    _delay_ms(2);
}

void lcd_send_data(uint8_t data) {
    PORTB |= (1 << RS);
    PORTB &= ~(1 << EN);
    PORTB = (PORTB & 0x0F) | (data & 0xF0);
    PORTB |= (1 << EN);
    _delay_ms(1);
    PORTB &= ~(1 << EN);
    PORTB = (PORTB & 0x0F) | ((data << 4) & 0xF0);
    PORTB |= (1 << EN);
    _delay_ms(1);
    PORTB &= ~(1 << EN);
    _delay_ms(2);
}

// void lcd_init() {
//     DDRB |= (1 << RS) | (1 << EN) | (1 << D4) | (1 << D5) | (1 << D6) | (1 << D7);
//     _delay_ms(50);
//     PORTB &= ~(1 << RS);
//     PORTB &= ~(1 << EN);
//     for (int i = 0; i < 3; i++) {
//         PORTB = (PORTB & 0x0F) | 0x30;
//         PORTB |= (1 << EN);
//         _delay_ms(1);
//         PORTB &= ~(1 << EN);
//         _delay_ms(5);
//     }
//     PORTB = (PORTB & 0x0F) | 0x20;
//     PORTB |= (1 << EN);
//     _delay_ms(1);
//     PORTB &= ~(1 << EN);
//     _delay_ms(2);
//     lcd_send_command(LCD_FUNCTION_4BIT_2LINE);
//     lcd_send_command(LCD_DISPLAY_ON);
//     lcd_send_command(LCD_CLEAR);
//     _delay_ms(2);
//     lcd_send_command(LCD_ENTRY_MODE);
// }

void lcd_print(char *str) {
    while (*str) {
        lcd_send_data(*str++);
    }
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t position = row == 0 ? LCD_SET_CURSOR + col : LCD_SET_CURSOR + 0x40 + col;
    lcd_send_command(position);
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

double apply_operator(char op, double a, double b) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b != 0) ? (a / b) : 0;
        case '^': return pow(a, b);
        default: return 0;
    }
}

void push_op(char op) { 
    if (op_top < MAX_EXPR - 1) op_stack[++op_top] = op; 
}

char pop_op() { 
    return (op_top >= 0) ? op_stack[op_top--] : '\0';
}

void push_val(double val) { 
    if (val_top < MAX_EXPR - 1) val_stack[++val_top] = val; 
}

double pop_val() { 
    return (val_top >= 0) ? val_stack[val_top--] : 0.0;
}

int is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

double evaluate_expression(const char *expr) {
    op_top = -1;
    val_top = -1;
    int i = 0;
    while (expr[i] != '\0') {
        if (expr[i] == ' ') {
            i++;
            continue;
        }
        if (isdigit(expr[i]) || (expr[i] == '-' && (i == 0 || expr[i-1] == '(' || is_operator(expr[i-1])))) {
            double sign = 1.0;
            if (expr[i] == '-') {
                sign = -1.0;
                i++;
            }
            char num[10];
            int j = 0;
            while (isdigit(expr[i]) || expr[i] == '.') {
                num[j++] = expr[i++];
                if (j >= 9) break;
            }
            num[j] = '\0';
            push_val(sign * atof(num));
            i--;
        } 
        else if (expr[i] == '(') {
            push_op(expr[i]);
        } 
        else if (expr[i] == ')') {
            while (op_top >= 0 && op_stack[op_top] != '(') {
                char op = pop_op();
                double b = pop_val();
                double a = pop_val();
                push_val(apply_operator(op, a, b));
            }
            if (op_top >= 0 && op_stack[op_top] == '(') {
                pop_op();
            }
        } 
        else if (is_operator(expr[i])) {
            while (op_top >= 0 && op_stack[op_top] != '(' && 
                   precedence(op_stack[op_top]) >= precedence(expr[i])) {
                char op = pop_op();
                double b = pop_val();
                double a = pop_val();
                push_val(apply_operator(op, a, b));
            }
            push_op(expr[i]);
        }
        i++;
    }
    while (op_top >= 0) {
        char op = pop_op();
        double b = pop_val();
        double a = pop_val();
        push_val(apply_operator(op, a, b));
    }
    return (val_top >= 0) ? val_stack[val_top] : 0.0;
}

double handle_bodmas(const char *expr) {
    return evaluate_expression(expr);
}

void display_result_on_lcd(double result) {
    char result_str[16];
    if (result == (int)result) {
        sprintf(result_str, "Result: %d", (int)result);
    } else {
        sprintf(result_str, "Result: %.2f", result);
    }
    lcd_send_command(LCD_CLEAR);
    _delay_ms(2);
    lcd_set_cursor(0, 0);
    lcd_print("BODMAS Result");
    lcd_set_cursor(1, 0);
    lcd_print(result_str);
}



const char keypad_normal[4][5] PROGMEM = {
    {'7', '8', '9', '+', 'D'},
    {'4', '5', '6', '-', 'S'},
    {'1', '2', '3', '*', 'A'},
    {'0', '.', '=', '/', 'C'}
};

const char* const keypad_alpha[4][5] PROGMEM  = {
    {"EXP", "cos", "tan", "^", "BS"},
    {"log", "ln", "e^", "√", "("},
    {"π", '*', '/', "1/x", ")"},
    {"sin", "ANS", "cos", "M-", "MR"}
};

const char* const shiftFunctions[4][5] PROGMEM = {
    {"asin", "acos", "atan", "y^x", "CLR"},
    {"10^", "e", "abs", "cbrt", "["},
    {"deg", "rad", "mod", "fact", "]"},
    {'7', '8', "asin", "acos", "MC"}
};


// Global variables
int shift_mode = 0;
int alpha_mode = 0;
double memory = 0.0;
double last_result = 0.0;
char buffer[32];  // Increased buffer size for string operations

// Function prototypes
void lcd_init(void);
void lcd_command(unsigned char cmd);
void lcd_char(unsigned char data);
void lcd_string(const char *str);
void lcd_string_P(const char *str);
void lcd_clear(void);
void keypad_init(void);
char keypad_scan(void);
void evaluate_expression(char *expression);
double perform_operation(double a, double b, char op);
double parse_number(char **expr);
void handle_shift_mode(char *key, char *expression, int *expr_index);
void handle_alpha_mode(char *key, char *expression, int *expr_index);
void handle_special_functions(char *key, char *expression, int *expr_index);
double factorial(int n);
void write_string_to_buffer(const char *src, char *dest, int *index);

#define MAX_EXPRESSION_LENGTH 64  // Increased for complex expressions

int main(void) {
    char expression[MAX_EXPRESSION_LENGTH] = "";
    int expr_index = 0;

    lcd_init();
    keypad_init();

    lcd_string_P(PSTR("Welcome !"));
    _delay_ms(2000);
    lcd_clear();

    lcd_string_P(PSTR("Enter expression:"));
    lcd_command(LCD_SET_CURSOR | 0x40);  // Move to second line

    while(1) {
        char key = keypad_scan();

        if (key != 0) {
            if (key == 'S') {
                shift_mode = !shift_mode;
                lcd_clear();
                lcd_string_P(shift_mode ? PSTR("Shift mode ON") : PSTR("Shift mode OFF"));
                _delay_ms(1000);
                lcd_clear();
                lcd_string_P(PSTR("Enter expression:"));
                lcd_command(LCD_SET_CURSOR | 0x40);
                lcd_string(expression);
            } else if (key == 'A') {
                alpha_mode = !alpha_mode;
                lcd_clear();
                lcd_string_P(alpha_mode ? PSTR("Alpha mode ON") : PSTR("Alpha mode OFF"));
                _delay_ms(1000);
                lcd_clear();
                lcd_string_P(PSTR("Enter expression:"));
                lcd_command(LCD_SET_CURSOR | 0x40);
                lcd_string(expression);
            } else if (key == 'C') {
                lcd_clear();
                lcd_string_P(PSTR("Enter expression:"));
                lcd_command(LCD_SET_CURSOR | 0x40);
                expr_index = 0;
                expression[expr_index] = '\0';
            } else if (key == 'D' && expr_index > 0) {
                expr_index--;
                expression[expr_index] = '\0';
                lcd_command(LCD_SET_CURSOR | 0x40);
                lcd_string_P(PSTR("                "));  // Clear second line
                lcd_command(LCD_SET_CURSOR | 0x40);
                lcd_string(expression);
            } else if (key == '=') {
                expression[expr_index] = '\0';
                lcd_clear();
                lcd_string_P(PSTR("Result:"));
                lcd_command(LCD_SET_CURSOR | 0x40);
                evaluate_expression(expression);
                expr_index = 0;
                expression[expr_index] = '\0';
                _delay_ms(3000);
                lcd_clear();
                lcd_string_P(PSTR("Enter expression:"));
                lcd_command(LCD_SET_CURSOR | 0x40);
            } else if (expr_index < MAX_EXPRESSION_LENGTH - 1) {
                if (shift_mode) {
                    handle_shift_mode(&key, expression, &expr_index);
                } else if (alpha_mode) {
                    handle_alpha_mode(&key, expression, &expr_index);
                } else {
                    handle_special_functions(&key, expression, &expr_index);
                }
                
                if (key != 0) {  // Only add the key if it wasn't handled by a special function
                    expression[expr_index++] = key;
                    expression[expr_index] = '\0';
                    lcd_char(key);
                }
            }
            _delay_ms(300);  // Debounce
        }
    }

    return 0;
}

// Function to write a string to buffer and update index
void write_string_to_buffer(const char *src, char *dest, int *index) {
    int i = 0;
    while (src[i] != '\0' && (*index + i) < MAX_EXPRESSION_LENGTH - 1) {
        dest[*index + i] = src[i];
        i++;
    }
    *index += i;
    dest[*index] = '\0';
}

char keypad_scan(void) {
    // Variable to store the pressed key
    char pressed_key = 0;
    
    // Iterate through each row
    for (int r = 0; r < 4; r++) {
        // Set the current row to LOW, keep others HIGH
        if (r == 0) ClearBit(ROW1_PORT, ROW1_PIN); else SetBit(ROW1_PORT, ROW1_PIN);
        if (r == 1) ClearBit(ROW2_PORT, ROW2_PIN); else SetBit(ROW2_PORT, ROW2_PIN);
        if (r == 2) ClearBit(ROW3_PORT, ROW3_PIN); else SetBit(ROW3_PORT, ROW3_PIN);
        if (r == 3) ClearBit(ROW4_PORT, ROW4_PIN); else SetBit(ROW4_PORT, ROW4_PIN);
        
        // Small delay to allow signals to stabilize
        _delay_us(10);
        
        // Check each column for the current row
        if (!(COL1_PIN_REG & (1 << COL1_PIN))) {
            pressed_key = pgm_read_byte(&keypad_normal[r][0]);
        }
        if (!(COL2_PIN_REG & (1 << COL2_PIN))) {
            pressed_key = pgm_read_byte(&keypad_normal[r][1]);
        }
        if (!(COL3_PIN_REG & (1 << COL3_PIN))) {
            pressed_key = pgm_read_byte(&keypad_normal[r][2]);
        }
        if (!(COL4_PIN_REG & (1 << COL4_PIN))) {
            pressed_key = pgm_read_byte(&keypad_normal[r][3]);
        }
        if (!(COL5_PIN_REG & (1 << COL5_PIN))) {
            pressed_key = pgm_read_byte(&keypad_normal[r][4]);
        }
        
        // If a key was pressed, restore row states and return the key
        if (pressed_key != 0) {
            // Restore all rows to HIGH
            SetBit(ROW1_PORT, ROW1_PIN);
            SetBit(ROW2_PORT, ROW2_PIN);
            SetBit(ROW3_PORT, ROW3_PIN);
            SetBit(ROW4_PORT, ROW4_PIN);
            
            return pressed_key;
        }
    }
    
    // No key was pressed, restore row states
    SetBit(ROW1_PORT, ROW1_PIN);
    SetBit(ROW2_PORT, ROW2_PIN);
    SetBit(ROW3_PORT, ROW3_PIN);
    SetBit(ROW4_PORT, ROW4_PIN);
    
    return 0;
}

void handle_shift_mode(char *key, char *expression, int *expr_index) {
    // First, find which key was pressed in the normal keypad
    int row = -1, col = -1;
    
    for (int r = 0; r < 4 && row == -1; r++) {
        for (int c = 0; c < 5; c++) {
            if (pgm_read_byte(&keypad_normal[r][c]) == *key) {
                row = r;
                col = c;
                break;
            }
        }
    }
    
    // If found, handle the shift function
    if (row != -1 && col != -1) {
        const char* function = (const char*)pgm_read_word(&shiftFunctions[row][col]);
        
        // Handle the function based on its name
        if (strcmp_P(function, PSTR("asin")) == 0) {
            strcpy_P(buffer, PSTR("asin("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("acos")) == 0) {
            strcpy_P(buffer, PSTR("acos("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("atan")) == 0) {
            strcpy_P(buffer, PSTR("atan("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("y^x")) == 0) {
            *key = '^';
        } else if (strcmp_P(function, PSTR("CLR")) == 0) {
            lcd_clear();
            lcd_string_P(PSTR("Enter expression:"));
            lcd_command(LCD_SET_CURSOR | 0x40);
            *expr_index = 0;
            expression[*expr_index] = '\0';
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("10^")) == 0) {
            strcpy_P(buffer, PSTR("10^"));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("e")) == 0) {
            strcpy_P(buffer, PSTR("2.7182818"));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("abs")) == 0) {
            strcpy_P(buffer, PSTR("abs("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("cbrt")) == 0) {
            strcpy_P(buffer, PSTR("cbrt("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("[")) == 0) {
            *key = '[';
        } else if (strcmp_P(function, PSTR("deg")) == 0) {
            strcpy_P(buffer, PSTR("deg("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("rad")) == 0) {
            strcpy_P(buffer, PSTR("rad("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("mod")) == 0) {
            *key = '%';
        } else if (strcmp_P(function, PSTR("fact")) == 0) {
            strcpy_P(buffer, PSTR("fact("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("]")) == 0) {
            *key = ']';
        } else if (strcmp_P(function, PSTR("HEX")) == 0 || 
                  strcmp_P(function, PSTR("DEC")) == 0 || 
                  strcmp_P(function, PSTR("BIN")) == 0 || 
                  strcmp_P(function, PSTR("OCT")) == 0) {
            // These would change the display mode, not implemented in this example
            *key = 0; // Don't add anything to expression
        } else if (strcmp_P(function, PSTR("MC")) == 0) {
            // Memory clear function
            memory = 0.0;
            lcd_clear();
            lcd_string_P(PSTR("Memory cleared"));
            _delay_ms(1000);
            lcd_clear();
            lcd_string_P(PSTR("Enter expression:"));
            lcd_command(LCD_SET_CURSOR | 0x40);
            lcd_string(expression);
            *key = 0; // Don't add anything to expression
        }
    }
    
    // Turn off shift mode after handling a key
    shift_mode = 0;
}

void handle_alpha_mode(char *key, char *expression, int *expr_index) {
    // First, find which key was pressed in the normal keypad
    int row = -1, col = -1;
    
    for (int r = 0; r < 4 && row == -1; r++) {
        for (int c = 0; c < 5; c++) {
            if (pgm_read_byte(&keypad_normal[r][c]) == *key) {
                row = r;
                col = c;
                break;
            }
        }
    }
    
    // If found, handle the alpha function
    if (row != -1 && col != -1) {
        const char* function = (const char*)pgm_read_word(&keypad_alpha[row][col]);
        
        // Handle the function based on its name
        if (strcmp_P(function, PSTR("sin")) == 0) {
            strcpy_P(buffer, PSTR("sin("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("cos")) == 0) {
            strcpy_P(buffer, PSTR("cos("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("tan")) == 0) {
            strcpy_P(buffer, PSTR("tan("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("^")) == 0) {
            *key = '^';
        } else if (strcmp_P(function, PSTR("BS")) == 0) {
            // Backspace functionality - handled in main
            *key = 'D'; // Use the existing delete key behavior
        } else if (strcmp_P(function, PSTR("log")) == 0) {
            strcpy_P(buffer, PSTR("log("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("ln")) == 0) {
            strcpy_P(buffer, PSTR("ln("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("e^")) == 0) {
            strcpy_P(buffer, PSTR("e^("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("√")) == 0) {
            strcpy_P(buffer, PSTR("sqrt("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("(")) == 0) {
            *key = '(';
        } else if (strcmp_P(function, PSTR("π")) == 0) {
            strcpy_P(buffer, PSTR("3.14159"));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("x²")) == 0) {
            strcpy_P(buffer, PSTR("^2"));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("x³")) == 0) {
            strcpy_P(buffer, PSTR("^3"));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("1/x")) == 0) {
            strcpy_P(buffer, PSTR("1/("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR(")")) == 0) {
            *key = ')';
        } else if (strcmp_P(function, PSTR("EXP")) == 0) {
            *key = 'E';
        } else if (strcmp_P(function, PSTR("ANS")) == 0) {
            // Add last result to expression
            dtostrf(last_result, 8, 2, buffer);
            // Trim trailing zeros and decimal point if whole number
            char *p = buffer + strlen(buffer) - 1;
            while (*p == '0' && p > buffer) {
                *p-- = '\0';
            }
            if (*p == '.') *p = '\0';
            
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("M+")) == 0) {
            // Add expression to memory
            // Evaluate current expression and add to memory
            char temp_expr[MAX_EXPRESSION_LENGTH];
            strcpy(temp_expr, expression);
            float result;
            // Simplified evaluation for memory operations
            result = atof(temp_expr);
            memory += result;
            
            lcd_clear();
            lcd_string_P(PSTR("Added to memory"));
            _delay_ms(1000);
            lcd_clear();
            lcd_string_P(PSTR("Enter expression:"));
            lcd_command(LCD_SET_CURSOR | 0x40);
            lcd_string(expression);
            *key = 0; // Don't add anything to expression
        } else if (strcmp_P(function, PSTR("M-")) == 0) {
            // Subtract expression from memory
            // Evaluate current expression and subtract from memory
            char temp_expr[MAX_EXPRESSION_LENGTH];
            strcpy(temp_expr, expression);
            float result;
            // Simplified evaluation for memory operations
            result = atof(temp_expr);
            memory -= result;
            
            lcd_clear();
            lcd_string_P(PSTR("Subtracted from mem"));
            _delay_ms(1000);
            lcd_clear();
            lcd_string_P(PSTR("Enter expression:"));
            lcd_command(LCD_SET_CURSOR | 0x40);
            lcd_string(expression);
            *key = 0; // Don't add anything to expression
        } else if (strcmp_P(function, PSTR("MR")) == 0) {
            // Recall memory value
            dtostrf(memory, 8, 2, buffer);
            // Trim trailing zeros and decimal point if whole number
            char *p = buffer + strlen(buffer) - 1;
            while (*p == '0' && p > buffer) {
                *p-- = '\0';
            }
            if (*p == '.') *p = '\0';
            
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
        }
    }
    
    // Turn off alpha mode after handling a key
    alpha_mode = 0;
}

void handle_special_functions(char *key, char *expression, int *expr_index) {
    // Handle some basic special functions for normal mode
    switch (*key) {
        case 's':
            strcpy_P(buffer, PSTR("sin("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
            break;
        case 'c':
            strcpy_P(buffer, PSTR("cos("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
            break;
        case 't':
            strcpy_P(buffer, PSTR("tan("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
            break;
        case 'p':
            strcpy_P(buffer, PSTR("3.14159"));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
            break;
        case 'e':
            strcpy_P(buffer, PSTR("2.71828"));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
            break;
        case 'l':
            strcpy_P(buffer, PSTR("log("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
            break;
        case 'r':
            strcpy_P(buffer, PSTR("sqrt("));
            write_string_to_buffer(buffer, expression, expr_index);
            lcd_string(buffer);
            *key = 0; // Don't add the key to expression
            break;
    }
}
// Function to calculate factorial
double factorial(int n) {
    if (n < 0) return NAN;  // Factorial undefined for negative numbers
    if (n <= 1) return 1;
    double result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// Function to calculate power (x^y)
double power(float base, int exp) {
    double result = 1.0;

    // If the exponent is negative, take the reciprocal of the base
    if (exp < 0) {
        base = 1.0 / base;
        exp = -exp;
    }

    // Multiply the base 'exp' times
    for (int i = 0; i < exp; i++) {
        result *= base;
    }

    return result;
}

// Improved evaluate_expression function with better function handling
void evaluate_expression(char *expression) {
    // Implementation of BODMAS rule with added functions
    char *expr = expression;
    double result = 0;
    
    // Check for special cases first
    if (strncmp(expr, "sin(", 4) == 0) {
        expr += 4;  // Skip "sin("
        double angle = atof(expr);
        // Find closing parenthesis
        while (*expr && *expr != ')') expr++;
        if (*expr == ')') expr++;
        
        // result = sin(angle * M_PI / 180.0);  // Convert to radians
        result = compute_scientific_functions(angle*PI/180.0, 2) ;
    } else if (strncmp(expr, "cos(", 4) == 0) {
        expr += 4;  // Skip "cos("
        double angle = atof(expr);
        // Find closing parenthesis
        while (*expr && *expr != ')') expr++;
        if (*expr == ')') expr++;
        
        // result = cos(angle * M_PI / 180.0);  // Convert to radians
        result = compute_scientific_functions(angle*PI/180.0, 3) ;
    } else if (strncmp(expr, "tan(", 4) == 0) {
        expr += 4;  // Skip "tan("
        double angle = atof(expr);
        // Find closing parenthesis
        while (*expr && *expr != ')') expr++;
        if (*expr == ')') expr++;
        
        // result = tan(angle * M_PI / 180.0);  // Convert to radians
        result = compute_scientific_functions(angle*PI/180.0, 4) ;
    } else if (strncmp(expr, "log(", 4) == 0) {
        expr += 4;  // Skip "log("
        double value = atof(expr);
        // Find closing parenthesis
        while (*expr && *expr != ')') expr++;
        if (*expr == ')') expr++;
        
        // result = log10(value);
        result = compute_scientific_functions(value, 6) ;
    } else if (strncmp(expr, "ln(", 3) == 0) {
        expr += 3;  // Skip "ln("
        double value = atof(expr);
        // Find closing parenthesis
        while (*expr && *expr != ')') expr++;
        if (*expr == ')') expr++;
        
        // result = log(value);
        result = compute_scientific_functions(value, 5) ;
    } else if (strncmp(expr, "sqrt(", 5) == 0) {
        expr += 5;  // Skip "sqrt("
        double value = atof(expr);
        // Find closing parenthesis
        while (*expr && *expr != ')') expr++;
        if (*expr == ')') expr++;
        
        // result = sqrt(value);
        result = compute_scientific_functions(value, 7) ;
    } else if (strncmp(expr, "asin(", 5) == 0) {
        expr += 5;  // Skip "asin("
        double value = atof(expr);
        // Find closing parenthesis
        while (*expr && *expr != ')') expr++;
        if (*expr == ')') expr++;
        
        // result = asin(value) * 180.0 / M_PI;  // Convert to degrees
        result = compute_scientific_functions(value, 8)*180/PI ;
    } else if (strncmp(expr, "acos(", 5) == 0) {
        expr += 5;  // Skip "acos("
        double value = atof(expr);
        // Find closing parenthesis
        while (*expr && *expr != ')') expr++;
        if (*expr == ')') expr++;
        
        // result = acos(value) * 180.0 / M_PI;  // Convert to degrees
        result = compute_scientific_functions(value, 9)*180/PI ;
    } else if (strncmp(expr, "atan(", 5) == 0) {
        expr += 5;  // Skip "atan("
        double value = atof(expr);
        // Find closing parenthesis
        while (*expr && *expr != ')') expr++;
        if (*expr == ')') expr++;
        
        // result = atan(value) * 180.0 / M_PI;  // Convert to degrees
        result = compute_scientific_functions(value, 10)*180/PI ;
    } else if (strncmp(expr, "e^(", 3) == 0) {
        expr += 3;  // Skip "e^("
        double value = atof(expr);
        // Find closing parenthesis
        while (*expr && *expr != ')') expr++;
        if (*expr == ')') expr++;
        
        result = power(E, value);
        // result = compute_scientific_functions(value, 1) ;
    } else if (strncmp(expr, "10^", 3) == 0) {
        expr += 3;  // Skip "10^"
        double value = atof(expr);
        result = power(10.0, value);
    } else if (strncmp(expr, "abs(", 4) == 0) {
        expr += 4;  // Skip "abs("
        double value = atof(expr);
        // Find closing parenthesis
        while (*expr && *expr != ')') expr++;
        if (*expr == ')') expr++;
        
        result = fabs(value);
    } else if (strncmp(expr, "cbrt(", 5) == 0) {
        expr += 5;  // Skip "cbrt("
        double value = atof(expr);
        // Find closing parenthesis
        while (*expr && *expr != ')') expr++;
        if (*expr == ')') expr++;
        
        result = cbrt(value);
    } else if (strncmp(expr, "fact(", 5) == 0) {
        expr += 5;  // Skip "fact("
        int value = (int)atof(expr);
        // Find closing parenthesis
        while (*expr && *expr != ')') expr++;
        if (*expr == ')') expr++;
        
        result = factorial(value);
    } else {
        // For basic arithmetic expressions
        char op = '+'; // Start with addition
        double current_value = 0;
        
        while (*expr) {
            // Skip whitespace
            while (*expr && isspace(*expr)) expr++;
            
            // Parse number
            if (isdigit(*expr) || *expr == '.') {
                current_value = atof(expr);
                // Move to end of number
                while (*expr && (isdigit(*expr) || *expr == '.')) expr++;
            }
            
            // Perform operation
            if (*expr == '+' || *expr == '-' || *expr == '*' || *expr == '/' || *expr == '^' || *expr == '%' || *expr == '\0') {
                switch (op) {
                    case '+':
                        result += current_value;
                        break;
                    case '-':
                        result -= current_value;
                        break;
                    case '*':
                        result *= current_value;
                        break;
                    case '/':
                        if (current_value != 0) {
                            result /= current_value;
                        } else {
                            lcd_string_P(PSTR("Div by zero error"));
                            return;
                        }
                        break;
                    case '^':
                        // result = pow(result, current_value);
                        result = power(result, current_value) ;
                        break;
                    case '%':
                        if (current_value != 0) {
                            result = (int)result % (int) current_value ; }
                        else {
                            lcd_string_P(PSTR("Mod by zero error"));
                            return;
                        }
                        break;
                }
                
                if (*expr) {
                    op = *expr;
                    expr++;
                }
            } else {
                // Skip any unrecognized character
                expr++;
            }
        }
    }
    
    // Store as last result
    last_result = result;
    
    // Format the result for display
    if (isnan(result)) {
        lcd_string_P(PSTR("Error"));
    } else if (isinf(result)) {
        lcd_string_P(PSTR("Infinity"));
    } else {
        dtostrf(result, 8, 4, buffer);
        
        // Trim trailing zeros and decimal point if whole number
        char *p = buffer + strlen(buffer) - 1;
        while (*p == '0' && p > buffer) {
            *p-- = '\0';
        }
        if (*p == '.') *p = '\0';
        
        lcd_string(buffer);
    }
}

// Function to initialize the LCD
void lcd_init(void) {
    // Set data pins as output
    SetBit(RS_DDR, RS_PIN);
    SetBit(EN_DDR, EN_PIN);
    SetBit(D4_DDR, D4_PIN);
    SetBit(D5_DDR, D5_PIN);
    SetBit(D6_DDR, D6_PIN);
    SetBit(D7_DDR, D7_PIN);
    
    // Wait for LCD to initialize
    _delay_ms(50);
    
    // 4-bit initialization sequence
    // Set to 4-bit mode
    ClearBit(RS_PORT, RS_PIN);  // RS = 0 for command
    
    // Function set - 8-bit mode first (3 times)
    ClearBit(D4_PORT, D4_PIN);
    ClearBit(D5_PORT, D5_PIN);
    SetBit(D6_PORT, D6_PIN);
    SetBit(D7_PORT, D7_PIN);
    
    // Pulse enable
    SetBit(EN_PORT, EN_PIN);
    _delay_us(1);
    ClearBit(EN_PORT, EN_PIN);
    _delay_ms(5);
    
    // Repeat again
    SetBit(EN_PORT, EN_PIN);
    _delay_us(1);
    ClearBit(EN_PORT, EN_PIN);
    _delay_ms(1);
    
    // Repeat third time
    SetBit(EN_PORT, EN_PIN);
    _delay_us(1);
    ClearBit(EN_PORT, EN_PIN);
    _delay_ms(1);
    
    // Now set to 4-bit mode
    ClearBit(D7_PORT, D7_PIN);
    SetBit(EN_PORT, EN_PIN);
    _delay_us(1);
    ClearBit(EN_PORT, EN_PIN);
    _delay_ms(1);
    
    // Now we can use lcd_command function
    // Set 4-bit interface, 2 line display, 5x8 font
    lcd_command(LCD_FUNCTION_4BIT_2LINE);
    // Display on, cursor on, blinking off
    lcd_command(LCD_DISPLAY_ON);
    // Entry mode set: increment cursor, no display shift
    lcd_command(LCD_ENTRY_MODE);
    // Clear display
    lcd_command(LCD_CLEAR);
    // Return home
    lcd_command(LCD_HOME);
    
    _delay_ms(2);
}

// Function to send a command to the LCD
void lcd_command(unsigned char cmd) {
    // RS = 0 for command
    ClearBit(RS_PORT, RS_PIN);
    
    // Send high nibble
    (cmd & 0x80) ? SetBit(D7_PORT, D7_PIN) : ClearBit(D7_PORT, D7_PIN);
    (cmd & 0x40) ? SetBit(D6_PORT, D6_PIN) : ClearBit(D6_PORT, D6_PIN);
    (cmd & 0x20) ? SetBit(D5_PORT, D5_PIN) : ClearBit(D5_PORT, D5_PIN);
    (cmd & 0x10) ? SetBit(D4_PORT, D4_PIN) : ClearBit(D4_PORT, D4_PIN);
    
    // Pulse enable
    SetBit(EN_PORT, EN_PIN);
    _delay_us(1);
    ClearBit(EN_PORT, EN_PIN);
    
    _delay_us(200);
    
    // Send low nibble
    (cmd & 0x08) ? SetBit(D7_PORT, D7_PIN) : ClearBit(D7_PORT, D7_PIN);
    (cmd & 0x04) ? SetBit(D6_PORT, D6_PIN) : ClearBit(D6_PORT, D6_PIN);
    (cmd & 0x02) ? SetBit(D5_PORT, D5_PIN) : ClearBit(D5_PORT, D5_PIN);
    (cmd & 0x01) ? SetBit(D4_PORT, D4_PIN) : ClearBit(D4_PORT, D4_PIN);
    
    // Pulse enable
    SetBit(EN_PORT, EN_PIN);
    _delay_us(1);
    ClearBit(EN_PORT, EN_PIN);
    
    // Commands need more time
    if (cmd == LCD_CLEAR || cmd == LCD_HOME) {
        _delay_ms(2);
    } else {
        _delay_us(50);
    }
}

// Function to send data (character) to the LCD
void lcd_char(unsigned char data) {
    // RS = 1 for data
    SetBit(RS_PORT, RS_PIN);
    
    // Send high nibble
    (data & 0x80) ? SetBit(D7_PORT, D7_PIN) : ClearBit(D7_PORT, D7_PIN);
    (data & 0x40) ? SetBit(D6_PORT, D6_PIN) : ClearBit(D6_PORT, D6_PIN);
    (data & 0x20) ? SetBit(D5_PORT, D5_PIN) : ClearBit(D5_PORT, D5_PIN);
    (data & 0x10) ? SetBit(D4_PORT, D4_PIN) : ClearBit(D4_PORT, D4_PIN);
    
    // Pulse enable
    SetBit(EN_PORT, EN_PIN);
    _delay_us(1);
    ClearBit(EN_PORT, EN_PIN);
    
    _delay_us(200);
    
    // Send low nibble
    (data & 0x08) ? SetBit(D7_PORT, D7_PIN) : ClearBit(D7_PORT, D7_PIN);
    (data & 0x04) ? SetBit(D6_PORT, D6_PIN) : ClearBit(D6_PORT, D6_PIN);
    (data & 0x02) ? SetBit(D5_PORT, D5_PIN) : ClearBit(D5_PORT, D5_PIN);
    (data & 0x01) ? SetBit(D4_PORT, D4_PIN) : ClearBit(D4_PORT, D4_PIN);
    
    // Pulse enable
    SetBit(EN_PORT, EN_PIN);
    _delay_us(1);
    ClearBit(EN_PORT, EN_PIN);
    
    _delay_us(50);
}

// Function to display a string on the LCD
void lcd_string(const char *str) {
    while (*str) {
        lcd_char(*str++);
    }
}

// Function to display a string from program memory
void lcd_string_P(const char *str) {
    char c;
    while ((c = pgm_read_byte(str++))) {
        lcd_char(c);
    }
}

// Function to clear the LCD
void lcd_clear(void) {
    lcd_command(LCD_CLEAR);
}

// Function to initialize the keypad
void keypad_init(void) {
    // Set row pins as outputs
    SetBit(ROW1_DDR, ROW1_PIN);
    SetBit(ROW2_DDR, ROW2_PIN);
    SetBit(ROW3_DDR, ROW3_PIN);
    SetBit(ROW4_DDR, ROW4_PIN);
    
    // Set row pins HIGH initially
    SetBit(ROW1_PORT, ROW1_PIN);
    SetBit(ROW2_PORT, ROW2_PIN);
    SetBit(ROW3_PORT, ROW3_PIN);
    SetBit(ROW4_PORT, ROW4_PIN);
    
    // Set column pins as inputs with pull-ups
    ClearBit(COL1_DDR, COL1_PIN);
    ClearBit(COL2_DDR, COL2_PIN);
    ClearBit(COL3_DDR, COL3_PIN);
    ClearBit(COL4_DDR, COL4_PIN);
    ClearBit(COL5_DDR, COL5_PIN);
    
    SetBit(COL1_PORT, COL1_PIN);  // Enable pull-up
    SetBit(COL2_PORT, COL2_PIN);  // Enable pull-up
    SetBit(COL3_PORT, COL3_PIN);  // Enable pull-up
    SetBit(COL4_PORT, COL4_PIN);  // Enable pull-up
    SetBit(COL5_PORT, COL5_PIN);  // Enable pull-up
}
