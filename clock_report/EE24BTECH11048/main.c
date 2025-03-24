#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// BCD pins (B0 to B3 for 7447 A, B, C, D)
#define BCD_PORT PORTB
#define BCD_DDR DDRB
#define BCD_MASK 0b00001111  // PB0 to PB3

// Common anode control (PB4, PB5, PD0, PD1, PD2, PD3 for six 7-segment displays)
#define COMMON_PORT_B PORTB
#define COMMON_DDR_B DDRB
#define COMMON_MASK_B 0b00110000  // PB4 and PB5

#define COMMON_PORT_D PORTD
#define COMMON_DDR_D DDRD
#define COMMON_MASK_D 0b00001111  // PD0 to PD3

// Button pins
#define BTN_CLK PC4  // Button 1 on A4 (PC4)
#define BTN_SW PC5   // Button 2 on A5 (PC5)
#define BTN_TMR PD7  // Button 3 on PD7

// Time variables
volatile int sec = 0, min = 30, hr = 15;
volatile int sw_sec = 0, sw_min = 0, sw_hr = 0;
volatile int tmr_sec = 0, tmr_min = 5, tmr_hr = 0;
volatile int mode = 0;

// Function prototypes
void showTime(int hr, int min, int sec);
void setDigit(int val);

void init() {
    // Configure BCD pins as outputs
    BCD_DDR |= BCD_MASK;
    BCD_PORT &= ~BCD_MASK;

    // Configure common anode control as outputs
    COMMON_DDR_B |= COMMON_MASK_B;
    COMMON_DDR_D |= COMMON_MASK_D;
    COMMON_PORT_B &= ~COMMON_MASK_B;
    COMMON_PORT_D &= ~COMMON_MASK_D;

    // Configure button pins as inputs with pull-up resistors
    DDRC &= ~((1 << BTN_CLK) | (1 << BTN_SW));
    PORTC |= (1 << BTN_CLK) | (1 << BTN_SW);
    DDRD &= ~(1 << BTN_TMR);
    PORTD |= (1 << BTN_TMR);

    // Timer1 setup for 1-second interrupt
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    OCR1A = 15625;
    TIMSK1 |= (1 << OCIE1A);

    sei();
}

ISR(TIMER1_COMPA_vect) {
    sec++;
    if (sec == 60) {
        sec = 0;
        min++;
        if (min == 60) {
            min = 0;
            hr = (hr + 1) % 24;
        }
    }

    if (mode == 1) {
        sw_sec++;
        if (sw_sec == 60) {
            sw_sec = 0;
            sw_min++;
            if (sw_min == 60) {
                sw_min = 0;
                sw_hr++;
            }
        }
    }

    if (mode == 2) {
        if (tmr_sec == 0 && tmr_min == 0 && tmr_hr == 0) {
            mode = 0;
        } else {
            if (tmr_sec == 0) {
                tmr_sec = 59;
                if (tmr_min == 0) {
                    tmr_min = 59;
                    if (tmr_hr > 0) {
                        tmr_hr--;
                    }
                } else {
                    tmr_min--;
                }
            } else {
                tmr_sec--;
            }
        }
    }
}

int main() {
    init();

    while (1) {
        if (!(PINC & (1 << BTN_CLK))) {
            _delay_ms(50);
            if (!(PINC & (1 << BTN_CLK))) {
                mode = 0;
            }
        }
        if (!(PINC & (1 << BTN_SW))) {
            _delay_ms(50);
            if (!(PINC & (1 << BTN_SW))) {
                mode = 1;
                sw_sec = 0;
                sw_min = 0;
                sw_hr = 0;
            }
        }
        if (!(PIND & (1 << BTN_TMR))) {
            _delay_ms(50);
            if (!(PIND & (1 << BTN_TMR))) {
                mode = 2;
                tmr_sec = 0;
                tmr_min = 5;
                tmr_hr = 0;
            }
        }

        if (mode == 0) {
            showTime(hr, min, sec);
        } else if (mode == 1) {
            showTime(sw_hr, sw_min, sw_sec);
        } else if (mode == 2) {
            showTime(tmr_hr, tmr_min, tmr_sec);
        }
    }
}

void showTime(int hr, int min, int sec) {
    int digits[6] = {
        hr / 10, hr % 10,
        min / 10, min % 10,
        sec / 10, sec % 10
    };

    for (int i = 0; i < 6; i++) {
        if (i < 2) {
            COMMON_PORT_B = (COMMON_PORT_B & ~COMMON_MASK_B) | ((1 << (i + 4)) & COMMON_MASK_B);
            COMMON_PORT_D &= ~COMMON_MASK_D;
        } else {
            COMMON_PORT_D = (COMMON_PORT_D & ~COMMON_MASK_D) | ((1 << (i - 2)) & COMMON_MASK_D);
            COMMON_PORT_B &= ~COMMON_MASK_B;
        }
        setDigit(digits[i]);
        _delay_ms(2);
        COMMON_PORT_B &= ~COMMON_MASK_B;
        COMMON_PORT_D &= ~COMMON_MASK_D;
    }
}

void setDigit(int val) {
    BCD_PORT = (BCD_PORT & ~BCD_MASK) | (val & BCD_MASK);
}
