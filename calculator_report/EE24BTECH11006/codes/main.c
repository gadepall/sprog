#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define STEP_SIZE 0.01
#define PI_VALUE 3.14159265359
#define E_VALUE 2.7182818

// This function computes the exponential value of e raised to the power of x using Runge-Kutta method.
double runge_kutta_single(double current_x, double current_y, double target_x, double (*derivative_function)(double, double)) {
    double k1, k2, k3, k4;
    while ((current_x - target_x > 1e-5) || (current_x - target_x < -1e-5)) {
        k1 = STEP_SIZE * derivative_function(current_x, current_y);
        k2 = STEP_SIZE * derivative_function(current_x + STEP_SIZE / 2, current_y + k1 / 2);
        k3 = STEP_SIZE * derivative_function(current_x + STEP_SIZE / 2, current_y + k2 / 2);
        k4 = STEP_SIZE * derivative_function(current_x + STEP_SIZE, current_y + k3);
        current_y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        current_x += STEP_SIZE;
    }
    return current_y;
}

// Define LCD pins
#define REGISTER_SELECT_PIN 12 // Pin used for selecting the register
#define ENABLE_PIN 11 // Pin used for enabling the LCD
#define DATA_PIN_4 5 // Data pin 4
#define DATA_PIN_5 4 // Data pin 5
#define DATA_PIN_6 3 // Data pin 6
#define DATA_PIN_7 2 // Data pin 7

// Define keypad pins
#define KEYPAD_ROW_1_PIN A0
#define KEYPAD_ROW_2_PIN A1
#define KEYPAD_ROW_3_PIN A2
#define KEYPAD_ROW_4_PIN A3

#define KEYPAD_COLUMN_1_PIN 8
#define KEYPAD_COLUMN_2_PIN 9
#define KEYPAD_COLUMN_3_PIN 10
#define KEYPAD_COLUMN_4_PIN 6
#define KEYPAD_COLUMN_5_PIN 7

// Define macros for pin manipulation
#define SET_BIT(PORT, BIT) ((PORT) |= (1 << (BIT)))
#define CLEAR_BIT(PORT, BIT) ((PORT) &= ~(1 << (BIT)))
#define READ_BIT(PORT, BIT) ((PORT) & (1 << (BIT)))

