#define F_CPU 16000000UL  // 16 MHz clock speed
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*******************************************************************
 * PIN DEFINITIONS
 *******************************************************************/
// BCD Pins (A, B, C, D for 7447)
#define A PD2  // BCD Input A to 7447 decoder
#define B PD3  // BCD Input B to 7447 decoder
#define C PD4  // BCD Input C to 7447 decoder
#define D PD5  // BCD Input D to 7447 decoder

// Display Control Pins
#define H1 PB4  // Hours Tens (leftmost digit)
#define H2 PB5  // Hours Units
#define M1 PC0  // Minutes Tens
#define M2 PC1  // Minutes Units
#define S1 PC2  // Seconds Tens
#define S2 PC3  // Seconds Units

// Push Button Pins
#define H_PLUS    (1 << PD6) // D6 - Increase hours button
#define H_MINUS   (1 << PD7) // D7 - Decrease hours button
#define M_PLUS    (1 << PB0) // D8 - Increase minutes button (on PORTB)
#define M_MINUS   (1 << PB1) // D9 - Decrease minutes button (on PORTB)
#define RESET     (1 << PB2) // D10 - Reset clock button (on PORTB)
#define START     (1 << PB3) // D11 - Start/Stop clock button (on PORTB)

/*******************************************************************
 * GLOBAL VARIABLES
 *******************************************************************/
// Clock Variables (BCD Format for direct display)
volatile uint8_t hours = 0b00000000;   // 00 in BCD format (24-hour format)
volatile uint8_t minutes = 0b00000000; // 00 in BCD format
volatile uint8_t seconds = 0b00000000; // 00 in BCD format
volatile uint8_t running = 0;          // Clock running state (0 = stopped, 1 = running)

// Button state tracking for debouncing
volatile uint8_t button_states = 0xFF; // Current debounced button states
volatile uint8_t debounce_counters[6] = {0}; // Counters for button debouncing
#define DEBOUNCE_THRESHOLD 5 // Number of consistent readings required for state change

/*******************************************************************
 * DISPLAY FUNCTIONS
 *******************************************************************/
// Display BCD Value on the active 7-segment display
void displayDigit(uint8_t digit) {
    // Set BCD output pins (A, B, C, D) while preserving other bits in PORTD
    PORTD = (PORTD & 0b11000011) | ((digit & 0b00001111) << 2);
}

// Multiplexed Display Update - Non-blocking version
void displayTime(uint8_t position) {
    static uint8_t digits[6]; // Cache for all 6 display digits
    
    // Only calculate digits when needed (e.g., when time changes or at position 0)
    digits[0] = position == 0 ? (hours >> 4) & 0b00001111 : digits[0];
    digits[1] = position == 0 ? hours & 0b00001111 : digits[1];
    digits[2] = position == 0 ? (minutes >> 4) & 0b00001111 : digits[2];
    digits[3] = position == 0 ? minutes & 0b00001111 : digits[3];
    digits[4] = position == 0 ? (seconds >> 4) & 0b00001111 : digits[4];
    digits[5] = position == 0 ? seconds & 0b00001111 : digits[5];
    
    // Turn off all displays to prevent ghosting
    PORTB &= ~((1 << H1) | (1 << H2));
    PORTC &= ~((1 << M1) | (1 << M2) | (1 << S1) | (1 << S2));
    
    // Display current digit at the selected position
    displayDigit(digits[position]);
    
    // Turn on only the currently active display using boolean logic
    PORTB |= ((!position) << H1) | ((position == 1) << H2);
    PORTC |= ((position == 2) << M1) | ((position == 3) << M2) | 
             ((position == 4) << S1) | ((position == 5) << S2);
}

/*******************************************************************
 * TIMER INTERRUPT HANDLERS
 *******************************************************************/
// Timer1 Interrupt (1 Second) - Handles time keeping
ISR(TIMER1_COMPA_vect) {
    // Skip time counting when clock is stopped
    uint8_t should_update = running;
    
    // Increment seconds when running
    seconds += should_update;
    
    // BCD adjustment for seconds (when units digit > 9)
    uint8_t need_adjust = ((seconds & 0b00001111) > 0b1001) & should_update;
    seconds = (seconds & ~(0b00001111 * need_adjust)) + (0b00010000 * need_adjust);
    
    // Check for 60 seconds (roll over to minutes)
    uint8_t rollover_seconds = (seconds >= 0b01100000) & should_update;
    seconds = seconds * (!rollover_seconds);
    minutes += rollover_seconds;
    
    // BCD adjustment for minutes
    uint8_t need_adjust_min = ((minutes & 0b00001111) > 0b1001) & rollover_seconds;
    minutes = (minutes & ~(0b00001111 * need_adjust_min)) + (0b00010000 * need_adjust_min);
    
    // Check for 60 minutes (roll over to hours)
    uint8_t rollover_minutes = (minutes >= 0b01100000) & rollover_seconds;
    minutes = minutes * (!rollover_minutes);
    hours += rollover_minutes;
    
    // BCD adjustment for hours
    uint8_t need_adjust_hr = ((hours & 0b00001111) > 0b1001) & rollover_minutes;
    hours = (hours & ~(0b00001111 * need_adjust_hr)) + (0b00010000 * need_adjust_hr);
    
    // Check for 24 hours (24-hour format rolls over to 00)
    uint8_t reset_hours = (hours >= 0b00100100) & rollover_minutes;
    hours = hours * (!reset_hours);
}

