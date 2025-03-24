#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdint.h>

// --- Configuration Constants ---
const uint32_t longPressThreshold = 2000; // 2 seconds for continuous increment
const uint32_t repeatDelay        = 200;  // 200 ms delay for repeat increment
const uint32_t pauseResetHoldTime = 5000; // 5 seconds for pause button reset

// --- Global Timekeeping Variables ---
volatile uint32_t timer_millis = 0;  // Incremented by Timer0 ISR

uint8_t clockHours = 0, clockMinutes = 0, clockSeconds = 0;
uint8_t timerHours = 0, timerMinutes = 0, timerSeconds = 0;
uint32_t lastMillis = 0;
uint32_t clockLastMillis = 0;

// --- Global Flags ---
volatile bool timerMode = false;
volatile bool clockPaused = false;
volatile bool timerPaused = false;
volatile bool timerBlinking = false;  // Blinking when timer hits zero

// --- Timer0 Compare Match Interrupt Service Routine ---
ISR(TIMER0_COMPA_vect) {
    timer_millis++;
}

// Returns the number of milliseconds since the timer started.
uint32_t millis(void) {
    uint32_t ms;
    cli();
    ms = timer_millis;
    sei();
    return ms;
}

void setup(void) {
    // Configure BCD segment output pins (PD2-PD5) as outputs.
    DDRD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5);
    
    // Configure digit enable pins:
    // Seconds digits on PORTD: Digital Pin 7 (PD7) and Digital Pin 6 (PD6).
    DDRD |= (1 << PD6) | (1 << PD7);
    // Minutes and Hours digits on PORTB: Digital Pins 8–11 (PB0-PB3).
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);
    
    // Configure buttons:
    // A0: Hour increment, A1: Minute increment, A2: Second increment,
    // A3: Pause/Play toggle, A5: Timer mode.
    // On ATmega328, A0-A3 map to PC0-PC3, A5 maps to PC5.
    DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC5));
    PORTC |= ((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC5)); // Enable internal pull-ups.
    
    // Initialize output pins to low.
    PORTD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5)); // Clear BCD pins.
    PORTD &= ~((1 << PD6) | (1 << PD7));                           // Clear seconds enable.
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3)); // Clear minutes/hours enable.
    
    // Set up Timer0 for a 1ms tick.
    TCCR0A = (1 << WGM01);              // CTC mode.
    TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64.
    OCR0A = 249;                        // 1ms period.
    TIMSK0 |= (1 << OCIE0A);            // Enable Timer0 Compare Match A.
}

// Displays a digit on a specific position
void displayDigit(uint8_t digit, uint8_t position) {
    // Set the BCD output bits on PD2-PD5.
    PORTD = (PORTD & 0xC3) | ((digit & 0x0F) << PD2);
    
    // Disable all digit enable pins.
    PORTD &= ~((1 << PD6) | (1 << PD7));
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3));

    // Enable the specific digit based on position.
    switch (position) {
        case 0: PORTD |= (1 << PD6); break;  // Hours tens
        case 1: PORTD |= (1 << PD7); break;  // Hours units
        case 2: PORTB |= (1 << PB0); break;  // Minutes tens
        case 3: PORTB |= (1 << PB1); break;  // Minutes units
        case 4: PORTB |= (1 << PB2); break;  // Seconds tens
        case 5: PORTB |= (1 << PB3); break;  // Seconds units
    }
    
    _delay_ms(3);  // Small delay for visibility.
    
    // Disable the digit after the delay.
    switch (position) {
        case 0: PORTD &= ~(1 << PD6); break;
        case 1: PORTD &= ~(1 << PD7); break;
        case 2: PORTB &= ~(1 << PB0); break;
        case 3: PORTB &= ~(1 << PB1); break;
        case 4: PORTB &= ~(1 << PB2); break;
        case 5: PORTB &= ~(1 << PB3); break;
    }
}

// Updates the display with the time of the active mode.
void updateDisplay(void) {
    static bool blinkState = false;       // Tracks blink state.
    static uint32_t lastBlinkMillis = 0;  // Last blink toggle time.

    if (timerMode && timerBlinking) {
        // Blink the display every 500ms when the timer hitszero.
        if (millis() - lastBlinkMillis >= 500) {
            blinkState = !blinkState;  // Toggle blink state.
            lastBlinkMillis = millis();
        }
        
        if (!blinkState) {
            return;  // Skip updating the display to create blink effect.
        }
    }

    if (timerMode) {
        displayDigit(timerHours / 10, 0);
        displayDigit(timerHours % 10, 1);
        displayDigit(timerMinutes / 10, 2);
        displayDigit(timerMinutes % 10, 3);
        displayDigit(timerSeconds / 10, 4);
        displayDigit(timerSeconds % 10, 5);
    } else {
        displayDigit(clockHours / 10, 0);
        displayDigit(clockHours % 10, 1);
        displayDigit(clockMinutes / 10, 2);
        displayDigit(clockMinutes % 10, 3);
        displayDigit(clockSeconds / 10, 4);
        displayDigit(clockSeconds % 10, 5);
    }
}

