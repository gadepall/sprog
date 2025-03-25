#define F_CPU 16000000UL  // 16 MHz clock speed
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// LCD Definitions
#define LCD_RS_PORT PORTB
#define LCD_RS_DDR DDRB
#define LCD_RS_PIN PB5  // Arduino pin 13

#define LCD_E_PORT PORTB
#define LCD_E_DDR DDRB
#define LCD_E_PIN PB4  // Arduino pin 12

#define LCD_DATA4_PORT PORTC
#define LCD_DATA4_DDR DDRC
#define LCD_DATA4_PIN PC2  // Arduino pin A2

#define LCD_DATA5_PORT PORTC
#define LCD_DATA5_DDR DDRC
#define LCD_DATA5_PIN PC3  // Arduino pin A3

#define LCD_DATA6_PORT PORTC
#define LCD_DATA6_DDR DDRC
#define LCD_DATA6_PIN PC4  // Arduino pin A4

#define LCD_DATA7_PORT PORTC
#define LCD_DATA7_DDR DDRC
#define LCD_DATA7_PIN PC5  // Arduino pin A5

// Keypad Row Pins
#define ROW1_PORT PORTD
#define ROW1_DDR DDRD
#define ROW1_PIN PD6  // Arduino pin 6

#define ROW2_PORT PORTD
#define ROW2_DDR DDRD
#define ROW2_PIN PD7  // Arduino pin 7

#define ROW3_PORT PORTB
#define ROW3_DDR DDRB
#define ROW3_PIN PB0  // Arduino pin 8

#define ROW4_PORT PORTB
#define ROW4_DDR DDRB
#define ROW4_PIN PB1  // Arduino pin 9

#define ROW5_PORT PORTC
#define ROW5_DDR DDRC
#define ROW5_PIN PC1  // Arduino pin A1

// Keypad Column Pins
#define COL1_PORT PORTD
#define COL1_DDR DDRD
#define COL1_PIN PD2  // Arduino pin 2
#define COL1_PIN_REG PIND

#define COL2_PORT PORTD
#define COL2_DDR DDRD
#define COL2_PIN PD3  // Arduino pin 3
#define COL2_PIN_REG PIND

#define COL3_PORT PORTD
#define COL3_DDR DDRD
#define COL3_PIN PD4  // Arduino pin 4
#define COL3_PIN_REG PIND

#define COL4_PORT PORTD
#define COL4_DDR DDRD
#define COL4_PIN PD5  // Arduino pin 5
#define COL4_PIN_REG PIND

#define COL5_PORT PORTC
#define COL5_DDR DDRC
#define COL5_PIN PC0  // Arduino pin A0
#define COL5_PIN_REG PINC

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

#define E 2.7183
#define PI 3.14159

// Function declarations - Add these after your #define statements and before any functions
float sin_de(float x);
float cos_de(float x);
float tan_de(float x);

// Add the implementations after your string functions but before evaluate_expression
// Differential equation implementation of trigonometric functions
// Using 4th order Runge-Kutta method

float sin_de(float x) {
    float h = 0.01;  // Step size
    float y = 0.0;   // sin(0) = 0
    float dy = 1.0;  // sin'(0) = 1
    
    // Convert to radians if input is in degrees
    float x_rad = x * M_PI / 180.0;
    
    // Normalize x to range [0, 2π)
    x_rad = fmod(x_rad, 2 * M_PI);
    if (x_rad < 0) x_rad += 2 * M_PI;
    
    // Number of steps
    int steps = (int)(x_rad / h);
    
    // Apply Euler's method for second-order differential equation
    for (int i = 0; i < steps; i++) {
        float d2y = -y;  // d²y/dx² = -y
        
        // Update using 4th order Runge-Kutta for better accuracy
        float k1 = dy;
        float l1 = d2y;
        
        float k2 = dy + 0.5 * h * l1;
        float l2 = -(y + 0.5 * h * k1);
        
        float k3 = dy + 0.5 * h * l2;
        float l3 = -(y + 0.5 * h * k2);
        
        float k4 = dy + h * l3;
        float l4 = -(y + h * k3);
        
        y += h * (k1 + 2*k2 + 2*k3 + k4) / 6.0;
        dy += h * (l1 + 2*l2 + 2*l3 + l4) / 6.0;
    }
    
    return y;
}

