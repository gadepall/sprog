#define F_CPU 16000000UL 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>  // Needed for _delay_ms()

// BCD Pins (A, B, C, D for 7447)
#define A PD2  
#define B PD3  
#define C PD4  
#define D PD5  

// Display Control Pins (H1 to S2 mapped to Digital 6-11)
#define H1 PD6  // Hours Tens (Digital Pin 6)
#define H2 PD7  // Hours Units (Digital Pin 7)
#define M1 PB0  // Minutes Tens (Digital Pin 8)
#define M2 PB1  // Minutes Units (Digital Pin 9)
#define S1 PB2  // Seconds Tens (Digital Pin 10)
#define S2 PB3  // Seconds Units (Digital Pin 11)

// Clock Variables (stored in BCD)
volatile uint8_t hours = 0b00010010;   // 12 in BCD
volatile uint8_t minutes = 0b00000000; // 00 in BCD
volatile uint8_t seconds = 0b00000000; // 00 in BCD

void displayDigit(uint8_t digit) {
    PORTD = (PORTD & 0b11000011) | (digit << 0b00000010);  // Set BCD bits on PD2-PD5
}

void displayTime() {
    uint8_t h1 = (hours >> 0b00000100) & 0b00001111;   // Tens place of hours
    uint8_t h2 = hours & 0b00001111;         // Units place of hours
    uint8_t m1 = (minutes >> 0b00000100) & 0b00001111;
    uint8_t m2 = minutes & 0b00001111;
    uint8_t s1 = (seconds >> 0b00000100) & 0b00001111;
    uint8_t s2 = seconds & 0b00001111;

    // Multiplexed Display Updates
    PORTD |= (0b00000001 << H1); displayDigit(h1); _delay_ms(0b00000101); PORTD &= ~(0b00000001 << H1);
    PORTD |= (0b00000001 << H2); displayDigit(h2); _delay_ms(0b00000101); PORTD &= ~(0b00000001 << H2);
    PORTB |= (0b00000001 << M1); displayDigit(m1); _delay_ms(0b00000101); PORTB &= ~(0b00000001 << M1);
    PORTB |= (0b00000001 << M2); displayDigit(m2); _delay_ms(0b00000101); PORTB &= ~(0b00000001 << M2);
    PORTB |= (0b00000001 << S1); displayDigit(s1); _delay_ms(0b00000101); PORTB &= ~(0b00000001 << S1);
    PORTB |= (0b00000001 << S2); displayDigit(s2); _delay_ms(0b00000101); PORTB &= ~(0b00000001 << S2);
}

// Timer Interrupt (1 Second)
ISR(TIMER1_COMPA_vect) {
    seconds += 0b00000001;  // Increment seconds

    // Handle BCD carry
    if ((seconds & 0b00001111) > 0b1001) { 
        seconds = (seconds & 0b11110000) + 0b00010000; // Carry to tens
    }
    if (seconds >= 0b01100000) {  // If seconds = 60 (BCD)
        seconds = 0b00000000;
        minutes += 0b00000001;
    }
    if ((minutes & 0b00001111) > 0b1001) { 
        minutes = (minutes & 0b11110000) + 0b00010000; // Carry to tens
    }
    if (minutes >= 0b01100000) {  // If minutes = 60 (BCD)
        minutes = 0b00000000;
        hours += 0b00000001;
    }
    if ((hours & 0b00001111) > 0b1001) { 
        hours = (hours & 0b11110000) + 0b00010000; // Carry to tens
    }
    if (hours >= 0b00100000) {  // If hours = 24 (BCD)
        hours = 0b00000000;
    }
}

// Timer1 Setup
void timer1_init() {
    TCCR1B |= (0b00000001 << WGM12) | (0b00000001 << CS12) | (0b00000001 << CS10);  // CTC mode, Prescaler 1024
    OCR1A = 0b0011110011111000;  // Compare match value for 1 second (16MHz / 1024 / 1Hz - 1)
    TIMSK1 |= (0b00000001 << OCIE1A);  // Enable Timer1 compare interrupt
    sei();  // Enable global interrupts
}

int main(void) {
    // Configure BCD output pins
    DDRD |= (0b00000001 << A) | (0b00000001 << B) | (0b00000001 << C) | (0b00000001 << D);
    
    // Configure Display Select Pins
    DDRD |= (0b00000001 << H1) | (0b00000001 << H2);  // Hours (PD0, PD1)
    DDRB |= (0b00000001 << M1) | (0b00000001 << M2) | (0b00000001 << S1) | (0b00000001 << S2);  // Minutes, Seconds (PB0-PB3)

    timer1_init();  // Start Timer1

    while (0b00000001) {
        displayTime();
    }
}
