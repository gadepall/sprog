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

float evaluate_expression(const char *expr) {
  char expression[32];
  strcpy(expression, expr);
  
  // Handle e and pi
  if (strstr(expression, "e") != NULL) {
    char temp[64];
    char *pos = strstr(expression, "e");

    // Copy everything before 'e'
    strncpy(temp, expression, pos - expression);
    temp[pos - expression] = '\0';
    // Append the value of e
    strcat(temp, "2.7183");

    // Append everything after 'e'
    strcat(temp, pos + 1);

    strcpy(expression, temp);  // Update expression
  }
  
  // Handle pi
  if (strstr(expression, "3.14159") != NULL) {
    char temp[64];
    char *pos = strstr(expression, "3.14159");

    // Copy everything before 'pi'
    strncpy(temp, expression, pos - expression);
    temp[pos - expression] = '\0';

    // Append the value of pi
    strcat(temp, "3.14159");

    // Append everything after 'pi'
    strcat(temp, pos + 7);

    strcpy(expression, temp);  // Update expression
  }

  // Evaluate Cube Root
  if (strstr(expression, "cbrt(") != NULL) {
    char *start = strstr(expression, "cbrt(");
    char *end = start + 5;
    char num_str[16];
    strcpy(num_str, end);
    float value = parse_number(num_str);
    return cbrt(value);
  }
  
  // Evaluate Inverse Trigonometry
  if (strstr(expression, "asin(") != NULL) {
    char *start = strstr(expression, "asin(");
    char *end = start + 5;
    char num_str[16];
    strcpy(num_str, end);
    float value = parse_number(num_str);
    return asin(value) * 180.0 / M_PI;
  }
  
  if (strstr(expression, "acos(") != NULL) {
    char *start = strstr(expression, "acos(");
    char *end = start + 5;
    char num_str[16];
    strcpy(num_str, end);
    float value = parse_number(num_str);
    return acos(value) * 180.0 / M_PI;
  }
  
  if (strstr(expression, "atan(") != NULL) {
    char *start = strstr(expression, "atan(");
    char *end = start + 5;
    char num_str[16];
    strcpy(num_str, end);
    float value = parse_number(num_str);
    return atan(value) * 180.0 / M_PI;
  }
  
  // Evaluate Normal Trigonometry
  if (strstr(expression, "sin(") != NULL) {
    char *start = strstr(expression, "sin(");
    char *end = start + 4;
    char num_str[16];
    strcpy(num_str, end);
    float value = parse_number(num_str);
    return sin(value * M_PI / 180.0);
  }
  
  if (strstr(expression, "cos(") != NULL) {
    char *start = strstr(expression, "cos(");
    char *end = start + 4;
    char num_str[16];
    strcpy(num_str, end);
    float value = parse_number(num_str);
    return cos(value * M_PI / 180.0);
  }
  
  if (strstr(expression, "tan(") != NULL) {
    char *start = strstr(expression, "tan(");
    char *end = start + 4;
    char num_str[16];
    strcpy(num_str, end);
    float value = parse_number(num_str);
    return tan(value * M_PI / 180.0);
  }
  
  // Evaluate Square Root
  if (strstr(expression, "sqrt(") != NULL) {
    char *start = strstr(expression, "sqrt(");
    char *end = start + 5;
    char num_str[16];
    strcpy(num_str, end);
    float value = parse_number(num_str);
    return sqrt(value);
  }
  
  // Evaluate Log and Natural Log
  if (strstr(expression, "ln(") != NULL) {
    char *start = strstr(expression, "ln(");
    char *end = start + 3;
    char num_str[16];
    strcpy(num_str, end);
    float value = parse_number(num_str);
    return log(value);
  }
  
  if (strstr(expression, "log(") != NULL) {
    char *start = strstr(expression, "log(");
    char *end = start + 4;
    char num_str[16];
    strcpy(num_str, end);
    float value = parse_number(num_str);
    return log10(value);
  }
  
  // Basic Operations
  float num1 = 0.0, num2 = 0.0;
  char op = ' ';
  int i = 0;
  
  while (expression[i] != '\0') {
    char ch = expression[i];
    if ((ch >= '0' && ch <= '9') || ch == '.') {
      if (op == ' ') {
        num1 = num1 * 10.0 + (ch - '0');
      } else {
        num2 = num2 * 10.0 + (ch - '0');
      }
    } else if (ch == '+'  ch == '-'  ch == '*'  ch == '/'  ch == '^') {
      op = ch;
    }
    i++;
  }
  
  switch (op) {
    case '+': return num1 + num2;
    case '-': return num1 - num2;
    case '*': return num1 * num2;
    case '/': return (num2 != 0.0) ? num1 / num2 : NAN;
    case '^': return pow(num1, num2);
    default: return num1;
  }
}

int main(void) {
  // Initialize LCD
  lcd_init();
  
  // Initialize Keypad
  keypad_init();
  
  // Display welcome message
  lcd_print("Calc Ready!");
  
  while (1) {
    char key = get_key();
    
    if (key != '\0') {
      if (key == 'C') {
        lcd_clear();
        inputIndex = 0;
        input[0] = '\0';
        lcd_print("Cleared");
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