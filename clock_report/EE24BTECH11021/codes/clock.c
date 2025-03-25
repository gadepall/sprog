#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

/*------------------ Millis Timer ------------------*/
volatile unsigned long millis_count = 0;
ISR(TIMER0_COMPA_vect) {
    millis_count++;
}

unsigned long currentMillis(void) {
    unsigned long m;
    cli();
    m = millis_count;
    sei();
    return m;
}

/*------------------ Button Definitions ------------------*/
#define NUM_BUTTONS 7
volatile uint8_t *buttonPinRegs[NUM_BUTTONS] = { &PINB, &PINB, &PINC, &PINC, &PINC, &PIND, &PINC };
uint8_t buttonBits[NUM_BUTTONS] = { 0, 1, 5, 3, 4, 0, 2 };

unsigned long lastButtonPress[NUM_BUTTONS] = {0};
volatile uint8_t buttonState = 0xFF;
const unsigned long debounceDelay = 200;

/*------------------ Fast Adjustment Parameters ------------------*/
const unsigned long HOLD_DELAY = 1000;    // 1 second to activate fast adjust
const unsigned long REPEAT_INTERVAL = 200; // 200ms between fast adjustments

/*------------------ Decoder and Display Definitions ------------------*/
volatile uint8_t *decoderPort = &PORTD;
const uint8_t decoderBits[4] = { 2, 5, 4, 3 };

volatile uint8_t *displayPortRegs[6] = { &PORTB, &PORTB, &PORTC, &PORTC, &PORTB, &PORTD };
const uint8_t displayBits[6] = { 5, 4, 1, 0, 3, 7 };

/*------------------ Time Variables ------------------*/
volatile uint8_t hours = 23;
volatile uint8_t minutes = 59;
volatile uint8_t seconds = 50;

/*------------------ Mode Flags ------------------*/
volatile unsigned long previousMillisTime = 0;
const unsigned long interval = 1000;
volatile bool timerMode = false;
volatile bool clockHalted = false;

/*------------------ New Variables for Reset/Restore ------------------*/
volatile bool resetDone = false;
volatile uint8_t prevHours = 0, prevMinutes = 0, prevSeconds = 0;

/*------------------ Function Prototypes ------------------*/
void checkButtons(void);
void incrementHour(void);
void decrementHour(void);
void incrementMinute(void);
void decrementMinute(void);
void incrementSecond(void);
void toggleTimerMode(void);
void resetClock(void);
void incrementTime(void);
void decrementTime(void);
void displayTime(void);
void showDigit(uint8_t digit, uint8_t displayIndex);

/*------------------ Main Function ------------------*/
int main(void) {
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS01) | (1 << CS00);
    OCR0A = 249;
    TIMSK0 = (1 << OCIE0A);
    sei();

    /* Set up decoder and display outputs */
    DDRD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5);
    DDRB |= (1 << PB5) | (1 << PB4) | (1 << PB3);
    DDRC |= (1 << PC1) | (1 << PC0);
    DDRD |= (1 << PD7);

    /* Set up button inputs with pull-ups */
    DDRB &= ~((1 << PB0) | (1 << PB1));  // hourInc on PB0, hourDec on PB1
    PORTB |= (1 << PB0) | (1 << PB1);
    DDRC &= ~((1 << PC5) | (1 << PC3) | (1 << PC4) | (1 << PC2)); // minInc (PC5), minDec (PC3), timerMode (PC4), reset (PC2)
    PORTC |= (1 << PC5) | (1 << PC3) | (1 << PC4) | (1 << PC2);
    DDRD &= ~(1 << PD0);  // secInc on PD0
    PORTD |= (1 << PD0);

    while (1) {
        unsigned long now = currentMillis();
        checkButtons();

        if (!clockHalted && (now - previousMillisTime >= interval)) {
            previousMillisTime = now;
            if (timerMode) {
                decrementTime();
                if (hours == 0 && minutes == 0 && seconds == 0) {
                    clockHalted = true;
                }
            } else {
                incrementTime();
            }
        }

        displayTime();
    }
    return 0;
}

/*------------------ Button Handling ------------------*/
void checkButtons(void) {
    static unsigned long holdStart[NUM_BUTTONS] = {0};
    static unsigned long lastFastRepeat[NUM_BUTTONS] = {0};
    void (*actions[])() = {incrementHour, decrementHour, incrementMinute,
                          decrementMinute, toggleTimerMode, incrementSecond, resetClock};

    uint8_t currentState = 0;
    unsigned long now = currentMillis();

    // Read current button states into bitfield (active LOW = 1)
    for(uint8_t i = 0; i < NUM_BUTTONS; i++) {
        currentState |= (((*buttonPinRegs[i] & (1 << buttonBits[i])) ? 0 : 1) << i);
    }

    // Detect initial button presses
    uint8_t changes = buttonState ^ currentState;
    uint8_t presses = changes & currentState;

    // Handle initial button presses for buttons 0-6
    for(uint8_t i = 0; i < NUM_BUTTONS; i++) {
        if((presses & (1 << i)) && (now - lastButtonPress[i] > debounceDelay)) {
            lastButtonPress[i] = now;
            actions[i]();
        }
    }

    // Handle fast repeat for adjustment buttons (skip timerMode and reset)
    for(uint8_t i = 0; i < NUM_BUTTONS; i++) {
        if(i == 4 || i == 6) continue;
        if(currentState & (1 << i)) {
            if(holdStart[i] == 0) {
                holdStart[i] = now;
            } else if(now - holdStart[i] > HOLD_DELAY) {
                if(now - lastFastRepeat[i] > REPEAT_INTERVAL) {
                    actions[i]();
                    lastFastRepeat[i] = now;
                }
            }
        } else {
            holdStart[i] = 0;
            lastFastRepeat[i] = 0;
        }
    }

    // For this code, the reset function is handled by the reset button (index 6) in actions[]
    // The toggleTimerMode is index 4.
    
    // Update the stored button state
    buttonState = currentState;
}

