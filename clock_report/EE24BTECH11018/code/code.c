#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Time variables
volatile uint8_t hours = 13;
volatile uint8_t minutes = 59;
volatile uint8_t seconds = 55;
volatile uint8_t clock_running = 1;

// BCD conversion lookup table
const uint8_t bcd_lookup[10] = {
    0b0000, 0b0001, 0b0010, 0b0011, 0b0100,
    0b0101, 0b0110, 0b0111, 0b1000, 0b1001
};

void init_timer(void) {
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    OCR1A = 15624;
    TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
    if(clock_running) {
        seconds++;
        if(seconds >= 60) {
            seconds = 0;
            minutes++;
            if(minutes >= 60) {
                minutes = 0;
                hours = (hours % 12) + 1;
            }
        }
    }
}

void init_ports(void) {
    DDRD |= 0b00111100;  // PORTD 2-5: BCD outputs
    DDRB |= 0b00111111;  // PORTB 0-5: Display enables
    DDRC &= ~0b00001111; // PORTC 0-3: Button inputs
    PORTC |= 0b00001111; // Enable pull-ups
}

void display_digit(uint8_t display, uint8_t digit) {
    // Turn off all displays
    PORTB &= ~0b00111111;
    
    // Set BCD value
    PORTD = (PORTD & 0b11000011) | ((bcd_lookup[digit] << 2) & 0b00111100);
    
    // Enable selected display
    PORTB |= (1 << display);
}

void check_buttons(void) {
    static uint8_t last_state = 0xFF;
    uint8_t current_state = PINC & 0x0F;
    uint8_t pressed = last_state & ~current_state;

    if(pressed) {
        // Debounce delay
        _delay_ms(50);
        current_state = PINC & 0x0F;
        pressed = last_state & ~current_state;
    }

    if(pressed & 0x01) { // A0: Toggle clock
        clock_running ^= 1;
    }
    if(pressed & 0x02) { // A1: Inc seconds
        seconds = (seconds + 1) % 60;
    }
    if(pressed & 0x04) { // A2: Inc minutes
        minutes = (minutes + 1) % 60;
    }
    if(pressed & 0x08) { // A3: Inc hours
        hours = (hours % 12) + 1;
    }

    last_state = current_state;
}

int main(void) {
    init_ports();
    init_timer();
    sei();

    uint8_t digits[6];
    
    while(1) {
        check_buttons();

        // Calculate display digits
        digits[0] = hours / 10;
        digits[1] = hours % 10;
        digits[2] = minutes / 10;
        digits[3] = minutes % 10;
        digits[4] = seconds / 10;
        digits[5] = seconds % 10;

        // Update displays
        for(uint8_t i = 0; i < 6; i++) {
            display_digit(i, digits[i]);
            _delay_us(1000);
        }
    }
    
    return 0;
}
