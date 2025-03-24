/*
   AVR-GCC Calculator Example
   This example is a port of an Arduino calculator sketch to AVR C.
   It assumes you have an HD44780 LCD with an accompanying lcd.h library.
   Adjust pin mappings and timing as necessary for your hardware.
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "lcd.h"  // You must supply this file with at least: lcd_init(), lcd_clear(), lcd_print(), lcd_set_cursor()

// ----- Pin Mapping Definitions -----
// (Adjust these definitions as needed for your wiring)
// For example, here we assume:
//   - Digit buttons (0-9) are connected to PORTD bits 0-7 and PORTB bits 0-1.
//   - Decimal button is on PORTC bit 0.
//   - Binary operator button is on PORTC bit 1.
//   - Equals button is on PORTC bit 2.
//   - Single-operation button is on PORTC bit 3.

// Note: Buttons are assumed to be wired with pull-ups (active low).
#define NUM_DIGIT_BUTTONS 10

// For simplicity we define a struct for a button’s port and bit.
typedef struct {
    volatile uint8_t *port;  // Data register pointer (for writing)
    volatile uint8_t *pin;   // Input register pointer (for reading)
    volatile uint8_t *ddr;   // Data direction register pointer
    uint8_t bit;             // Bit number (0-7)
} Button;

Button digitButtons[NUM_DIGIT_BUTTONS] = {
    { &PORTD, &PIND, &DDRD, 0 },  // Digit 0
    { &PORTD, &PIND, &DDRD, 1 },  // Digit 1
    { &PORTD, &PIND, &DDRD, 2 },  // Digit 2
    { &PORTD, &PIND, &DDRD, 3 },  // Digit 3
    { &PORTD, &PIND, &DDRD, 4 },  // Digit 4
    { &PORTD, &PIND, &DDRD, 5 },  // Digit 5
    { &PORTD, &PIND, &DDRD, 6 },  // Digit 6
    { &PORTD, &PIND, &DDRD, 7 },  // Digit 7
    { &PORTB, &PINB, &DDRB, 0 },  // Digit 8
    { &PORTB, &PINB, &DDRB, 1 }   // Digit 9
};

Button decimalButton = { &PORTC, &PINC, &DDRC, 0 };
Button opButton      = { &PORTC, &PINC, &DDRC, 1 };
Button equalsButton  = { &PORTC, &PINC, &DDRC, 2 };
Button singleOpButton= { &PORTC, &PINC, &DDRC, 3 };

// ----- Debounce settings -----
#define DEBOUNCE_DELAY_MS 50

// ----- Calculator Global Variables -----
double operand1 = 0;
double currentNumber = 0;
char op = '\0';
char displayStr[32] = "";  // Expression string

// Binary operation flags:
uint8_t operatorPressed = 0;
uint8_t opSelected = 0;

// Decimal input flags:
uint8_t decimalEntered = 0;
double decimalMultiplier = 0.1;

// Single-operation mode:
uint8_t singleOpMode = 0;
uint8_t singleOpSelected = 0;
char singleOp = '\0';

// Result display time in milliseconds
#define RESULT_DISPLAY_TIME 3000

// ----- Utility Functions -----

// Initialize a button: set as input with internal pull-up.
void init_button(Button *b) {
    *(b->ddr) &= ~(1 << b->bit);   // Input
    *(b->port) |= (1 << b->bit);    // Enable pull-up
}

// Simple debounced read; returns 1 if pressed (active low), 0 otherwise.
uint8_t read_button(Button *b) {
    if ((*(b->pin) & (1 << b->bit)) == 0) {  // button pressed
        _delay_ms(DEBOUNCE_DELAY_MS);
        if ((*(b->pin) & (1 << b->bit)) == 0) {
            return 1;
        }
    }
    return 0;
}

// Reset calculator state and update LCD.
void resetCalculator() {
    operand1 = 0;
    currentNumber = 0;
    op = '\0';
    displayStr[0] = '\0';
    operatorPressed = 0;
    opSelected = 0;
    singleOpMode = 0;
    singleOpSelected = 0;
    decimalEntered = 0;
    decimalMultiplier = 0.1;
    lcd_clear();
    lcd_print("Enter number:");
}

// Append text to displayStr (ensuring no overflow)
void append_display(const char *s) {
    strncat(displayStr, s, sizeof(displayStr) - strlen(displayStr) - 1);
}

// Append a single character to displayStr.
void append_char(char c) {
    size_t len = strlen(displayStr);
    if (len < sizeof(displayStr) - 1) {
        displayStr[len] = c;
        displayStr[len+1] = '\0';
    }
}

// Append an integer digit (0-9) to displayStr.
void append_digit(uint8_t digit) {
    char buf[2];
    buf[0] = '0' + digit;
    buf[1] = '\0';
    append_display(buf);
}

// ----- Main Function -----

int main(void) {
    // Set F_CPU correctly for _delay_ms to work.
    // Initialize LCD (assumes lcd_init is defined in lcd.h)
    lcd_init();
    lcd_print("Enter number:");

    // Initialize button pins.
    init_button(&decimalButton);
    init_button(&opButton);
    init_button(&equalsButton);
    init_button(&singleOpButton);
    for (uint8_t i = 0; i < NUM_DIGIT_BUTTONS; i++) {
        init_button(&digitButtons[i]);
    }

    // Main loop
    while (1) {
        // --- Check Decimal Button ---
        if (read_button(&decimalButton) && !decimalEntered) {
            decimalEntered = 1;
            append_char('.');
            lcd_clear();
            lcd_print(displayStr);
            _delay_ms(200);
        }
        
        // --- Check Single-Operation Button ---
        if (read_button(&singleOpButton)) {
            if (!singleOpMode) {
                singleOpMode = 1;
                currentNumber = 0;
                displayStr[0] = '\0';
                singleOpSelected = 0;
                decimalEntered = 0;
                decimalMultiplier = 0.1;
                lcd_clear();
                lcd_print("SingleOp: Press digit");
                _delay_ms(200);
            }
        }
        
        // --- Process Digit Buttons ---
        for (uint8_t i = 0; i < NUM_DIGIT_BUTTONS; i++) {
            if (read_button(&digitButtons[i])) {
                uint8_t digit = i;  // Digit value (0-9)
                if (singleOpMode) {
                    if (!singleOpSelected) {
                        // Map digit to function selection:
                        // 0: sin, 1: cos, 2: tan, 3: sin⁻¹, 4: cos⁻¹, 5: tan⁻¹,
                        // 6: exp, 7: log. For digits 8 and 9 default to sin.
                        switch (digit) {
                            case 0: singleOp = 's'; break;
                            case 1: singleOp = 'c'; break;
                            case 2: singleOp = 't'; break;
                            case 3: singleOp = 'S'; break;
                            case 4: singleOp = 'C'; break;
                            case 5: singleOp = 'T'; break;
                            case 6: singleOp = 'e'; break;
                            case 7: singleOp = 'l'; break;
                            default: singleOp = 's'; break;
                        }
                        singleOpSelected = 1;
                        // Append function text to display string.
                        if (singleOp == 's')
                            append_display("sin(");
                        else if (singleOp == 'c')
                            append_display("cos(");
                        else if (singleOp == 't')
                            append_display("tan(");
                        else if (singleOp == 'S')
                            append_display("asin(");
                        else if (singleOp == 'C')
                            append_display("acos(");
                        else if (singleOp == 'T')
                            append_display("atan(");
                        else if (singleOp == 'e')
                            append_display("exp(");
                        else if (singleOp == 'l')
                            append_display("log(");
                        lcd_clear();
                        lcd_print(displayStr);
                        _delay_ms(200);
                    } else {
                        // Build the operand for the single operation.
                        if (!decimalEntered) {
                            currentNumber = currentNumber * 10 + digit;
                        } else {
                            currentNumber = currentNumber + digit * decimalMultiplier;
                            decimalMultiplier *= 0.1;
                        }
                        append_digit(digit);
                        lcd_clear();
                        lcd_print(displayStr);
                        _delay_ms(200);
                    }
                } else {
                    // ----- Binary Operation Mode -----
                    if (!operatorPressed) {
                        // Building the first operand.
                        if (!decimalEntered) {
                            currentNumber = currentNumber * 10 + digit;
                        } else {
                            currentNumber = currentNumber + digit * decimalMultiplier;
                            decimalMultiplier *= 0.1;
                        }
                        append_digit(digit);
                        lcd_clear();
                        lcd_print(displayStr);
                        _delay_ms(200);
                    } else if (operatorPressed && !opSelected) {
                        // Digit selects the binary operator.
                        // Mapping: 0 -> '+', 1 -> '-', 2 -> '*', 3 -> '/' (others default to '+')
                        switch (digit) {
                            case 0: op = '+'; break;
                            case 1: op = '-'; break;
                            case 2: op = '*'; break;
                            case 3: op = '/'; break;
                            default: op = '+'; break;
                        }
                        opSelected = 1;
                        // Set operand1 from the previously built number.
                        operand1 = currentNumber;
                        // Prepare for next operand.
                        currentNumber = 0;
                        decimalEntered = 0;
                        decimalMultiplier = 0.1;
                        append_char(op);
                        lcd_clear();
                        lcd_print(displayStr);
                        _delay_ms(200);
                    } else if (operatorPressed && opSelected) {
                        // Building the second operand.
                        if (!decimalEntered) {
                            currentNumber = currentNumber * 10 + digit;
                        } else {
                            currentNumber = currentNumber + digit * decimalMultiplier;
                            decimalMultiplier *= 0.1;
                        }
                        append_digit(digit);
                        lcd_clear();
                        lcd_print(displayStr);
                        _delay_ms(200);
                    }
                }
            } // end if button pressed
        } // end for digitButtons
        
        // --- Process Binary Operator Button ---
        if (!singleOpMode && read_button(&opButton)) {
            // When pressed and not already in operator mode:
            operand1 = currentNumber;
            currentNumber = 0;
            decimalEntered = 0;
            decimalMultiplier = 0.1;
            operatorPressed = 1;
            lcd_clear();
            // Show prompt (append " Op?" to the expression)
            append_display(" Op?");
            lcd_print(displayStr);
            _delay_ms(200);
        }
        
        // --- Process Equals Button ---
        if (read_button(&equalsButton)) {
            // --- Single-Operation Calculation ---
            if (singleOpMode) {
                double result = 0;
                if (singleOp == 's') {
                    double rad = currentNumber * (M_PI / 180.0);
                    result = sin(rad);
                } else if (singleOp == 'c') {
                    double rad = currentNumber * (M_PI / 180.0);
                    result = cos(rad);
                } else if (singleOp == 't') {
                    double rad = currentNumber * (M_PI / 180.0);
                    result = tan(rad);
                    if (fabs(result) > 1e6 || isnan(result) || isinf(result)) {
                        lcd_clear();
                        lcd_print("Error: Undefined");
                        _delay_ms(RESULT_DISPLAY_TIME);
                        resetCalculator();
                        continue;
                    }
                } else if (singleOp == 'S') {
                    result = asin(currentNumber) * (180.0 / M_PI);
                } else if (singleOp == 'C') {
                    result = acos(currentNumber) * (180.0 / M_PI);
                } else if (singleOp == 'T') {
                    result = atan(currentNumber) * (180.0 / M_PI);
                } else if (singleOp == 'e') {
                    result = exp(currentNumber);
                } else if (singleOp == 'l') {
                    result = log(currentNumber);
                }
                
                // Domain error checking for inverse trig and logarithm.
                if (((singleOp == 'S' || singleOp == 'C' || singleOp == 'T') &&
                      (isnan(result) || isinf(result))) ||
                    (singleOp == 'l' && currentNumber <= 0)) {
                    lcd_clear();
                    lcd_print("Error: Out of range");
                    _delay_ms(RESULT_DISPLAY_TIME);
                    resetCalculator();
                    continue;
                }
                
                // Append closing parenthesis.
                append_char(')');
                lcd_clear();
                lcd_print(displayStr);
                
                // Prepare to print result on second “line.”
                char resultStr[16];
                snprintf(resultStr, sizeof(resultStr), "%.4f", result);
                lcd_set_cursor(0, 1);
                lcd_print(resultStr);
                _delay_ms(RESULT_DISPLAY_TIME);
                resetCalculator();
            }
            // --- Binary Operation Calculation ---
            else if (operatorPressed && opSelected) {
                double operand2 = currentNumber;
                if (op == '/' && operand2 == 0) {
                    lcd_clear();
                    lcd_print("Error: Div by 0");
                    _delay_ms(RESULT_DISPLAY_TIME);
                    resetCalculator();
                    continue;
                } else {
                    double result = 0;
                    switch (op) {
                        case '+': result = operand1 + operand2; break;
                        case '-': result = operand1 - operand2; break;
                        case '*': result = operand1 * operand2; break;
                        case '/': result = operand1 / operand2; break;
                        default: result = operand1 + operand2; break;
                    }
                    lcd_clear();
                    lcd_print(displayStr);
                    char resultStr[16];
                    snprintf(resultStr, sizeof(resultStr), "%.4f", result);
                    lcd_set_cursor(0, 1);
                    lcd_print(resultStr);
                    _delay_ms(RESULT_DISPLAY_TIME);
                    resetCalculator();
                }
            }
        }
        // A short delay at the end of the loop to avoid excessive polling.
        _delay_ms(10);
    } // end while(1)
    
    return 0;
}

