#define F_CPU 16000000UL  // 16 MHz clock
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//========================= LCD CONFIGURATION =========================//
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

//========================= BUTTON CONFIGURATION =========================//
#define BTN_COUNT 10
const uint8_t buttons[BTN_COUNT] = {PD6, PD7, PB0, PB1, PB2, PB3, PC0, PC1, PC2, PC3};
#define SHIFT_BTN PC4
#define EXTRA_BTN PC5

//========================= CONSTANTS & VARIABLES =========================//
#define PI 3.1415926536f
#define EULER 2.7182818285f
#define ITERATIONS 10  // For approximation functions

char input_buffer[32] = "";
uint8_t shift_active = 0;
uint8_t extra_active = 0;

//========================= FUNCTION PROTOTYPES =========================//
// Math Functions
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

// Button Handling
uint8_t read_button(uint8_t pin);

// Calculator Logic
void handle_input(char c);
void evaluate_expression();
void update_display();

//========================= CUSTOM MATH FUNCTIONS =========================//
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

float custom_sqrt(float x) {
    if(x < 0) return 0;
    float guess = x/2.0f;
    for(uint8_t i=0; i<ITERATIONS; i++) {
        guess = (guess + x/guess)/2.0f;
    }
    return guess;
}

float custom_sin(float x) {
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

float custom_cos(float x) {
    return custom_sin(x + PI/2);
}

//========================= LCD FUNCTIONS =========================//
void lcd_init() {
    DDRD |= (1<<LCD_RS_PIN)|(1<<LCD_EN_PIN);
    DDRD |= (1<<LCD_D4_PIN)|(1<<LCD_D5_PIN)|(1<<LCD_D6_PIN)|(1<<LCD_D7_PIN);
    _delay_ms(50);
    lcd_command(0x33);
    lcd_command(0x32);
    lcd_command(0x28);
    lcd_command(0x0C);
    lcd_command(0x06);
    lcd_clear();
}

void lcd_command(uint8_t cmd) {
    LCD_RS_PORT &= ~(1<<LCD_RS_PIN);
    LCD_D4_PORT = (cmd & 0x10) ? (1<<LCD_D4_PIN) : 0;
    LCD_D5_PORT = (cmd & 0x20) ? (1<<LCD_D5_PIN) : 0;
    LCD_D6_PORT = (cmd & 0x40) ? (1<<LCD_D6_PIN) : 0;
    LCD_D7_PORT = (cmd & 0x80) ? (1<<LCD_D7_PIN) : 0;
    LCD_EN_PORT |= (1<<LCD_EN_PIN);
    _delay_us(1);
    LCD_EN_PORT &= ~(1<<LCD_EN_PIN);
    _delay_us(100);
}

void lcd_data(uint8_t data) {
    LCD_RS_PORT |= (1<<LCD_RS_PIN);
    lcd_command(data);
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

//========================= BUTTON HANDLING =========================//
uint8_t read_button(uint8_t pin) {
    return 0;
}

//========================= MAIN PROGRAM =========================//
int main(void) {
    lcd_init();
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
    while(1) {
        for(uint8_t i=0; i<BTN_COUNT; i++) {
            if(!read_button(buttons[i])) {
                _delay_ms(50);
                if(!read_button(buttons[i])) {
                    while(!read_button(buttons[i]));
                    handle_input('0' + i);
                }
            }
        }
        if(!read_button(SHIFT_BTN)) shift_active = 1;
        if(!read_button(EXTRA_BTN)) extra_active = 1;
        update_display();
    }
}