// Function to initialize the LCD
void lcd_init(void) {
    // Set data pins as output
    SET_BIT(RS_DDR, RS_PIN);
    SET_BIT(EN_DDR, EN_PIN);
    SET_BIT(D4_DDR, D4_PIN);
    SET_BIT(D5_DDR, D5_PIN);
    SET_BIT(D6_DDR, D6_PIN);
    SET_BIT(D7_DDR, D7_PIN);
    
    // Wait for LCD to initialize
    _delay_ms(50);
    
    // 4-bit initialization sequence
    // Set to 4-bit mode
    CLEAR_BIT(RS_PORT, RS_PIN);  // RS = 0 for command
    
    // Function set - 8-bit mode first (3 times)
    CLEAR_BIT(D4_PORT, D4_PIN);
    CLEAR_BIT(D5_PORT, D5_PIN);
    SET_BIT(D6_PORT, D6_PIN);
    SET_BIT(D7_PORT, D7_PIN);
    
    // Pulse enable
    SET_BIT(EN_PORT, EN_PIN);
    _delay_us(1);
    CLEAR_BIT(EN_PORT, EN_PIN);
    _delay_ms(5);
    
    // Repeat again
    SET_BIT(EN_PORT, EN_PIN);
    _delay_us(1);
    CLEAR_BIT(EN_PORT, EN_PIN);
    _delay_ms(1);
    
    // Repeat third time
    SET_BIT(EN_PORT, EN_PIN);
    _delay_us(1);
    CLEAR_BIT(EN_PORT, EN_PIN);
    _delay_ms(1);
    
    // Now set to 4-bit mode
    CLEAR_BIT(D7_PORT, D7_PIN);
    SET_BIT(EN_PORT, EN_PIN);
    _delay_us(1);
    CLEAR_BIT(EN_PORT, EN_PIN);
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

// Function to handle shift mode
void handle_shift_function(char *pressed_key, char *expression_string, int *expression_index) {
    // First, find which key was pressed in the normal keypad
    int row = -1, col = -1;
    
    for (int r = 0; r < 4 && row == -1; r++) {
        for (int c = 0; c < 5; c++) {
            if (pgm_read_byte(&keypad_normal[r][c]) == *pressed_key) {
                row = r;
                col = c;
                break;
            }
        }
    }
    
    // If found, handle the shift function
    if (row != -1 && col != -1) {
        const char* function = (const char*)pgm_read_word(&shiftFunctions[row][col]);
        
        // Handle the function based on its nam
        if (strcmp_P(function, PSTR("asin")) == 0) {
            strcpy_P(buffer, PSTR("asin("));
            write_string_to_buffer(buffer, expression_string, expression_index);
            lcd_string(buffer);
            *pressed_key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("acos")) == 0) {
            strcpy_P(buffer, PSTR("acos("));
            write_string_to_buffer(buffer, expression_string, expression_index);
            lcd_string(buffer);
            *pressed_key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("atan")) == 0) {
            strcpy_P(buffer, PSTR("atan("));
            write_string_to_buffer(buffer, expression_string, expression_index);
            lcd_string(buffer);
            *pressed_key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("y^x")) == 0) {
            *pressed_key = '^';
        } else if (strcmp_P(function, PSTR("CLR")) == 0) {
            lcd_clear();
            lcd_string_P(PSTR("Enter expression:"));
            lcd_command(LCD_SET_CURSOR | 0x40);
            *expression_index = 0;
            expression_string[*expression_index] = '\0';
            *pressed_key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("10^")) == 0) {
            strcpy_P(buffer, PSTR("10^"));
            write_string_to_buffer(buffer, expression_string, expression_index);
            lcd_string(buffer);
            *pressed_key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("e")) == 0) {
            strcpy_P(buffer, PSTR("2.7182818"));
            write_string_to_buffer(buffer, expression_string, expression_index);
            lcd_string(buffer);
            *pressed_key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("abs")) == 0) {
            strcpy_P(buffer, PSTR("abs("));
            write_string_to_buffer(buffer, expression_string, expression_index);
            lcd_string(buffer);
            *pressed_key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("cbrt")) == 0) {
            strcpy_P(buffer, PSTR("cbrt("));
            write_string_to_buffer(buffer, expression_string, expression_index);
            lcd_string(buffer);
            *pressed_key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("[")) == 0) {
            *pressed_key = '[';
        } else if (strcmp_P(function, PSTR("deg")) == 0) {
            strcpy_P(buffer, PSTR("deg("));
            write_string_to_buffer(buffer, expression_string, expression_index);
            lcd_string(buffer);
            *pressed_key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("rad")) == 0) {
            strcpy_P(buffer, PSTR("rad("));
            write_string_to_buffer(buffer, expression_string, expression_index);
            lcd_string(buffer);
            *pressed_key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("mod")) == 0) {
            *pressed_key = '%';
        } else if (strcmp_P(function, PSTR("fact")) == 0) {
            strcpy_P(buffer, PSTR("fact("));
            write_string_to_buffer(buffer, expression_string, expression_index);
            lcd_string(buffer);
            *pressed_key = 0; // Don't add the key to expression
        } else if (strcmp_P(function, PSTR("]")) == 0) {
            *pressed_key = ']';
        } else if (strcmp_P(function, PSTR("HEX")) == 0 || 
                  strcmp_P(function, PSTR("DEC")) == 0 || 
                  strcmp_P(function, PSTR("BIN")) == 0 || 
                  strcmp_P(function, PSTR("OCT")) == 0) {
            // These would change the display mode, not implemented in this example
            *pressed_key = 0; // Don't add anything to expression
        } else if (strcmp_P(function, PSTR("MC")) == 0) {
            // Memory clear function
            memory = 0.0;
            lcd_clear();
            lcd_string_P(PSTR("Memory cleared"));
            _delay_ms(1000);
            lcd_clear();
            lcd_string_P(PSTR("Enter expression:"));
            lcd_command(LCD_SET_CURSOR | 0x40);
            lcd_string(expression_string);
            *pressed_key = 0; // Don't add anything to expression
        }
    }
    
    // Turn off shift mode after handling a key
    shift_mode = 0;
}