float cos_de(float x) {
    float h = 0.01;  // Step size
    float y = 1.0;   // cos(0) = 1
    float dy = 0.0;  // cos'(0) = 0
    
    // Convert to radians if input is in degrees
    float x_rad = x * M_PI / 180.0;
    
    // Normalize x to range [0, 2π)
    x_rad = fmod(x_rad, 2 * M_PI);
    if (x_rad < 0) x_rad += 2 * M_PI;
    
    // Number of steps
    int steps = (int)(x_rad / h);
    
    // Apply Euler's method for second-order differential equation
    for (int i = 0; i < steps; i++) {
        float d2y = -y;  // d²y/dx² = -y
        
        // Update using 4th order Runge-Kutta for better accuracy
        float k1 = dy;
        float l1 = d2y;
        
        float k2 = dy + 0.5 * h * l1;
        float l2 = -(y + 0.5 * h * k1);
        
        float k3 = dy + 0.5 * h * l2;
        float l3 = -(y + 0.5 * h * k2);
        
        float k4 = dy + h * l3;
        float l4 = -(y + h * k3);
        
        y += h * (k1 + 2*k2 + 2*k3 + k4) / 6.0;
        dy += h * (l1 + 2*l2 + 2*l3 + l4) / 6.0;
    }
    
    return y;
}

float tan_de(float x) {
    float h = 0.01;  // Step size
    float y = 0.0;   // tan(0) = 0
    
    // Convert to radians if input is in degrees
    float x_rad = x * M_PI / 180.0;
    
    // Check for values close to π/2 + nπ
    float mod_x = fmod(x_rad, M_PI);
    if (fabs(mod_x - M_PI/2) < 0.1) {
        return INFINITY;  // Return INFINTY for values close to singularities
    }
    
    // Normalize x to range [0, π)
    x_rad = fmod(x_rad, M_PI);
    if (x_rad < 0) x_rad += M_PI;
    
    // Number of steps
    int steps = (int)(x_rad / h);
    
    // Apply 4th order Runge-Kutta method for first-order differential equation
    for (int i = 0; i < steps; i++) {
        float k1 = 1 + y * y;
        float k2 = 1 + (y + 0.5 * h * k1) * (y + 0.5 * h * k1);
        float k3 = 1 + (y + 0.5 * h * k2) * (y + 0.5 * h * k2);
        float k4 = 1 + (y + h * k3) * (y + h * k3);
        
        y += h * (k1 + 2*k2 + 2*k3 + k4) / 6.0;
    }
    
    return y;
}

// Keypad Mapping
char keys[5][5] = {
  {'1', '2', '3', '4', '5'},
  {'6', '7', '8', '9', '0'},
  {'+', '-', '*', '/', '='},
  {'^', 'r', 'l', 'g', 'S'},  // r = sqrt, S = Shift
  {'s', 'c', 't', '.', 'C'}  // s = sin, c = cos, t = tan, C = Clear
};

char input[32] = "";
uint8_t inputIndex = 0;
uint8_t shiftPressed = 0;

// LCD Functions
void lcd_send_nibble(uint8_t nibble) {
  // Set data pins
  if (nibble & 0x01) LCD_DATA4_PORT |= (1 << LCD_DATA4_PIN);
  else LCD_DATA4_PORT &= ~(1 << LCD_DATA4_PIN);
  
  if (nibble & 0x02) LCD_DATA5_PORT |= (1 << LCD_DATA5_PIN);
  else LCD_DATA5_PORT &= ~(1 << LCD_DATA5_PIN);
  
  if (nibble & 0x04) LCD_DATA6_PORT |= (1 << LCD_DATA6_PIN);
  else LCD_DATA6_PORT &= ~(1 << LCD_DATA6_PIN);
  
  if (nibble & 0x08) LCD_DATA7_PORT |= (1 << LCD_DATA7_PIN);
  else LCD_DATA7_PORT &= ~(1 << LCD_DATA7_PIN);
  
  // Toggle enable pin
  LCD_E_PORT |= (1 << LCD_E_PIN);
  _delay_us(1);
  LCD_E_PORT &= ~(1 << LCD_E_PIN);
  _delay_us(50);
}

void lcd_send_byte(uint8_t value, uint8_t mode) {
  // Set RS pin based on mode (data or command)
  if (mode) LCD_RS_PORT |= (1 << LCD_RS_PIN);
  else LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
  
  // Send high nibble
  lcd_send_nibble(value >> 4);
  
  // Send low nibble
  lcd_send_nibble(value & 0x0F);
  
  _delay_us(50);
}

