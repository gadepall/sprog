#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

// LCD Pins
#define LCD_RS PD2
#define LCD_EN PD3
#define LCD_D4 PD4
#define LCD_D5 PD5
#define LCD_D6 PD6
#define LCD_D7 PD7

// Button Assignments
#define BTN_0 PB0
#define BTN_1 PB1
#define BTN_2 PB2
#define BTN_3 PB3
#define BTN_4 PB4  // sin(x)
#define BTN_5 PB5  // cos(x)
#define BTN_6 PC0
#define BTN_7 PC1
#define BTN_8 PC2
#define BTN_9 PC3
#define BTN_EQ PC4
#define BTN_MODE PC5

// LCD Function Prototypes
void lcd_send_cmd(uint8_t cmd);
void lcd_initialize(void);
void lcd_clear_screen(void);
void lcd_display_char(char data);
void lcd_display_string(char *text);
void lcd_display_mode(int mode);

// Mathematical Function Prototypes
long double compute_sin(long double x);
long double compute_cos(long double x);
long double compute_ln(long double x);
long double compute_sqrt(long double x);
long double compute_arctan(double x);
char get_button_input(int mode);

// LCD Functions Implementation
void lcd_send_cmd(uint8_t cmd) {
    PORTD = (PORTD & 0x0F) | (cmd & 0xF0);
    PORTD &= ~(1 << LCD_RS);
    PORTD |= (1 << LCD_EN);
    _delay_ms(2);
    PORTD &= ~(1 << LCD_EN);
    PORTD = (PORTD & 0x0F) | (cmd << 4);
    PORTD |= (1 << LCD_EN);
    _delay_ms(2);
    PORTD &= ~(1 << LCD_EN);
}

void lcd_initialize(void) {
    DDRD |= (1 << LCD_RS) | (1 << LCD_EN) | (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);
    _delay_ms(100);
    lcd_send_cmd(0x33);
    lcd_send_cmd(0x32);
    lcd_send_cmd(0x28);
    lcd_send_cmd(0x0C);
    lcd_send_cmd(0x06);
    _delay_ms(5);
}

void lcd_clear_screen(void) {
    lcd_send_cmd(0x01);
    _delay_ms(5);
}

void lcd_display_char(char data) {
    PORTD = (PORTD & 0x0F) | (data & 0xF0);
    PORTD |= (1 << LCD_RS);
    PORTD |= (1 << LCD_EN);
    _delay_ms(2);
    PORTD &= ~(1 << LCD_EN);
    PORTD = (PORTD & 0x0F) | (data << 4);
    PORTD |= (1 << LCD_EN);
    _delay_ms(2);
    PORTD &= ~(1 << LCD_EN);
}

void lcd_display_string(char *text) {
    while (*text) {
        lcd_display_char(*text++);
    }
}

void lcd_display_mode(int mode) {
    lcd_send_cmd(0xC0);
    lcd_display_string(mode ? "Mode: Sci" : "Mode: Basic");
}

// Mathematical Computations
long double compute_sin(long double x) {
    return sin(x);
}

long double compute_cos(long double x) {
    return cos(x);
}

long double compute_ln(long double x) {
    return (x > 0) ? log(x) : -1;
}

long double compute_sqrt(long double x) {
    return (x >= 0) ? sqrt(x) : -1;
}

double compute_arctan(double x) {
    return atan(x);
}

// Button Input Handling
char get_button_input(int mode) {
    if (!(PINB & (1 << BTN_0))) return mode ? '+' : '0';
    if (!(PINB & (1 << BTN_1))) return mode ? '-' : '1';
    if (!(PINB & (1 << BTN_2))) return mode ? '*' : '2';
    if (!(PINB & (1 << BTN_3))) return mode ? '/' : '3';
    if (!(PINB & (1 << BTN_4))) return mode ? 'S' : '4';
    if (!(PINB & (1 << BTN_5))) return mode ? 'C' : '5';
    if (!(PINC & (1 << BTN_6))) return mode ? 'L' : '6';
    if (!(PINC & (1 << BTN_7))) return mode ? 'R' : '7';
    if (!(PINC & (1 << BTN_8))) return mode ? 'T' : '8';
    if (!(PINC & (1 << BTN_9))) return '9';
    if (!(PINC & (1 << BTN_MODE))) return 'M';
    if (!(PINC & (1 << BTN_EQ))) return '=';
    return '\0';
}

// Main Execution
int main(void) {
    DDRB &= ~0x3F;
    DDRC &= ~0x3F;
    PORTB |= 0x3F;
    PORTC |= 0x3F;

    lcd_initialize();
    lcd_clear_screen();
    lcd_display_string("Ready!");

    int num1 = 0, num2 = 0, state = 0, sci_mode = 0;
    char operation = '\0', display[16];

    while (1) {
        lcd_display_mode(sci_mode);
        char key = get_button_input(sci_mode);

        if (key) {
            _delay_ms(200);

            if (key == 'M') {
                sci_mode = !sci_mode;
            } else if (state == 0) {
                if (key >= '0' && key <= '9') {
                    num1 = num1 * 10 + (key - '0');
                    sprintf(display, "%d", num1);
                    lcd_clear_screen();
                    lcd_display_string(display);
                } else {
                    operation = key;
                    state = 1;
                    sprintf(display, "%d %c", num1, operation);
                    lcd_clear_screen();
                    lcd_display_string(display);
                }
            } else if (state == 1) {
                if (key >= '0' && key <= '9') {
                    num2 = num2 * 10 + (key - '0');
                    sprintf(display, "%d %c %d", num1, operation, num2);
                    lcd_clear_screen();
                    lcd_display_string(display);
                } else if (key == '=') {
                    double result = (operation == '+') ? num1 + num2 :
                                    (operation == '-') ? num1 - num2 :
                                    (operation == '*') ? num1 * num2 :
                                    (operation == '/' && num2 != 0) ? (double)num1 / num2 : 0;
                    sprintf(display, "%d %c %d = %.2f", num1, operation, num2, result);
                    lcd_clear_screen();
                    lcd_display_string(display);
                    num1 = num2 = 0;
                    state = 0;
                }
            }
        }
    }
}

