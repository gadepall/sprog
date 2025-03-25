#define F_CPU 16000000UL  // 16 MHz clock
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// LCD Connections (modify for your hardware)
#define LCD_RS_PORT PORTD
#define LCD_RS_PIN PD2
#define LCD_EN_PORT PORTD
#define LCD_EN_PIN PD3
#define LCD_D4_PORT PORTD
#define LCD_D4_PIN PD4
#define LCD_D5_PORT PORTD
#define LCD_D5_PIN PD5
#define LCD_D6_PORT PORTD
#define LCD_D6_PIN PD6
#define LCD_D7_PORT PORTD
#define LCD_D7_PIN PD7

// Button Definitions
#define BTN_COUNT 10
const uint8_t buttons[BTN_COUNT] = {PD6, PD7, PB0, PB1, PB2, PB3, PC0, PC1, PC2, PC3};
#define SHIFT_BTN PC4
#define EXTRA_BTN PC5

// Custom Math Constants
#define PI 3.1415926536f
#define EULER 2.7182818285f
#define ITERATIONS 10  // For approximation functions

// Custom Math Functions
float custom_pow(float base, float exp);
float custom_sqrt(float x);
float custom_sin(float x);
float custom_cos(float x);
float custom_atan(float x);
float custom_log(float x);
float custom_exp(float x);

// LCD Functions
void lcd_init();
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_print(const char *str);
void lcd_clear();
void lcd_set_cursor(uint8_t col, uint8_t row);

// Button Functions
uint8_t read_button(uint8_t pin);

// Calculator Logic
void handle_input(char c);
void evaluate_expression();
void update_display();

char input_buffer[32] = "";
uint8_t shift_active = 0;
uint8_t extra_active = 0;

// Custom Power Function (using exponentiation by squaring)
float custom_pow(float base, float exp) {
    if(exp == 0) return 1.0f;
    float result = 1.0f;
    uint8_t negative = exp < 0;
    if(negative) exp = -exp;
    
    while(exp > 0) {
        if((int)exp % 2 == 1)
            result *= base;
        base *= base;
        exp = (int)(exp / 2);
    }
    return negative ? 1.0f/result : result;
}

// Newton-Raphson Square Root
float custom_sqrt(float x) {
    if(x < 0) return 0;
    float guess = x/2.0f;
    for(uint8_t i=0; i<ITERATIONS; i++) {
        guess = (guess + x/guess)/2.0f;
    }
    return guess;
}

// Taylor Series Sine Approximation
float custom_sin(float x) {
    // Normalize angle to [-π, π]
    while(x > PI) x -= 2*PI;
    while(x < -PI) x += 2*PI;
    
    float x2 = x*x;
    float term = x;
    float result = term;
    
    for(uint8_t i=1; i<=ITERATIONS; i++) {
        term *= -x2 / ((2*i)*(2*i+1));
        result += term;
    }
    return result;
}

// Cosine from Sine identity
float custom_cos(float x) {
    return custom_sin(x + PI/2);
}

// Arctangent Approximation
float custom_atan(float x) {
    if(x > 1.0f) return PI/2 - custom_atan(1.0f/x);
    if(x < -1.0f) return -PI/2 - custom_atan(1.0f/x);
    
    float x2 = x*x;
    float term = x;
    float result = term;
    
    for(uint8_t i=1; i<=ITERATIONS; i++) {
        term *= -x2;
        result += term / (2*i+1);
    }
    return result;
}

// Natural Logarithm Approximation
float custom_log(float x) {
    if(x <= 0) return 0;
    float y = (x-1)/(x+1);
    float y2 = y*y;
    float term = y;
    float result = term;
    
    for(uint8_t i=1; i<=ITERATIONS; i++) {
        term *= y2;
        result += term / (2*i+1);
    }
    return 2*result;
}

// Exponential Function
float custom_exp(float x) {
    float term = 1.0f;
    float result = term;
    
    for(uint8_t i=1; i<=ITERATIONS; i++) {
        term *= x/i;
        result += term;
    }
    return result;
}

// LCD Implementation
void lcd_init() {
    // Set control pins as outputs
    DDRD |= (1<<LCD_RS_PIN)|(1<<LCD_EN_PIN);
    // Set data pins as outputs
    DDRD |= (1<<LCD_D4_PIN)|(1<<LCD_D5_PIN)|(1<<LCD_D6_PIN)|(1<<LCD_D7_PIN);
    
    _delay_ms(50);
    
    // Initialization sequence
    lcd_command(0x33);
    lcd_command(0x32);
    lcd_command(0x28);  // 4-bit, 2-line, 5x8 font
    lcd_command(0x0C);  // Display on, cursor off
    lcd_command(0x06);  // Entry mode
    lcd_clear();
}