void lcd_clear(void) {
  lcd_send_byte(LCD_CLEARDISPLAY, 0);
  _delay_ms(2);
}

void lcd_home(void) {
  lcd_send_byte(LCD_RETURNHOME, 0);
  _delay_ms(2);
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
  uint8_t row_offsets[] = {0x00, 0x40};
  lcd_send_byte(LCD_SETDDRAMADDR | (col + row_offsets[row]), 0);
}

void lcd_init(void) {
  // Configure pins as outputs
  LCD_RS_DDR |= (1 << LCD_RS_PIN);
  LCD_E_DDR |= (1 << LCD_E_PIN);
  LCD_DATA4_DDR |= (1 << LCD_DATA4_PIN);
  LCD_DATA5_DDR |= (1 << LCD_DATA5_PIN);
  LCD_DATA6_DDR |= (1 << LCD_DATA6_PIN);
  LCD_DATA7_DDR |= (1 << LCD_DATA7_PIN);
  
  // Wait for LCD to initialize
  _delay_ms(50);
  
  // Initialize in 4-bit mode
  LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
  LCD_E_PORT &= ~(1 << LCD_E_PIN);
  
  // Send first 3 commands in 8-bit mode
  lcd_send_nibble(0x03);
  _delay_ms(5);
  lcd_send_nibble(0x03);
  _delay_ms(5);
  lcd_send_nibble(0x03);
  _delay_ms(1);
  
  // Set to 4-bit mode
  lcd_send_nibble(0x02);
  _delay_ms(1);
  
  // Function set: 4-bit mode, 2 lines, 5x8 dots
  lcd_send_byte(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS, 0);
  
  // Display control: display on, cursor off, blink off
  lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF, 0);
  
  // Entry mode: left to right, no shifting
  lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT, 0);
  
  // Clear display
  lcd_clear();
}

void lcd_write(char c) {
  lcd_send_byte(c, 1);
}

void lcd_print(const char *str) {
  while (*str) {
    lcd_write(*str++);
  }
}

// Keypad Functions
void keypad_init(void) {
  // Configure row pins as outputs
  ROW1_DDR |= (1 << ROW1_PIN);
  ROW2_DDR |= (1 << ROW2_PIN);
  ROW3_DDR |= (1 << ROW3_PIN);
  ROW4_DDR |= (1 << ROW4_PIN);
  ROW5_DDR |= (1 << ROW5_PIN);
  
  // Set row pins high
  ROW1_PORT |= (1 << ROW1_PIN);
  ROW2_PORT |= (1 << ROW2_PIN);
  ROW3_PORT |= (1 << ROW3_PIN);
  ROW4_PORT |= (1 << ROW4_PIN);
  ROW5_PORT |= (1 << ROW5_PIN);
  
  // Configure column pins as inputs with pull-ups
  COL1_DDR &= ~(1 << COL1_PIN);
  COL2_DDR &= ~(1 << COL2_PIN);
  COL3_DDR &= ~(1 << COL3_PIN);
  COL4_DDR &= ~(1 << COL4_PIN);
  COL5_DDR &= ~(1 << COL5_PIN);
  
  COL1_PORT |= (1 << COL1_PIN); // Enable pull-up
  COL2_PORT |= (1 << COL2_PIN); // Enable pull-up
  COL3_PORT |= (1 << COL3_PIN); // Enable pull-up
  COL4_PORT |= (1 << COL4_PIN); // Enable pull-up
  COL5_PORT |= (1 << COL5_PIN); // Enable pull-up
}