/*------------------ Time Updates ------------------*/
void incrementTime(void) {
    seconds++;
    if(seconds > 59) {
        seconds = 0;
        minutes++;
        if(minutes > 59) {
            minutes = 0;
            hours++;
            if(hours > 23) {
                hours = 0;
            }
        }
    }
}

void decrementTime(void) {
    if(hours == 0 && minutes == 0 && seconds == 0) {
        clockHalted = true;
        return;
    }
    
    if(seconds == 0) {
        seconds = 59;
        if(minutes == 0) {
            minutes = 59;
            if(hours == 0) {
                hours = 23;
            } else {
                hours--;
            }
        } else {
            minutes--;
        }
    } else {
        seconds--;
    }
}

/*------------------ Adjustment Functions ------------------*/
void incrementHour(void) { 
    hours++; 
    if(hours > 23) hours = 0;
}

void decrementHour(void) { 
    if(hours == 0) hours = 23;
    else hours--;
}

void incrementMinute(void) { 
    minutes++; 
    if(minutes > 59) minutes = 0;
}

void decrementMinute(void) { 
    if(minutes == 0) minutes = 59;
    else minutes--;
}

void incrementSecond(void) { 
    seconds++; 
    if(seconds > 59) seconds = 0;
}

void toggleTimerMode(void) {
    timerMode = !timerMode;
    clockHalted = false;
    previousMillisTime = currentMillis();
}

/*------------------ Revised Reset Function ------------------*/
/* On first press, store current time and set clock to 0.
   On second press, restore previous time. */
void resetClock(void) {
    static bool alreadyReset = false;
    static uint8_t savedHours = 0, savedMinutes = 0, savedSeconds = 0;

    if (!alreadyReset) {
        savedHours = hours;
        savedMinutes = minutes;
        savedSeconds = seconds;
        hours = 0;
        minutes = 0;
        seconds = 0;
        alreadyReset = true;
        clockHalted = true;
    } else {
        hours = savedHours;
        minutes = savedMinutes;
        seconds = savedSeconds;
        alreadyReset = false;
        clockHalted = false;
        previousMillisTime = currentMillis();
    }
}

/*------------------ Pause/Play Function ------------------*/
void togglePausePlay(void) {
    // Not used in checkButtons here because pause/play is not in our actions[].
    // If you want to add a dedicated pause button, you can do so in checkButtons.
}

/*------------------ Display Functions ------------------*/
void displayTime(void) {
    uint8_t digits[6];
    
    // Calculate tens and ones for hours, minutes, seconds without using / or %
    uint8_t temp = hours, tens = 0;
    while(temp >= 10) { temp -= 10; tens++; }
    digits[0] = tens;
    digits[1] = temp;

    temp = minutes; tens = 0;
    while(temp >= 10) { temp -= 10; tens++; }
    digits[2] = tens;
    digits[3] = temp;

    temp = seconds; tens = 0;
    while(temp >= 10) { temp -= 10; tens++; }
    digits[4] = tens;
    digits[5] = temp;

    for(uint8_t i = 0; i < 6; i++) {
        showDigit(digits[i], i);
        _delay_ms(1);
    }
}

void showDigit(uint8_t digit, uint8_t displayIndex) {
    // Clear decoder pins on PORTD: PD2, PD5, PD4, PD3
    *decoderPort &= ~((1 << PD2) | (1 << PD5) | (1 << PD4) | (1 << PD3));
    
    // Set decoder pins based on digit's binary value using boolean logic
    if(digit & 0x01) *decoderPort |= (1 << PD2);
    if(digit & 0x02) *decoderPort |= (1 << PD5);
    if(digit & 0x04) *decoderPort |= (1 << PD4);
    if(digit & 0x08) *decoderPort |= (1 << PD3);

    // Turn off all displays first
    PORTB &= ~((1 << PB5) | (1 << PB4) | (1 << PB3));
    PORTC &= ~((1 << PC1) | (1 << PC0));
    PORTD &= ~(1 << PD7);

    // Enable the current display based on displayIndex
    switch(displayIndex) {
        case 0: PORTB |= (1 << PB5); break;
        case 1: PORTB |= (1 << PB4); break;
        case 2: PORTC |= (1 << PC1); break;
        case 3: PORTC |= (1 << PC0); break;
        case 4: PORTB |= (1 << PB3); break;
        case 5: PORTD |= (1 << PD7); break;
    }
}
