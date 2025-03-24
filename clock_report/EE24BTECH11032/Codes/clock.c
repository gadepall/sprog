#define F_CPU 16000000UL 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// BCD Pins for 7447 Decoder
#define BCD_A PD2  
#define BCD_B PD3  
#define BCD_C PD4  
#define BCD_D PD5  

// Display Control Pins
#define HR_TENS PD6  // Tens place of hours
#define HR_UNITS PD7  // Units place of hours
#define MIN_TENS PB0  // Tens place of minutes
#define MIN_UNITS PB1  // Units place of minutes
#define SEC_TENS PB2  // Tens place of seconds
#define SEC_UNITS PB3  // Units place of seconds

// Time Variables (Stored in BCD format)
volatile uint8_t hr = 0x12;  // 12 in BCD
volatile uint8_t min = 0x00; // 00 in BCD
volatile uint8_t sec = 0x00; // 00 in BCD

// Configure I/O Pins
void initPins() {
    // Set BCD (A-D) pins as output
    DDRD |= (1 << BCD_A) | (1 << BCD_B) | (1 << BCD_C) | (1 << BCD_D);
    
    // Set Display Select Pins as output
    DDRD |= (1 << HR_TENS) | (1 << HR_UNITS);  // Hours
    DDRB |= (1 << MIN_TENS) | (1 << MIN_UNITS) | (1 << SEC_TENS) | (1 << SEC_UNITS);  // Minutes & Seconds
}

// Display a single digit on the 7-segment display
void showDigit(uint8_t digit) {
    PORTD = (PORTD & 0xC3) | (digit << 2);  // Set BCD bits on PD2-PD5
}

// Multiplexing function to update the 7-segment display
void updateDisplay() {
    uint8_t h_t = (hr >> 4) & 0x0F;  // Extract tens digit
    uint8_t h_u = hr & 0x0F;         // Extract units digit
    uint8_t m_t = (min >> 4) & 0x0F;
    uint8_t m_u = min & 0x0F;
    uint8_t s_t = (sec >> 4) & 0x0F;
    uint8_t s_u = sec & 0x0F;

    // Refresh each digit sequentially
    PORTD |= (1 << HR_TENS);  showDigit(h_t); _delay_ms(5); PORTD &= ~(1 << HR_TENS);
    PORTD |= (1 << HR_UNITS); showDigit(h_u); _delay_ms(5); PORTD &= ~(1 << HR_UNITS);
    PORTB |= (1 << MIN_TENS); showDigit(m_t); _delay_ms(5); PORTB &= ~(1 << MIN_TENS);
    PORTB |= (1 << MIN_UNITS); showDigit(m_u); _delay_ms(5); PORTB &= ~(1 << MIN_UNITS);
    PORTB |= (1 << SEC_TENS); showDigit(s_t); _delay_ms(5); PORTB &= ~(1 << SEC_TENS);
    PORTB |= (1 << SEC_UNITS); showDigit(s_u); _delay_ms(5); PORTB &= ~(1 << SEC_UNITS);
}

// Increment the time and handle BCD conversion
void incrementTime() {
    sec++;

    // Handle BCD carry for seconds
    if ((sec & 0x0F) > 9) { 
        sec = (sec & 0xF0) + 0x10;  // Carry over to tens place
    }
    if (sec >= 0x60) {  // If seconds reach 60
        sec = 0x00;
        min++;
    }

    // Handle BCD carry for minutes
    if ((min & 0x0F) > 9) { 
        min = (min & 0xF0) + 0x10;
    }
    if (min >= 0x60) {  // If minutes reach 60
        min = 0x00;
        hr++;
    }

    // Handle BCD carry for hours
    if ((hr & 0x0F) > 9) { 
        hr = (hr & 0xF0) + 0x10;
    }
    if (hr >= 0x24) {  // If hours reach 24
        hr = 0x00;
    }
}

// Timer1 Compare Match Interrupt (Triggers Every 1 Second)
ISR(TIMER1_COMPA_vect) {
    incrementTime();
}

// Initialize Timer1 for 1-second interrupts
void initTimer() {
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);  // CTC mode, Prescaler 1024
    OCR1A = 15624;  // Compare match value for 1-second interrupt
    TIMSK1 |= (1 << OCIE1A);  // Enable Timer1 compare interrupt
    sei();  // Enable global interrupts
}

int main(void) {
    initPins();   // Setup pins
    initTimer();  // Start Timer1

    while (1) {
        updateDisplay();
    }
}