// Timer0 for display multiplexing and button scanning (runs at ~1kHz)
ISR(TIMER0_COMPA_vect) {
    static uint8_t display_pos = 0;
    
    // Update one digit at a time (multiplexing)
    displayTime(display_pos);
    display_pos = (display_pos + 1) % 6; // Cycle through all 6 positions
    
    // Scan buttons and apply debouncing algorithm
    uint8_t current_buttons_low = PIND & 0xC0;  // Read PD6-PD7 (D6-D7)
    uint8_t current_buttons_high = PINB & 0x0F; // Read PB0-PB3 (D8-D11)
    
    // Combine button states into a single value for processing
    uint8_t current_buttons = current_buttons_low | (current_buttons_high << 8);
    uint8_t changed_buttons = current_buttons ^ button_states;
    
    // Increment debounce counters for buttons that changed state
    for (uint8_t i = 0; i < 6; i++) {
        uint8_t button_bit = (i < 2) ? (1 << (i + 6)) : (1 << ((i - 2) + 8));
        uint8_t state_changed = !!(changed_buttons & button_bit);
        
        debounce_counters[i] = (debounce_counters[i] + state_changed) * state_changed;
        
        uint8_t threshold_reached = (debounce_counters[i] >= DEBOUNCE_THRESHOLD);
        button_states = (button_states & ~(button_bit * threshold_reached)) | 
                        ((current_buttons & button_bit) * threshold_reached);
    }
}

/*******************************************************************
 * BUTTON HANDLING FUNCTIONS
 *******************************************************************/
// Check if button is pressed and released (returns 1 on button release)
uint8_t checkButton(uint8_t button_mask) {
    static uint8_t last_button_state = 0xFF; // Tracks previous button states
    uint8_t current_state = button_states & button_mask;
    
    // Detect button release (button was pressed, now released)
    uint8_t result = !(last_button_state & button_mask) & !!(current_state & button_mask);
    
    // Update last state for next call
    last_button_state = (last_button_state & ~button_mask) | (current_state & button_mask);
    return result;
}

