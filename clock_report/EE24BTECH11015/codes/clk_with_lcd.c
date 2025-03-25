#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <LiquidCrystal.h> // Include the LiquidCrystal library

// LCD pin connections
const int rs = 12, en = 13, d4 = A0, d5 = A1, d6 = A2, d7 = A3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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
volatile int hours = 15, minutes = 30, seconds = 10; // Clock time
volatile int stopwatch_seconds = 0, stopwatch_minutes = 0, stopwatch_hours = 0; // Stopwatch time
volatile int timer_hours = 1, timer_minutes = 0, timer_seconds = 0; // Timer set to 5 minutes
volatile int timer_sec = timer_seconds, timer_min = timer_minutes, timer_hour = timer_hours; // Timer reset
volatile int mode = 0; // 0 = Clock, 1 = Stopwatch, 2 = Timer

// Date variables
volatile int day = 22;   // Day (1-31)
volatile int month = 3;  // Month (1-12)
volatile int year = 2025; // Year (e.g., 2022)
volatile int dayOfWeek;  // Day of the week (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
const char* daysOfWeek[] = {"S", "M", "T", "W", "T", "F", "S"}; // Days of the week

// LCD blink variables
volatile int blinkState = 0; // Toggle for blinking the current day

// Function to calculate the day of the week (Zeller's Congruence)
int calculateDayOfWeek(int day, int month, int year) {
    if (month < 3) {
        month += 12;
        year--;
    }
    int h = (day + (13 * (month + 1)) / 5 + year + year / 4 - year / 100 + year / 400) % 7;
    return (h + 6) % 7; // Adjust to make Sunday = 0
}

void setup() {
    // Initialize LCD
    lcd.begin(16, 2); // Set up the LCD's number of columns and rows

    // Calculate the day of the week
    dayOfWeek = calculateDayOfWeek(day, month, year);

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
            if (hours == 0) {
                // Increment day and update day of the week at midnight
                day++;
                if (day > 31) { // Simple day overflow handling (for demonstration)
                    day = 1;
                    month++;
                    if (month > 12) {
                        month = 1;
                        year++;
                    }
                }
                dayOfWeek = (dayOfWeek + 1) % 7; // Increment day of the week
            }
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

    // Toggle blink state every second
    blinkState = !blinkState;
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
                timer_seconds = timer_sec; // Reset timer
                timer_minutes = timer_min; // Set timer to 5 minutes
                timer_hours = timer_hour;
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

        // Update LCD display
        updateLCD();
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

void updateLCD() {
    // Display date in the format "12/04/2022"
    lcd.setCursor(0, 0); // Set cursor to top left
    lcd.print(day / 10);   // Display tens digit of day
    lcd.print(day % 10);   // Display ones digit of day
    lcd.print("/");
    lcd.print(month / 10); // Display tens digit of month
    lcd.print(month % 10); // Display ones digit of month
    lcd.print("/");
    lcd.print(year);       // Display year

    // Display days of the week on the bottom row
    lcd.setCursor(0, 1); // Set cursor to bottom row
    for (int i = 0; i < 7; i++) {
        if (i == dayOfWeek) {
            // Blink the current day
            if (blinkState) {
                lcd.print(daysOfWeek[i]);
            } else {
                lcd.print(" ");
            }
        } else {
            lcd.print(daysOfWeek[i]);
        }
        lcd.print(" "); // Add space between days
    }
}
