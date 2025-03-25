#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

// Define LCD control pins
#define LCD_RS PB0  // Register select pin
#define LCD_EN PB1  // Enable pin
#define LCD_D4 PB2  // Data pin 4
#define LCD_D5 PB3  // Data pin 5
#define LCD_D6 PB4  // Data pin 6
#define LCD_D7 PB5  // Data pin 7

#define LCD_PORT PORTB
#define LCD_DDR DDRB

// Send a command to the LCD
void lcd_command(uint8_t cmd) {
    LCD_PORT &= ~(1 << LCD_RS);  // RS = 0 for command
    LCD_PORT &= 0x0F;  // Clear upper nibble
    LCD_PORT |= (cmd & 0xF0);  // Send upper nibble
    LCD_PORT |= (1 << LCD_EN);  // Enable pulse
    _delay_us(1);
    LCD_PORT &= ~(1 << LCD_EN);
    
    _delay_us(200);
    
    LCD_PORT &= 0x0F;  // Clear upper nibble
    LCD_PORT |= ((cmd << 4) & 0xF0);  // Send lower nibble
    LCD_PORT |= (1 << LCD_EN);
    _delay_us(1);
    LCD_PORT &= ~(1 << LCD_EN);
    
    _delay_ms(2);
}

// Send data (characters) to the LCD
void lcd_data(uint8_t data) {
    LCD_PORT |= (1 << LCD_RS);  // RS = 1 for data
    LCD_PORT &= 0x0F;  // Clear upper nibble
    LCD_PORT |= (data & 0xF0);  // Send upper nibble
    LCD_PORT |= (1 << LCD_EN);  // Enable pulse
    _delay_us(1);
    LCD_PORT &= ~(1 << LCD_EN);
    
    _delay_us(200);
    
    LCD_PORT &= 0x0F;  // Clear upper nibble
    LCD_PORT |= ((data << 4) & 0xF0);  // Send lower nibble
    LCD_PORT |= (1 << LCD_EN);
    _delay_us(1);
    LCD_PORT &= ~(1 << LCD_EN);
    
    _delay_ms(2);
}

// Initialize the LCD
void lcd_init() {
    LCD_DDR |= (1 << LCD_RS) | (1 << LCD_EN) | (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);
    _delay_ms(20);  // LCD power-up delay

    lcd_command(0x33);  // Initialize LCD in 4-bit mode
    lcd_command(0x32);  
    lcd_command(0x28);  // 4-bit mode, 2 lines, 5x8 font
    lcd_command(0x0C);  // Display ON, cursor OFF
    lcd_command(0x06);  // Entry mode set (increment cursor)
    lcd_command(0x01);  // Clear display
    _delay_ms(2);
}

// Clear the LCD screen
void lcd_clear() {
    lcd_command(0x01);
    _delay_ms(2);
}

// Move the cursor to a specified position
void lcd_set_cursor(uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    lcd_command(0x80 | (col + row_offsets[row]));
}

// Print a string to the LCD
void lcd_print(const char *str) {
    while (*str) {
        lcd_data(*str++);
    }
}