char get_key(void) {
  uint8_t row_pins[5] = {ROW1_PIN, ROW2_PIN, ROW3_PIN, ROW4_PIN, ROW5_PIN};
  volatile uint8_t *row_ports[5] = {&ROW1_PORT, &ROW2_PORT, &ROW3_PORT, &ROW4_PORT, &ROW5_PORT};
  
  uint8_t col_pins[5] = {COL1_PIN, COL2_PIN, COL3_PIN, COL4_PIN, COL5_PIN};
  volatile uint8_t *col_pin_regs[5] = {&COL1_PIN_REG, &COL2_PIN_REG, &COL3_PIN_REG, &COL4_PIN_REG, &COL5_PIN_REG};
  
  for (uint8_t i = 0; i < 5; i++) {
    // Set current row low
    *row_ports[i] &= ~(1 << row_pins[i]);
    
    for (uint8_t j = 0; j < 5; j++) {
      // Check if column is low (button pressed)
      if (!(*col_pin_regs[j] & (1 << col_pins[j]))) {
        _delay_ms(50); // Debounce
        
        // Wait for button release
        while (!(*col_pin_regs[j] & (1 << col_pins[j])));
        
        // Set row back to high
        *row_ports[i] |= (1 << row_pins[i]);
        
        return keys[i][j];
      }
    }
    
    // Set row back to high
    *row_ports[i] |= (1 << row_pins[i]);
  }
  
  return '\0';
}

// String functions
void append_char(char c) {
  if (inputIndex < sizeof(input) - 1) {
    input[inputIndex++] = c;
    input[inputIndex] = '\0';
  }
}

void append_string(const char *str) {
  while (*str && inputIndex < sizeof(input) - 1) {
    input[inputIndex++] = *str++;
  }
  input[inputIndex] = '\0';
}

// Math functions
float parse_number(const char *str) {
  return atof(str);
}

// Modify the evaluate_expression function to properly handle operations