void lcd_command(uint8_t cmd) {
    LCD_RS_PORT &= ~(1<<LCD_RS_PIN);
    
    // High nibble
    LCD_D4_PORT = (LCD_D4_PORT & ~(1<<LCD_D4_PIN)) | ((cmd >> 4) & 1 ? (1<<LCD_D4_PIN) : 0);
    LCD_D5_PORT = (LCD_D5_PORT & ~(1<<LCD_D5_PIN)) | ((cmd >> 5) & 1 ? (1<<LCD_D5_PIN) : 0);
    LCD_D6_PORT = (LCD_D6_PORT & ~(1<<LCD_D6_PIN)) | ((cmd >> 6) & 1 ? (1<<LCD_D6_PIN) : 0);
    LCD_D7_PORT = (LCD_D7_PORT & ~(1<<LCD_D7_PIN)) | ((cmd >> 7) & 1 ? (1<<LCD_D7_PIN) : 0);
    
    LCD_EN_PORT |= (1<<LCD_EN_PIN);
    _delay_us(1);
    LCD_EN_PORT &= ~(1<<LCD_EN_PIN);
    _delay_us(100);
    
    // Low nibble
    LCD_D4_PORT = (LCD_D4_PORT & ~(1<<LCD_D4_PIN)) | ((cmd >> 0) & 1 ? (1<<LCD_D4_PIN) : 0);
    LCD_D5_PORT = (LCD_D5_PORT & ~(1<<LCD_D5_PIN)) | ((cmd >> 1) & 1 ? (1<<LCD_D5_PIN) : 0);
    LCD_D6_PORT = (LCD_D6_PORT & ~(1<<LCD_D6_PIN)) | ((cmd >> 2) & 1 ? (1<<LCD_D6_PIN) : 0);
    LCD_D7_PORT = (LCD_D7_PORT & ~(1<<LCD_D7_PIN)) | ((cmd >> 3) & 1 ? (1<<LCD_D7_PIN) : 0);
    
    LCD_EN_PORT |= (1<<LCD_EN_PIN);
    _delay_us(1);
    LCD_EN_PORT &= ~(1<<LCD_EN_PIN);
    _delay_us(100);
}

void lcd_data(uint8_t data) {
    LCD_RS_PORT |= (1<<LCD_RS_PIN);
    
    // Same nibble handling as lcd_command
    // ... (implementation identical to lcd_command)
    
    _delay_us(100);
}

void lcd_print(const char *str) {
    while(*str) {
        lcd_data(*str++);
    }
}

void lcd_clear() {
    lcd_command(0x01);
    _delay_ms(2);
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
    lcd_command(0x80 | (col + (row * 0x40)));
}

// Button Handling
uint8_t read_button(uint8_t pin) {
    // Implement button reading based on your hardware
    // Returns 1 if pressed, 0 if not
    return 0;
}

// Calculator Logic
void handle_input(char c) {
    // Handle input characters
    // ... (implementation depends on your button mapping)
}

void evaluate_expression() {
    // Implement expression parsing and evaluation
    // using the custom math functions
}

void update_display() {
    lcd_clear();
    lcd_print(input_buffer);
}

int main(void) {
    // Initialize peripherals
    lcd_init();
    
    // Set button pins as inputs with pull-ups
    for(uint8_t i=0; i<BTN_COUNT; i++) {
        if(buttons[i] >= PC0) {
            DDRC &= ~(1<<(buttons[i]-PC0));
            PORTC |= (1<<(buttons[i]-PC0));
        } else if(buttons[i] >= PB0) {
            DDRB &= ~(1<<(buttons[i]-PB0));
            PORTB |= (1<<(buttons[i]-PB0));
        } else {
            DDRD &= ~(1<<buttons[i]);
            PORTD |= (1<<buttons[i]);
        }
    }
    
    // Main loop
    while(1) {
        // Check buttons
        for(uint8_t i=0; i<BTN_COUNT; i++) {
            if(!read_button(buttons[i])) {
                _delay_ms(50); // Debounce
                if(!read_button(buttons[i])) {
                    // Handle button press
                    while(!read_button(buttons[i])); // Wait for release
                    handle_input('0' + i); // Example - map to characters
                }
            }
        }
        
        // Check shift/extra buttons
        if(!read_button(SHIFT_BTN)) shift_active = 1;
        if(!read_button(EXTRA_BTN)) extra_active = 1;
        
        update_display();
    }
}
