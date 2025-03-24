/*
 * Digital Clock using 6 seven-segment displays with SN7447 driver
 * ATmega328P (Arduino Uno) version in pure C for avr-gcc
 *
 * Pin Mapping:
 *   BCD outputs (to SN7447): Arduino pins 2-5  => PD2, PD3, PD4, PD5
 *   Digit Select outputs:
 *      Digit 1 (tens of hours): pin 6  => PD6
 *      Digit 2 (ones of hours):  pin 7  => PD7
 *      Digit 3 (tens of minutes):pin 8  => PB0
 *      Digit 4 (ones of minutes):pin 9  => PB1
 *      Digit 5 (tens of seconds):pin 10 => PB2
 *      Digit 6 (ones of seconds):pin 11 => PB3
 *   Push Buttons:
 *      Hour adjust:   pin 12 => PB4 (input with pull-up)
 *      Minute adjust: pin 13 => PB5 (input with pull-up)
 *
 * The SN7447 decodes the 4-bit BCD (on PD2-PD5) into the segments
 * for the displays. The displays are multiplexed by activating their
 * common anode via the digit select pins.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define DIGIT_DELAY_MS 2    // Time (in ms) each digit is active
#define NUM_DIGITS 6

int main(void) {
    // --- I/O Setup ---

    // Set up BCD output pins on PORTD: PD2, PD3, PD4, PD5 as outputs
    DDRD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5);

    // Set up digit select pins:
    // Arduino pins 6 and 7: PD6 and PD7 as outputs (for digits 1 and 2)
    DDRD |= (1 << PD6) | (1 << PD7);
    // Arduino pins 8, 9, 10, 11: PB0-PB3 as outputs (for digits 3 to 6)
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);

    // Set up push button pins on Arduino pins 12 and 13: PB4 and PB5 as inputs with pull-ups
    DDRB &= ~((1 << PB4) | (1 << PB5));  // Set PB4 and PB5 as inputs
    PORTB |= (1 << PB4) | (1 << PB5);      // Enable internal pull-ups on PB4 and PB5

    // --- Clock Variables ---
    // Initialize starting time: 12:00:00
    uint8_t hours = 12;
    uint8_t minutes = 0;
    uint8_t seconds = 0;
    
    // Cycle counter to approximate 1-second elapsed time
    uint16_t cycle_count = 0;  // Each full multiplex cycle is ~12 ms (6 digits x 2ms)

    // --- Main Loop ---
    while (1) {
        // Break the current time into 6 digits for HH:MM:SS.
        uint8_t digits[NUM_DIGITS];
        digits[0] = hours / 10;       // Tens digit of hours
        digits[1] = hours % 10;       // Ones digit of hours
        digits[2] = minutes / 10;     // Tens digit of minutes
        digits[3] = minutes % 10;     // Ones digit of minutes
        digits[4] = seconds / 10;     // Tens digit of seconds
        digits[5] = seconds % 10;     // Ones digit of seconds

        // --- Multiplexing: Cycle through each digit ---
        for (uint8_t i = 0; i < NUM_DIGITS; i++) {
            // Set BCD outputs on PD2-PD5
            // Clear bits PD2-PD5 (mask: 0x3C = 00111100) then set new BCD value shifted left by 2.
            PORTD = (PORTD & ~0x3C) | ((digits[i] << 2) & 0x3C);

            // Activate the appropriate digit select pin.
            // For digits 0 and 1, use PORTD pins PD6 and PD7.
            if (i < 2) {
                if (i == 0)
                    PORTD |= (1 << PD6);  // Activate digit 1 (tens of hours)
                else // i == 1
                    PORTD |= (1 << PD7);  // Activate digit 2 (ones of hours)
            } else {
                // For digits 2 to 5, use PORTB pins PB0 to PB3.
                // (i=2 => PB0, i=3 => PB1, i=4 => PB2, i=5 => PB3)
                PORTB |= (1 << (i - 2));
            }
            _delay_ms(DIGIT_DELAY_MS);

            // Deactivate the digit select pin to turn off the digit.
            if (i < 2) {
                if (i == 0)
                    PORTD &= ~(1 << PD6);
                else // i == 1
                    PORTD &= ~(1 << PD7);
            } else {
                PORTB &= ~(1 << (i - 2));
            }
        } // End of multiplex loop

        // Each complete multiplex cycle is approximately 6 x 2ms = 12ms.
        cycle_count++;

        // --- Check Push Buttons for Time Adjustments ---
        // Button on PB4: Adjust hour (active LOW)
        if (!(PINB & (1 << PB4))) {
            _delay_ms(50); // Debounce delay
            if (!(PINB & (1 << PB4))) {
                hours = (hours + 1) % 24;
                // Wait until button is released.
                while (!(PINB & (1 << PB4)));
                _delay_ms(50); // Extra debounce after release
            }
        }
        // Button on PB5: Adjust minute (active LOW)
        if (!(PINB & (1 << PB5))) {
            _delay_ms(50);
            if (!(PINB & (1 << PB5))) {
                minutes = (minutes + 1) % 60;
                while (!(PINB & (1 << PB5)));
                _delay_ms(50);
            }
        }

        // --- Update the Clock Time ---
        // Approximately every 1000 ms (83 cycles x ~12ms ≈ 996ms), update the time.
        if (cycle_count >= 83) {
            cycle_count = 0;
            seconds++;
            if (seconds >= 60) {
                seconds = 0;
                minutes++;
                if (minutes >= 60) {
                    minutes = 0;
                    hours++;
                    if (hours >= 24)
                        hours = 0;
                }
            }
        }
    } // End of main loop

    return 0;
}