float evaluate_expression(const char *expr) {
  char expression[32];
  strcpy(expression, expr);
  
  // Handle e and pi substitutions first
  char processed_expr[64] = "";
  char *pos = expression;
  
  while (*pos) {
    if (*pos == 'e') {
      strcat(processed_expr, "2.7183");
      pos++;
    } else if (strncmp(pos, "3.14159", 7) == 0) {
      strcat(processed_expr, "3.14159");
      pos += 7;
    } else {
      char temp[2] = {*pos, '\0'};
      strcat(processed_expr, temp);
      pos++;
    }
  }
  
  strcpy(expression, processed_expr);
  
  // Special functions first
  
  // Evaluate Cube Root
  if (strstr(expression, "cbrt(") != NULL) {
    char *start = strstr(expression, "cbrt(");
    char *end = start + 5;
    int paren_count = 1;
    char num_str[16] = "";
    char *num_pos = num_str;
    
    while (*end && paren_count > 0) {
      if (*end == '(') paren_count++;
      else if (*end == ')') paren_count--;
      
      if (paren_count > 0)
        *num_pos++ = *end++;
      else
        end++;
    }
    *num_pos = '\0';
    
    float inner_value = evaluate_expression(num_str);
    return cbrt(inner_value);
  }
  
  // Evaluate Square Root
  if (strstr(expression, "sqrt(") != NULL) {
    char *start = strstr(expression, "sqrt(");
    char *end = start + 5;
    int paren_count = 1;
    char num_str[16] = "";
    char *num_pos = num_str;
    
    while (*end && paren_count > 0) {
      if (*end == '(') paren_count++;
      else if (*end == ')') paren_count--;
      
      if (paren_count > 0)
        *num_pos++ = *end++;
      else
        end++;
    }
    *num_pos = '\0';
    
    float inner_value = evaluate_expression(num_str);
    return sqrt(inner_value);
  }
  
  // Evaluate log (base 10)
  if (strstr(expression, "log(") != NULL) {
    char *start = strstr(expression, "log(");
    char *end = start + 4;
    int paren_count = 1;
    char num_str[16] = "";
    char *num_pos = num_str;
    
    while (*end && paren_count > 0) {
      if (*end == '(') paren_count++;
      else if (*end == ')') paren_count--;
      
      if (paren_count > 0)
        *num_pos++ = *end++;
      else
        end++;
    }
    *num_pos = '\0';
    
    float inner_value = evaluate_expression(num_str);
    return log10(inner_value);
  }
  
  // Evaluate ln (natural log)
  if (strstr(expression, "ln(") != NULL) {
    char *start = strstr(expression, "ln(");
    char *end = start + 3;
    int paren_count = 1;
    char num_str[16] = "";
    char *num_pos = num_str;
    
    while (*end && paren_count > 0) {
      if (*end == '(') paren_count++;
      else if (*end == ')') paren_count--;
      
      if (paren_count > 0)
        *num_pos++ = *end++;
      else
        end++;
    }
    *num_pos = '\0';
    
    float inner_value = evaluate_expression(num_str);
    return log(inner_value);
  }
  
  // Evaluate Inverse Trigonometry
  if (strstr(expression, "asin(") != NULL) {
    char *start = strstr(expression, "asin(");
    char *end = start + 5;
    int paren_count = 1;
    char num_str[16] = "";
    char *num_pos = num_str;
    
    while (*end && paren_count > 0) {
      if (*end == '(') paren_count++;
      else if (*end == ')') paren_count--;
      
      if (paren_count > 0)
        *num_pos++ = *end++;
      else
        end++;
    }
    *num_pos = '\0';
    
    float inner_value = evaluate_expression(num_str);
    return asin(inner_value) * 180.0 / M_PI;
  }
  
  if (strstr(expression, "acos(") != NULL) {
    char *start = strstr(expression, "acos(");
    char *end = start + 5;
    int paren_count = 1;
    char num_str[16] = "";
    char *num_pos = num_str;
    
    while (*end && paren_count > 0) {
      if (*end == '(') paren_count++;
      else if (*end == ')') paren_count--;
      
      if (paren_count > 0)
        *num_pos++ = *end++;
      else
        end++;
    }
    *num_pos = '\0';
    
    float inner_value = evaluate_expression(num_str);
    return acos(inner_value) * 180.0 / M_PI;
  }
  
  if (strstr(expression, "atan(") != NULL) {
    char *start = strstr(expression, "atan(");
    char *end = start + 5;
    int paren_count = 1;
    char num_str[16] = "";
    char *num_pos = num_str;
    
    while (*end && paren_count > 0) {
      if (*end == '(') paren_count++;
      else if (*end == ')') paren_count--;
      
      if (paren_count > 0)
        *num_pos++ = *end++;
      else
        end++;
    }
    *num_pos = '\0';
    
    float inner_value = evaluate_expression(num_str);
    return atan(inner_value) * 180.0 / M_PI;
  }
  
  // Evaluate Normal Trigonometry
  if (strstr(expression, "sin(") != NULL) {
    char *start = strstr(expression, "sin(");
    char *end = start + 4;
    int paren_count = 1;
    char num_str[16] = "";
    char *num_pos = num_str;
    
    while (*end && paren_count > 0) {
      if (*end == '(') paren_count++;
      else if (*end == ')') paren_count--;
      
      if (paren_count > 0)
        *num_pos++ = *end++;
      else
        end++;
    }
    *num_pos = '\0';
    
    float inner_value = evaluate_expression(num_str);
    return sin_de(inner_value);
  }
  
  if (strstr(expression, "cos(") != NULL) {
    char *start = strstr(expression, "cos(");
    char *end = start + 4;
    int paren_count = 1;
    char num_str[16] = "";
    char *num_pos = num_str;
    
    while (*end && paren_count > 0) {
      if (*end == '(') paren_count++;
      else if (*end == ')') paren_count--;
      
      if (paren_count > 0)
        *num_pos++ = *end++;
      else
        end++;
    }
    *num_pos = '\0';
    
    float inner_value = evaluate_expression(num_str);
    return cos_de(inner_value);
  }
  
  if (strstr(expression, "tan(") != NULL) {
    char *start = strstr(expression, "tan(");
    char *end = start + 4;
    int paren_count = 1;
    char num_str[16] = "";
    char *num_pos = num_str;
    
    while (*end && paren_count > 0) {
      if (*end == '(') paren_count++;
      else if (*end == ')') paren_count--;
      
      if (paren_count > 0)
        *num_pos++ = *end++;
      else
        end++;
    }
    *num_pos = '\0';
    
    float inner_value = evaluate_expression(num_str);
    return tan_de(inner_value);
  }
  
  // Handle basic operations
  
  // First look for addition and subtraction (lowest precedence)
  char *add_pos = strrchr(expression, '+');
  char *sub_pos = strrchr(expression, '-');
  
  // Make sure we're not looking at a negative sign
  if (sub_pos == expression) {
    // If the minus is at the beginning, it's a negative sign
    sub_pos = NULL;
    
    // Look for another minus that isn't at the beginning
    for (char *p = expression + 1; *p; p++) {
      if (*p == '-' && *(p-1) != '*' && *(p-1) != '/' && *(p-1) != '^') {
        sub_pos = p;
        break;
      }
    }
  }
  
  if (add_pos != NULL || sub_pos != NULL) {
    // Find the rightmost + or - that is not part of a number (e.g., 1e+2)
    char *op_pos = NULL;
    
    if (add_pos != NULL && (sub_pos == NULL || add_pos > sub_pos)) {
      op_pos = add_pos;
    } else {
      op_pos = sub_pos;
    }
    
    // Split expression at operator
    char left_expr[32] = "";
    char right_expr[32] = "";
    
    strncpy(left_expr, expression, op_pos - expression);
    left_expr[op_pos - expression] = '\0';
    strcpy(right_expr, op_pos + 1);
    
    float left_val = evaluate_expression(left_expr);
    float right_val = evaluate_expression(right_expr);
    
    return (*op_pos == '+') ? left_val + right_val : left_val - right_val;
  }
  
  // Next look for multiplication and division (medium precedence)
  char *mul_pos = strrchr(expression, '*');
  char *div_pos = strrchr(expression, '/');
  
  if (mul_pos != NULL || div_pos != NULL) {
    char *op_pos = NULL;
    
    if (mul_pos != NULL && (div_pos == NULL || mul_pos > div_pos)) {
      op_pos = mul_pos;
    } else {
      op_pos = div_pos;
    }
    
    // Split expression at operator
    char left_expr[32] = "";
    char right_expr[32] = "";
    
    strncpy(left_expr, expression, op_pos - expression);
    left_expr[op_pos - expression] = '\0';
    strcpy(right_expr, op_pos + 1);
    
    float left_val = evaluate_expression(left_expr);
    float right_val = evaluate_expression(right_expr);
    
    return (*op_pos == '*') ? left_val * right_val : left_val / right_val;
  }
  
  // Finally, look for power (highest precedence)
  char *pow_pos = strrchr(expression, '^');
  
  if (pow_pos != NULL) {
    // Split expression at operator
    char left_expr[32] = "";
    char right_expr[32] = "";
    
    strncpy(left_expr, expression, pow_pos - expression);
    left_expr[pow_pos - expression] = '\0';
    strcpy(right_expr, pow_pos + 1);
    
    float left_val = evaluate_expression(left_expr);
    float right_val = evaluate_expression(right_expr);
    
    return pow(left_val, right_val);
  }
  
  // If there are no operators, just return the value
  return atof(expression);
}

