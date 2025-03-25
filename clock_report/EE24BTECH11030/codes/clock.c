#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// BCD Pins (A, B, C, D for 7447)
#define A PD2  
#define B PD3  
#define C PD4  
#define D PD5  

// Display Control Pins
#define H1 PD6  // Hours Tens
#define H2 PD7  // Hours Units
#define M1 PB0  // Minutes Tens
#define M2 PB1  // Minutes Units
#define S1 PB2  // Seconds Tens
#define S2 PB3  // Seconds Units

// Push Button Pins
#define INC_HOURS_PIN 0b00000001  // PC0 (A0)
#define INC_MIN_TENS_PIN 0b00000010  // PC1 (A1)
#define INC_MIN_UNITS_PIN 0b00000100  // PC2 (A2)

// Clock Variables (BCD Format)
volatile uint8_t hours = 0b00010010;   // 12
volatile uint8_t minutes = 0b00000000; // 00
volatile uint8_t seconds = 0b00000000; // 00

// Display BCD Value
void displayDigit(uint8_t digit) {
    PORTD = (PORTD & 0b11000011) | (digit << 2);
}

// Multiplexed Display Update
void displayTime() {
    uint8_t h1 = (hours >> 4) & 0b00001111;
    uint8_t h2 = hours & 0b00001111;
    uint8_t m1 = (minutes >> 4) & 0b00001111;
    uint8_t m2 = minutes & 0b00001111;
    uint8_t s1 = (seconds >> 4) & 0b00001111;
    uint8_t s2 = seconds & 0b00001111;

    PORTD |= (1 << H1); displayDigit(h1); _delay_ms(5); PORTD &= ~(1 << H1);
    PORTD |= (1 << H2); displayDigit(h2); _delay_ms(5); PORTD &= ~(1 << H2);
    PORTB |= (1 << M1); displayDigit(m1); _delay_ms(5); PORTB &= ~(1 << M1);
    PORTB |= (1 << M2); displayDigit(m2); _delay_ms(5); PORTB &= ~(1 << M2);
    PORTB |= (1 << S1); displayDigit(s1); _delay_ms(5); PORTB &= ~(1 << S1);
    PORTB |= (1 << S2); displayDigit(s2); _delay_ms(5); PORTB &= ~(1 << S2);
}

// Timer Interrupt (1 Second)
ISR(TIMER1_COMPA_vect) {
    seconds++;

    if ((seconds & 0b00001111) > 0b1001) {
        seconds = (seconds & 0b11110000) + 0b00010000;
    }
    if (seconds >= 0b01100000) {  // 60 sec
        seconds = 0b00000000;
        minutes++;
    }
    if ((minutes & 0b00001111) > 0b1001) {
        minutes = (minutes & 0b11110000) + 0b00010000;
    }
    if (minutes >= 0b01100000) {  // 60 min
        minutes = 0b00000000;
        hours++;
    }
    if ((hours & 0b00001111) > 0b1001) {
        hours = (hours & 0b11110000) + 0b00010000;
    }

    // If hours go beyond 12, reset to 1
    if (hours == 0b00010011) {  // 13 in BCD
        hours = 0b00000001;  // Reset to 1
    }
}

// Timer1 Setup
void timer1_init() {
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    OCR1A = 15624;  
    TIMSK1 |= (1 << OCIE1A);  
    sei();  
}

// Read Button State
uint8_t readButton(uint8_t buttonMask) {
    if ((PINC & buttonMask) == 0) {
        _delay_ms(200);
        return 1;
    }
    return 0;
}

int main(void) {
    DDRD |= (1 << A) | (1 << B) | (1 << C) | (1 << D);
    DDRD |= (1 << H1) | (1 << H2);
    DDRB |= (1 << M1) | (1 << M2) | (1 << S1) | (1 << S2);
    DDRC &= ~(INC_HOURS_PIN | INC_MIN_TENS_PIN | INC_MIN_UNITS_PIN);
    PORTC |= (INC_HOURS_PIN | INC_MIN_TENS_PIN | INC_MIN_UNITS_PIN);

    timer1_init();

    while (1) {
        if (readButton(INC_HOURS_PIN)) {
            hours++;
            if ((hours & 0b00001111) > 0b1001) {
                hours = (hours & 0b11110000) + 0b00010000;
            }
            if (hours == 0b00010011) {  // If 13
                hours = 0b00000001;  // Reset to 1
            }
        }

        if (readButton(INC_MIN_TENS_PIN)) {
            minutes += 0b00010000;
            if (minutes >= 0b01100000) {  
                minutes = 0b00000000;
                hours++;
                if ((hours & 0b00001111) > 0b1001) {
                    hours = (hours & 0b11110000) + 0b00010000;
                }
                if (hours == 0b00010011) {  
                    hours = 0b00000001;
                }
            }
        }

        if (readButton(INC_MIN_UNITS_PIN)) {
            minutes++;
            if ((minutes & 0b00001111) > 0b1001) {
                minutes = (minutes & 0b11110000) + 0b00010000;
            }
        }

        displayTime();
    }
}
