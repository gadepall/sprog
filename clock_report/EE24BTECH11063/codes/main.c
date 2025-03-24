#define F_CPU 0x1000000UL // Defines the micro-controller frequency (16MHz)
#include <avr/io.h> // Header file for handling inputs-outputs
#include <util/delay.h> // Header file for handling functions related to delay
#include <avr/interrupt.h> // Required for interrupts
#include <stdbool.h> // Include boolean header

// BCD pins (D2 to D5 for 7447 A, B, C, D)
#define BCD_PORT PORTD
#define BCD_DDR DDRD
#define BCD_MASK 0x3C  // PD2 to PD5 as outputs (0b00111100)

// Common anode control (A0 to A5 for six 7-segment displays)
#define COMMON_PORT PORTC
#define COMMON_DDR DDRC

// Button pins
#define HOUR_INC_PIN PD6    // Hour increment button
#define MIN_INC_PIN PD7     // Minute increment button
#define HOUR_DEC_PIN PB0    // Hour decrement button (added)
#define MIN_DEC_PIN PB1     // Minute decrement button (added)

// Time variables (BCD format)
volatile int seconds = 0x00, minutes = 0x30, hours = 0x15;

void setup() {
    // Configure BCD pins (PD2-PD5) as outputs
    BCD_DDR |= BCD_MASK;
    BCD_PORT &= ~BCD_MASK; // Initialize BCD output to LOW

    // Configure common anode control (PC0-PC5) as outputs
    COMMON_DDR = 0xFF;  
    COMMON_PORT = 0x00; // Turn off all displays

    // Configure button pins as inputs with pull-ups
    PORTD |= (0x01 << HOUR_INC_PIN) | (0x01 << MIN_INC_PIN); // Enable internal pull-ups
    DDRB &= ~((0x01 << HOUR_DEC_PIN) | (0x01 << MIN_DEC_PIN)); // Set as inputs
    PORTB |= (0x01 << HOUR_DEC_PIN) | (0x01 << MIN_DEC_PIN); // Enable internal pull-ups

    // Timer1 setup for 1-second interrupt
    TCCR1B |= (0x01 << WGM12) | (0x01 << CS12) | (0x01 << CS10); // CTC mode, Prescaler = 1024
    OCR1A = 0x3D08; // Compare match value for 1Hz (1 sec) - 16MHz/1024/1Hz - 1
    TIMSK1 |= (0x01 << OCIE1A); // Enable Timer1 compare interrupt

    sei(); // Enable global interrupts
}

ISR(TIMER1_COMPA_vect) {
    // Increment seconds
    seconds++;
    // Adjust if lower digit > 9
    if ((seconds & 0x0F) > 0x09) {
        seconds = (seconds & 0xF0) + 0x10;
    }
    // Check for 60 seconds
    if (seconds >= 0x60) {
        seconds = 0x00;
        
        // Increment minutes
        minutes++;
        // Adjust if lower digit > 9
        if ((minutes & 0x0F) > 0x09) {
            minutes = (minutes & 0xF0) + 0x10;
        }
        // Check for 60 minutes
        if (minutes >= 0x60) {
            minutes = 0x00;
            
            // Increment hours
            hours++;
            // Adjust if lower digit > 9
            if ((hours & 0x0F) > 0x09) {
                hours = (hours & 0xF0) + 0x10;
            }
            // Check for 24 hours
            if (hours >= 0x24) {
                hours = 0x00;
            }
        }
    }
}

void displayTime();
void setBCD(int value);
void checkButtons();
void decrementHours();
void decrementMinutes();

int main() {
    setup();
    while (true) {
        checkButtons(); // Read push-buttons
        displayTime(); // Refresh display continuously
    }
}