int main(void) {
  // Initialize LCD
  lcd_init();
  
  // Initialize Keypad
  keypad_init();
  
  // Display welcome message
  lcd_print("Sci Calc Ready!");
  
  while (1) {
    char key = get_key();
    
    if (key != '\0') {
      if (key == 'C') {
        lcd_clear();
        inputIndex = 0;
        input[0] = '\0';
        lcd_print("Cleared!");
      } else if (key == 'S') {
        shiftPressed = !shiftPressed;
        lcd_clear();
        lcd_set_cursor(0, 1);
        lcd_print("Shift: ");
        lcd_print(shiftPressed ? "On " : "Off ");
      } else if (key == '=') {
        float result = evaluate_expression(input);
        lcd_clear();
        if (isnan(result)) {
          lcd_print("NaN");
        } else {
          lcd_print(input);
          lcd_write('=');
          
          // Convert result to string
          char result_str[16];
          dtostrf(result, 4, 2, result_str);
          lcd_print(result_str);
          
          // Replace input with result
          inputIndex = 0;
          append_string(result_str);
        }
      } else {
        if (shiftPressed) {
          // Shifted Functions
          if (key == 's') append_string("asin(");
          else if (key == 'c') append_string("acos(");
          else if (key == 't') append_string("atan(");
          else if (key == 'l') append_char('e'); // Euler's number
          else if (key == 'g') append_string("log(");
          else if (key == 'r') append_string("cbrt("); // Cube Root when shifted
          else if (key == '.') append_string("3.14159"); // Pi value when shifted
          else append_char(key); // Numbers and operators
        } else {
          // Normal Functions
          if (key == 's') append_string("sin(");
          else if (key == 'c') append_string("cos(");
          else if (key == 't') append_string("tan(");
          else if (key == 'r') append_string("sqrt(");  // Square Root
          else if (key == 'l') append_string("ln(");
          else if (key == 'g') append_string("log(");
          else append_char(key);
        }
        
        lcd_clear();
        lcd_print(input);
      }
    }
  }
  
  return 0;
}
