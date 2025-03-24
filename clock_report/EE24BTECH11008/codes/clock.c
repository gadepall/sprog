#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // Required for interrupts

// BCD pins (D2 to D5 for 7447 A, B, C, D)
#define BCD_PORT PORTD
#define BCD_DDR DDRD
#define BCD_MASK 0b00111100  // PD2 to PD5

// Common anode control (A0 to A5 for six 7-segment displays)
#define COMMON_PORT PORTC
#define COMMON_DDR DDRC

// Time variables
volatile int seconds = 0, minutes = 30, hours = 15;

void setup() {
    // Configure BCD pins (PD2-PD5) as outputs
    BCD_DDR |= BCD_MASK;
    BCD_PORT &= ~BCD_MASK; // Initialize BCD output to LOW

    // Configure common anode control (PC0-PC5) as outputs
    COMMON_DDR = 0xFF;  
    COMMON_PORT = 0x00; // Turn off all displays

    // Timer1 setup for 1-second interrupt
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC mode, Prescaler = 1024
    OCR1A = 15625; // Compare match value for 1Hz (1 sec)
    TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare interrupt

    sei(); // Enable global interrupts
}

ISR(TIMER1_COMPA_vect) {
    seconds++;
    if (seconds == 60) {
        seconds = 0;
        minutes++;
        if (minutes == 60) {
            minutes = 0;
            hours = (hours + 1) % 24;  // Wrap around 24-hour format
        }
    }
}

void displayTime();
void setBCD(int value);

int main() {
    setup();

    while (1) {
        displayTime(); // Refresh display continuously
    }
}

void displayTime() {
    int digits[6] = {
        hours / 10, hours % 10,
        minutes / 10, minutes % 10,
        seconds / 10, seconds % 10
    };

    for (int i = 0; i < 6; i++) {
        COMMON_PORT = (1 << i); // Activate current display
        setBCD(digits[i]);      // Send BCD value to 7447
        _delay_ms(2);           // Short delay for persistence of vision
        COMMON_PORT = 0x00;     // Deactivate all displays
    }
}

void setBCD(int value) {
    BCD_PORT = (BCD_PORT & ~BCD_MASK) | ((value << 2) & BCD_MASK);
}