// Checks buttons to update the time for the active mode.
void checkButtons(void) {
    uint32_t currentMillis = millis();
    
    // --- Hour Button (A0, PC0) ---
    static uint32_t hourHoldStart = 0, hourLastRepeat = 0;
    bool hourButton = (PINC & (1 << PC0)) == 0;
    if (hourButton) {
        if (hourHoldStart == 0) {
            hourHoldStart = currentMillis;
            hourLastRepeat = currentMillis;
        } else if (currentMillis - hourHoldStart >= longPressThreshold &&
                   currentMillis - hourLastRepeat >= repeatDelay) {
            if (timerMode)
                timerHours = (timerHours + 1) % 24;
            else
                clockHours = (clockHours + 1) % 24;
            hourLastRepeat = currentMillis;
        }
    } else if (hourHoldStart != 0) {
        if (currentMillis - hourHoldStart < longPressThreshold) {
            if (timerMode)
                timerHours = (timerHours + 1) % 24;
            else
                clockHours = (clockHours + 1) % 24;
        }
        hourHoldStart = 0;
    }
    
    // --- Minute Button (A1, PC1) ---
    static uint32_t minuteHoldStart = 0, minuteLastRepeat = 0;
    bool minuteButton = (PINC & (1 << PC1)) == 0;
    if (minuteButton) {
        if (minuteHoldStart == 0) {
            minuteHoldStart = currentMillis;
            minuteLastRepeat = currentMillis;
        } else if (currentMillis - minuteHoldStart >= longPressThreshold &&
                   currentMillis - minuteLastRepeat >= repeatDelay) {
            if (timerMode)
                timerMinutes = (timerMinutes + 1) % 60;
            else
                clockMinutes = (clockMinutes + 1) % 60;
            minuteLastRepeat = currentMillis;
        }
    } else if (minuteHoldStart != 0) {
        if (currentMillis - minuteHoldStart < longPressThreshold) {
            if (timerMode)
                timerMinutes = (timerMinutes + 1) % 60;
            else
                clockMinutes = (clockMinutes + 1) % 60;
        }
        minuteHoldStart = 0;
    }
    
    // --- Second Button (A2, PC2) ---
    static uint32_t secondHoldStart = 0, secondLastRepeat = 0;
    bool secondButton = (PINC & (1 << PC2)) == 0;
    if (secondButton) {
        if (secondHoldStart == 0) {
            secondHoldStart = currentMillis;
            secondLastRepeat = currentMillis;
        } else if (currentMillis - secondHoldStart >= longPressThreshold &&
                   currentMillis - secondLastRepeat >= repeatDelay) {
            if (timerMode)
                timerSeconds = (timerSeconds + 1) % 60;
            else
                clockSeconds = (clockSeconds + 1) % 60;
            secondLastRepeat = currentMillis;
        }
    } else if (secondHoldStart != 0) {
        if (currentMillis - secondHoldStart < longPressThreshold) {
            if (timerMode)
                timerSeconds = (timerSeconds + 1) % 60;
            else
                clockSeconds = (clockSeconds + 1) % 60;
        }
        secondHoldStart = 0;
    }
    
    // --- Pause/Play Button (A3, PC3) ---
    static uint32_t pauseHoldStart = 0;
    bool pauseButton = (PINC & (1 << PC3)) == 0;
    if (pauseButton) {
        if (pauseHoldStart == 0) {
            pauseHoldStart = currentMillis;
        } else if (currentMillis - pauseHoldStart >= pauseResetHoldTime) {
            if (timerMode) {
                timerHours = timerMinutes = timerSeconds = 0;
                timerPaused = true;
            } else {
                clockHours = clockMinutes = clockSeconds = 0;
                clockPaused = true;
            }
            pauseHoldStart = 0;
        }
    } else if (pauseHoldStart != 0) {
        if (currentMillis - pauseHoldStart < pauseResetHoldTime) {
            if (timerMode)
                timerPaused = !timerPaused;
            else
                clockPaused = !clockPaused;
        }
        pauseHoldStart = 0;
    }
    
    // --- Timer Mode Button (A5, PC5) ---
    static bool lastTimerState = true;
    bool currentTimerState = (PINC & (1 << PC5)) == 0;
    if (lastTimerState == false && currentTimerState == true) {
        timerMode = !timerMode;
    }
    lastTimerState = currentTimerState;
}

// Main loop
int main(void) {
    setup();
    sei();  // Enable global interrupts.
    
    while (1) {
        uint32_t currentMillis = millis();
        
        // Update clock even in timer mode
        if ((currentMillis - clockLastMillis) >= 1000) {
            clockLastMillis = currentMillis;
            if (!clockPaused) {
                clockSeconds++;
                if (clockSeconds >= 60) {
                    clockSeconds = 0;
                    clockMinutes++;
                }
                if (clockMinutes >= 60) {
                    clockMinutes = 0;
                    clockHours++;
                }
                if (clockHours >= 24) {
                    clockHours = 0;
                }
            }
        }
        
        // Timer update
        if ((currentMillis - lastMillis) >= 1000) {
            lastMillis = currentMillis;
            
            if (timerMode && !timerPaused) {
                if (timerHours == 0 && timerMinutes == 0 && timerSeconds == 0) {
                    timerBlinking = true;  // Start blinking when timer hits zero
                } else {
                    timerBlinking = false;
                    if (timerSeconds > 0)
                        timerSeconds--;
                    else {
                        timerSeconds = 59;
                        if (timerMinutes > 0)
                            timerMinutes--;
                        else {
                            timerMinutes = 59;
                            if (timerHours > 0)
                                timerHours--;
                        }
                    }
                }
            }
        }
        
        checkButtons();
        updateDisplay();
    }
    
    return 0;
}
 
