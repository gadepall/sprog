#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // Required for interrupts

// BCD pins (D2 to D5 for 7447 A, B, C, D)
#define BCD_PORT PORTD
#define BCD_DDR DDRD
#define BCD_MASK 0b00111100  // PD2 to PD5

// Common anode control (PD6, PD7, PB0, PB1, PB2, PB3 for six 7-segment displays)
#define COMMON_PORT_D PORTD
#define COMMON_DDR_D DDRD
#define COMMON_MASK_D 0b11000000  // PD6 and PD7

#define COMMON_PORT_B PORTB
#define COMMON_DDR_B DDRB
#define COMMON_MASK_B 0b00001111  // PB0 to PB3

// Button pins
#define BUTTON_CLOCK_PIN PC4      // Button 1 connected to A4 (PC4)
#define BUTTON_STOPWATCH_PIN PC5  // Button 2 connected to A5 (PC5)
#define BUTTON_TIMER_PIN PD1      // Button 3 connected to PD1

// Time variables
volatile int seconds = 0, minutes = 30, hours = 15; // Clock time
volatile int stopwatch_seconds = 0, stopwatch_minutes = 0, stopwatch_hours = 0; // Stopwatch time
volatile int timer_seconds = 0, timer_minutes = 5, timer_hours = 0; // Timer set to 5 minutes
volatile int mode = 0; // 0 = Clock, 1 = Stopwatch, 2 = Timer

void setup() {
    // Configure BCD pins (PD2-PD5) as outputs
    BCD_DDR |= BCD_MASK;
    BCD_PORT &= ~BCD_MASK; // Initialize BCD output to LOW

    // Configure common anode control (PD6, PD7, PB0-PB3) as outputs
    COMMON_DDR_D |= COMMON_MASK_D;  // Set PD6 and PD7 as outputs
    COMMON_DDR_B |= COMMON_MASK_B;  // Set PB0 to PB3 as outputs
    COMMON_PORT_D &= ~COMMON_MASK_D; // Turn off PD6 and PD7
    COMMON_PORT_B &= ~COMMON_MASK_B; // Turn off PB0 to PB3

    // Configure button pins as inputs with pull-up resistors
    DDRC &= ~((1 << BUTTON_CLOCK_PIN) | (1 << BUTTON_STOPWATCH_PIN)); // PC4 and PC5 as inputs
    PORTC |= (1 << BUTTON_CLOCK_PIN) | (1 << BUTTON_STOPWATCH_PIN); // Enable pull-up resistors for PC4 and PC5
    DDRD &= ~(1 << BUTTON_TIMER_PIN); // PD1 as input
    PORTD |= (1 << BUTTON_TIMER_PIN); // Enable pull-up resistor for PD1

    // Timer1 setup for 1-second interrupt
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC mode, Prescaler = 1024
    OCR1A = 15625; // Compare match value for 1Hz (1 sec)
    TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare interrupt

    sei(); // Enable global interrupts
}

ISR(TIMER1_COMPA_vect) {
    // Update clock time in the background
    seconds++;
    if (seconds == 60) {
        seconds = 0;
        minutes++;
        if (minutes == 60) {
            minutes = 0;
            hours = (hours + 1) % 24; // Wrap around 24-hour format
        }
    }

    // Update stopwatch time if in stopwatch mode
    if (mode == 1) {
        stopwatch_seconds++;
        if (stopwatch_seconds == 60) {
            stopwatch_seconds = 0;
            stopwatch_minutes++;
            if (stopwatch_minutes == 60) {
                stopwatch_minutes = 0;
                stopwatch_hours++;
            }
        }
    }

    // Update timer time if in timer mode
    if (mode == 2) {
        if (timer_seconds == 0 && timer_minutes == 0 && timer_hours == 0) {
            mode = 0; // Switch back to clock mode when timer reaches 0
        } else {
            if (timer_seconds == 0) {
                timer_seconds = 59;
                if (timer_minutes == 0) {
                    timer_minutes = 59;
                    if (timer_hours > 0) {
                        timer_hours--;
                    }
                } else {
                    timer_minutes--;
                }
            } else {
                timer_seconds--;
            }
        }
    }
}

void displayTime(int hours, int minutes, int seconds);
void setBCD(int value);

int main() {
    setup();

    while (1) {
        // Check button presses
        if (!(PINC & (1 << BUTTON_CLOCK_PIN))) { // Button 1 (Clock) pressed
            _delay_ms(50); // Debounce delay
            if (!(PINC & (1 << BUTTON_CLOCK_PIN))) { // Confirm button press
                mode = 0; // Switch to clock mode
            }
        }
        if (!(PINC & (1 << BUTTON_STOPWATCH_PIN))) { // Button 2 (Stopwatch) pressed
            _delay_ms(50); // Debounce delay
            if (!(PINC & (1 << BUTTON_STOPWATCH_PIN))) { // Confirm button press
                mode = 1; // Switch to stopwatch mode
                stopwatch_seconds = 0; // Reset stopwatch
                stopwatch_minutes = 0;
                stopwatch_hours = 0;
            }
        }
        if (!(PIND & (1 << BUTTON_TIMER_PIN))) { // Button 3 (Timer) pressed
            _delay_ms(50); // Debounce delay
            if (!(PIND & (1 << BUTTON_TIMER_PIN))) { // Confirm button press
                mode = 2; // Switch to timer mode
                timer_seconds = 0; // Reset timer
                timer_minutes = 5; // Set timer to 5 minutes
                timer_hours = 0;
            }
        }

        // Display the current mode's time
        if (mode == 0) { // Clock mode
            displayTime(hours, minutes, seconds);
        } else if (mode == 1) { // Stopwatch mode
            displayTime(stopwatch_hours, stopwatch_minutes, stopwatch_seconds);
        } else if (mode == 2) { // Timer mode
            displayTime(timer_hours, timer_minutes, timer_seconds);
        }
    }
}

void displayTime(int hours, int minutes, int seconds) {
    int digits[6] = {
        hours / 10, hours % 10,
        minutes / 10, minutes % 10,
        seconds / 10, seconds % 10
    };

    for (int i = 0; i < 6; i++) {
        if (i < 2) {
            // Activate PD6 or PD7 for the first two displays
            COMMON_PORT_D = (COMMON_PORT_D & ~COMMON_MASK_D) | ((1 << (i + 6)) & COMMON_MASK_D);
            COMMON_PORT_B &= ~COMMON_MASK_B; // Ensure PORTB is off
        } else {
            // Activate PB0, PB1, PB2, or PB3 for the remaining four displays
            COMMON_PORT_B = (COMMON_PORT_B & ~COMMON_MASK_B) | ((1 << (i - 2)) & COMMON_MASK_B);
            COMMON_PORT_D &= ~COMMON_MASK_D; // Ensure PORTD is off
        }
        setBCD(digits[i]);      // Send BCD value to 7447
        _delay_ms(2);           // Short delay for persistence of vision
        COMMON_PORT_D &= ~COMMON_MASK_D; // Turn off PORTD
        COMMON_PORT_B &= ~COMMON_MASK_B; // Turn off PORTB
    }
}

void setBCD(int value) {
    BCD_PORT = (BCD_PORT & ~BCD_MASK) | ((value << 2) & BCD_MASK);
}