void displayTime() {
    // Extract individual digits correctly
    int digits[0x06] = {
        (hours >> 0x04) & 0x0F,    // Hours tens digit
        hours & 0x0F,           // Hours ones digit
        (minutes >> 0x04) & 0x0F,  // Minutes tens digit
        minutes & 0x0F,         // Minutes ones digit
        (seconds >> 0x04) & 0x0F,  // Seconds tens digit
        seconds & 0x0F          // Seconds ones digit
    };

    for (int i = 0x00; i < 0x06; i++) {
        COMMON_PORT = (0x01 << i);  // Activate current display
        setBCD(digits[i]);       // Send BCD value to 7447
        _delay_ms(0x02);            // Short delay for persistence of vision
        COMMON_PORT = 0x00;         // Deactivate all displays
    }
}

void setBCD(int value) {
    BCD_PORT = (BCD_PORT & ~BCD_MASK) | ((value << 0x02) & BCD_MASK);
}

void decrementHours() {
    if (hours == 0x00) {
        hours = 0x23; // Wrap around to 23 when decrementing from 0
    } else {
        if ((hours & 0x0F) == 0x00) {
            // If the lower nibble is 0, borrow from the upper nibble
            hours = (hours - 0x10) | 0x09;
        } else {
            // Simply decrement if no BCD borrowing is needed
            hours--;
        }
    }
}


void decrementMinutes() {
    if (minutes == 0x00) {
        minutes = 0x59; // Wrap around to 59 when decrementing from 0
    } else {
        if ((minutes & 0x0F) == 0x00) {
            // If the lower nibble is 0, borrow from the upper nibble
            minutes = (minutes - 0x10) | 0x09;
        } else {
            // Simply decrement if no BCD borrowing is needed
            minutes--;
        }
    }
}


void checkButtons() {
    // Check increment buttons
    bool isHourIncPressed = !(PIND & (0x01 << HOUR_INC_PIN));
    bool isMinuteIncPressed = !(PIND & (0x01 << MIN_INC_PIN));
    
    // Check decrement buttons
    bool isHourDecPressed = !(PINB & (0x01 << HOUR_DEC_PIN));
    bool isMinuteDecPressed = !(PINB & (0x01 << MIN_DEC_PIN));

    // Handle hour increment button
    if (isHourIncPressed) {
        _delay_ms(0x32); // Debounce delay (50ms in hex)
        isHourIncPressed = !(PIND & (0x01 << HOUR_INC_PIN)); // Check again
        if (isHourIncPressed) {
            hours++;
            if ((hours & 0x0F) > 0x09) {
                hours = (hours & 0xF0) + 0x10;
            }
            if (hours >= 0x24) { // Reset at 24
                hours = 0x00;
            }
            seconds = 0x00;
            while (!(PIND & (0x01 << HOUR_INC_PIN))); // Wait for release
        }
    }

    // Handle minute increment button
    if (isMinuteIncPressed) {
        _delay_ms(0x32); // Debounce delay (50ms in hex)
        isMinuteIncPressed = !(PIND & (0x01 << MIN_INC_PIN)); // Check again
        if (isMinuteIncPressed) {
            minutes++;
            if ((minutes & 0x0F) > 0x09) {
                minutes = (minutes & 0xF0) + 0x10;
            }
            if (minutes >= 0x60) { // 60 in BCD
                minutes = 0x00;
            }
            seconds = 0x00;
            while (!(PIND & (0x01 << MIN_INC_PIN))); // Wait for release
        }
    }
    
    // Handle hour decrement button
    if (isHourDecPressed) {
        _delay_ms(0x32); // Debounce delay (50ms in hex)
        isHourDecPressed = !(PINB & (0x01 << HOUR_DEC_PIN)); // Check again
        if (isHourDecPressed) {
            decrementHours();
            seconds = 0x00;
            while (!(PINB & (0x01 << HOUR_DEC_PIN))); // Wait for release
        }
    }
    
    // Handle minute decrement button
    if (isMinuteDecPressed) {
        _delay_ms(0x32); // Debounce delay (50ms in hex)
        isMinuteDecPressed = !(PINB & (0x01 << MIN_DEC_PIN)); // Check again
        if (isMinuteDecPressed) {
            decrementMinutes();
            seconds = 0x00;
            while (!(PINB & (0x01 << MIN_DEC_PIN))); // Wait for release
        }
    }
}