// Handle Button Inputs - Non-blocking
void handleButtons() {
    // Hours Increment Button (H+)
    uint8_t h_plus_pressed = checkButton(H_PLUS);
    hours += h_plus_pressed;
    
    // BCD adjustment
    uint8_t need_adjust_h = ((hours & 0b00001111) > 0b1001) & h_plus_pressed;
    hours = (hours & ~(0b00001111 * need_adjust_h)) + (0b00010000 * need_adjust_h);
    
    // Check for 24 hours (24-hour format)
    uint8_t reset_hours = (hours >= 0b00100100) & h_plus_pressed;
    hours = hours * (!reset_hours);
    
    // Hours Decrement Button (H-)
    uint8_t h_minus_pressed = checkButton(H_MINUS);
    
    // Handle wrap around and BCD adjustment
    uint8_t at_zero = (hours == 0b00000000) & h_minus_pressed;
    uint8_t units_zero = ((hours & 0b00001111) == 0) & h_minus_pressed & !at_zero;
    uint8_t normal_case = h_minus_pressed & !at_zero & !units_zero;
    
    hours = (0b00100011 * at_zero) | 
            (((hours & 0b11110000) - (0b00010000 * units_zero)) | (0b00001001 * units_zero)) |
            (hours * (!h_minus_pressed)) |
            ((hours - 1) * normal_case);
    
    // Minutes Increment Button (M+)
    uint8_t m_plus_pressed = checkButton(M_PLUS);
    minutes += m_plus_pressed;
    
    // BCD adjustment for minutes
    uint8_t need_adjust_m = ((minutes & 0b00001111) > 0b1001) & m_plus_pressed;
    minutes = (minutes & ~(0b00001111 * need_adjust_m)) + (0b00010000 * need_adjust_m);
    
    // Check for 60 minutes
    uint8_t reset_minutes = (minutes >= 0b01100000) & m_plus_pressed;
    minutes = minutes * (!reset_minutes);
    
    // Increment hours when minutes roll over
    hours += reset_minutes;
    
    // BCD adjustment for hours when minutes roll over
    uint8_t need_adjust_h_from_m = ((hours & 0b00001111) > 0b1001) & reset_minutes;
    hours = (hours & ~(0b00001111 * need_adjust_h_from_m)) + (0b00010000 * need_adjust_h_from_m);
    
    // Check for 24 hours after minutes roll over
    uint8_t reset_hours_from_m = (hours >= 0b00100100) & reset_minutes;
    hours = hours * (!reset_hours_from_m);
    
    // Minutes Decrement Button (M-)
    uint8_t m_minus_pressed = checkButton(M_MINUS);
    
    // Complex minute decrement with hour adjustment logic
    uint8_t min_units_zero = ((minutes & 0b00001111) == 0) & m_minus_pressed;
    uint8_t min_all_zero = ((minutes & 0b11110000) == 0) & min_units_zero;
    uint8_t normal_min_case = m_minus_pressed & !min_units_zero;
    uint8_t min_bcd_adjust = min_units_zero & !min_all_zero;
    
    // Handle wrap and decrement logic
    minutes = (0b01011001 * min_all_zero) | 
              (((minutes & 0b11110000) - (0b00010000 * min_bcd_adjust)) | (0b00001001 * min_bcd_adjust)) |
              ((minutes - 1) * normal_min_case) |
              (minutes * (!m_minus_pressed));
    
    // Hour adjustments from minute rollunder
    uint8_t hr_at_zero = (hours == 0b00000000) & min_all_zero;
    uint8_t hr_units_zero = ((hours & 0b00001111) == 0) & min_all_zero & !hr_at_zero;
    uint8_t hr_normal_case = min_all_zero & !hr_at_zero & !hr_units_zero;
    
    hours = (0b00100011 * hr_at_zero) | 
            (((hours & 0b11110000) - (0b00010000 * hr_units_zero)) | (0b00001001 * hr_units_zero)) |
            ((hours - 1) * hr_normal_case) |
            (hours * (!min_all_zero));
    
    // Reset Button - Set clock to 00:00:00
    uint8_t reset_pressed = checkButton(RESET);
    hours = hours * (!reset_pressed);
    minutes = minutes * (!reset_pressed);
    seconds = seconds * (!reset_pressed);
    
    // Start/Stop Button - Toggle clock running state
    running ^= checkButton(START); // Toggle running state (0 to 1 or 1 to 0)
}

/*******************************************************************
 * TIMER INITIALIZATION FUNCTIONS
 *******************************************************************/
// Timer1 Setup for 1-second interrupts (time keeping)
void timer1_init() {
    // Set CTC mode and 1024 prescaler
    TCCR1A = 0;  // Normal operation
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);  // CTC mode, 1024 prescaler
    OCR1A = 15624;  // For 1 second with 16MHz crystal (16000000/1024/1Hz - 1)
    TIMSK1 |= (1 << OCIE1A);  // Enable compare match interrupt
}

// Timer0 Setup for display multiplexing and button scanning (~1kHz)
void timer0_init() {
    // Set CTC mode and 64 prescaler
    TCCR0A = (1 << WGM01);  // CTC mode
    TCCR0B = (1 << CS01) | (1 << CS00);  // 64 prescaler
    OCR0A = 249;  // For ~1kHz interrupt rate (16MHz/64/1kHz - 1)
    TIMSK0 |= (1 << OCIE0A);  // Enable compare match interrupt
}

/*******************************************************************
 * MAIN FUNCTION
 *******************************************************************/
int main(void) {
    // Configure I/O pins
    DDRD |= (1 << A) | (1 << B) | (1 << C) | (1 << D);  // BCD outputs
    DDRB |= (1 << H1) | (1 << H2);  // Hours display control (now on PORTB)
    DDRC |= (1 << M1) | (1 << M2) | (1 << S1) | (1 << S2);  // Minutes and seconds display control (now on PORTC)
    
    // Configure button inputs with pull-ups enabled
    DDRD &= ~(H_PLUS | H_MINUS); // Set PD6-PD7 as inputs
    PORTD |= (H_PLUS | H_MINUS); // Enable pull-ups for PD6-PD7
    
    // For buttons on PORTB (PB0-PB3)
    DDRB &= ~(M_PLUS | M_MINUS | RESET | START); // Set PB0-PB3 as inputs 
    PORTB |= (M_PLUS | M_MINUS | RESET | START); // Enable pull-ups for PB0-PB3
    
    // Initialize timers
    timer0_init();  // For display multiplexing (~1kHz)
    timer1_init();  // For 1-second time keeping
    
    // Enable global interrupts
    sei();
    
    // Main loop - most work is done in interrupt handlers
    while (1) {
        handleButtons(); // Check for button presses
    }
    
    return 0;
}