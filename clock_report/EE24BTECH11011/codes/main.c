#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL

// BCD and Display Control Pins
enum {
    BCD_A = PD2, BCD_B = PD3, BCD_C = PD4, BCD_D = PD5,
    DISP1 = PD6, DISP2 = PD7, DISP3 = PB0,
    DISP4 = PB1, DISP5 = PB2, DISP6 = PB3,
    HOUR_BTN = PB4, MIN_BTN = PC0, SEC_BTN = PC1
};

volatile uint8_t hrs = 0, mins = 0, secs = 0;

void setBCDOutput(uint8_t value) {
    PORTD = (PORTD & 0xC3) | ((value & 0x0F) << 2);
}

void activateDisplay(uint8_t disp) {
    PORTD &= ~((1 << DISP1) | (1 << DISP2));
    PORTB &= ~((1 << DISP3) | (1 << DISP4) | (1 << DISP5) | (1 << DISP6));
    if (disp < 2) PORTD |= (1 << (DISP1 + disp));
    else PORTB |= (1 << (DISP3 + (disp - 2)));
}

void refreshDisplay() {
    uint8_t timeDigits[6] = { hrs / 10, hrs % 10, mins / 10, mins % 10, secs / 10, secs % 10 };
    static uint8_t activeSegment = 0;
    setBCDOutput(timeDigits[activeSegment]);
    activateDisplay(activeSegment);
    activeSegment = (activeSegment + 1) % 6;
}

void updateTime() {
    static uint16_t tickCounter = 0;
    if (++tickCounter >= 200) {
        tickCounter = 0;
        if (++secs >= 60) { secs = 0; if (++mins >= 60) { mins = 0; if (++hrs >= 24) hrs = 0; } }
    }
}

void handleButtons() {
    if (!(PINB & (1 << HOUR_BTN))) { _delay_ms(50); if (!(PINB & (1 << HOUR_BTN))) hrs = (hrs + 1) % 24; }
    if (!(PINC & (1 << MIN_BTN)))  { _delay_ms(50); if (!(PINC & (1 << MIN_BTN)))  mins = (mins + 1) % 60; }
    if (!(PINC & (1 << SEC_BTN)))  { _delay_ms(50); if (!(PINC & (1 << SEC_BTN)))  secs = (secs + 1) % 60; }
}

int main() {
    DDRD |= (1 << BCD_A) | (1 << BCD_B) | (1 << BCD_C) | (1 << BCD_D) | (1 << DISP1) | (1 << DISP2);
    DDRB |= (1 << DISP3) | (1 << DISP4) | (1 << DISP5) | (1 << DISP6);
    DDRB &= ~(1 << HOUR_BTN);
    DDRC &= ~((1 << MIN_BTN) | (1 << SEC_BTN));
    PORTB |= (1 << HOUR_BTN);
    PORTC |= (1 << MIN_BTN) | (1 << SEC_BTN);
    
    while (1) {
        handleButtons();
        updateTime();
        refreshDisplay();
        _delay_ms(5);
    }
}

