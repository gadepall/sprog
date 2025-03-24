#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define PINS_COUNT 6
#define NO_SEGMENTS 7

//Initialising the clock
uint8_t h1 = 0, h2 = 0, m1 = 0, m2 = 0, s1 = 0, s2 = 0;
uint8_t alarm_h1 = 0, alarm_h2 = 0, alarm_m1 = 0, alarm_m2 = 0, alarm_s1 = 0, alarm_s2 = 0;
uint8_t alarm_set = 0, alarm_active = 0, setting_alarm = 0;

#define HOUR_PIN PC1
#define MINUTE_PIN PC2
#define SECOND_PIN PC3
#define ALARM_PIN PC4
#define BUZZER_PIN PC5

volatile unsigned long millis_counter = 0;
uint8_t currentDisplay = 0;
unsigned long lastHourPress = 0, lastMinutePress = 0, lastSecondPress = 0, lastAlarmPress = 0;
unsigned long alarm_start_time = 0;

void timer0_init(void) {
    TCCR0A = 0;
    TCCR0B = (1 << CS01) | (1 << CS00);
    TIMSK0 = (1 << TOIE0);
    sei();
}

ISR(TIMER0_OVF_vect) {
    millis_counter += 1;
}

unsigned long millis(void) {
    return millis_counter;
}

void clearSegments(void) {
    PORTD &= 0b00000011;
    PORTB &= 0b11111110;
}

//Defining the connections of seven segment display to arduino
void sevenseg(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g) {
    clearSegments();
    PORTD |= (a << PD2) | (b << PD3) | (c << PD4) | (d << PD5) | (e << PD6) | (f << PD7);
    if (g) PORTB |= (1 << PB0); else PORTB &= ~(1 << PB0);
}

//Boolean logic for each segment of seven segment display
void segments(uint8_t A,uint8_t B, uint8_t C, uint8_t D){
	uint8_t a=!(A || C || (!B && !D) || (B && D));
	uint8_t b=!(A || (C && D) || !B ||(!C && !D));
	uint8_t c=!(A || B || !C || D);
	uint8_t d=!(A || ( C && !D) || (!B && !D ) || (!B && C) || (B && !C && D));
	uint8_t e=!((C && !D) || (!B && !D ));
	uint8_t f=!(A || (B && !C) || (B && !D ) || (!C && !D));
	uint8_t g=!((C && !D) || (!C && B) || (C && !B) || A);
	sevenseg(a, b, c, d, e, f, g);
}

//BCD for digits 0 to 9
void displayDigit(uint8_t digit) {
    static const uint8_t segment_map[10][4] = {
        {0, 0, 0, 0},
        {0, 0, 0, 1},
        {0, 0, 1, 0},
        {0, 0, 1, 1},
        {0, 1, 0, 0},
        {0, 1, 0, 1},
        {0, 1, 1, 0},
        {0, 1, 1, 1},
        {1, 0, 0, 0},
        {1, 0, 0, 1}
    };
     segments(segment_map[digit][0], segment_map[digit][1], segment_map[digit][2], segment_map[digit][3]);
}

//Function to display time
void displayClock(void) {
    PORTB &= 0b11000001;
    PORTC &= ~(1 << PC0);

    if (currentDisplay == 5) {
        PORTC |= (1 << PC0);
    } else {
        PORTB |= (1 << (currentDisplay + 1));
    }

    uint8_t display_val = (setting_alarm)
                              ? (currentDisplay == 0 ? alarm_h1 : currentDisplay == 1 ? alarm_h2
                                                                                      : currentDisplay == 2 ? alarm_m1
                                                                                                            : currentDisplay == 3 ? alarm_m2
                                                                                                                                  : currentDisplay == 4 ? alarm_s1
                                                                                                                                                        : alarm_s2)
                              : (currentDisplay == 0 ? h1 : currentDisplay == 1 ? h2
                                                                               : currentDisplay == 2 ? m1
                                                                                                     : currentDisplay == 3 ? m2
                                                                                                                           : currentDisplay == 4 ? s1
                                                                                                                                                 : s2);

    displayDigit(display_val);
    currentDisplay = (currentDisplay + 1) % PINS_COUNT;
}

//Function to handle Hour button, Minute button ans seconds button
void handleButtons(void) {
    unsigned long currentMillis = millis();

    // Toggle alarm setting mode
    if (!(PINC & (1 << ALARM_PIN))) {
        if (currentMillis - lastAlarmPress > 300) {
            lastAlarmPress = currentMillis;
            setting_alarm = !setting_alarm;
            if (!setting_alarm) {
                alarm_set = 1;
            }
        }
    }

    // Adjust hours
    if (!(PINC & (1 << HOUR_PIN))) {
        if (currentMillis - lastHourPress > 300) {
            lastHourPress = currentMillis;
            uint8_t *h1_ptr = setting_alarm ? &alarm_h1 : &h1;
            uint8_t *h2_ptr = setting_alarm ? &alarm_h2 : &h2;

            (*h2_ptr)++;
            if (*h2_ptr >= 10) {
                *h2_ptr = 0;
                (*h1_ptr)++;
            }
            if (*h1_ptr >= 2 && *h2_ptr >= 4) {
                *h1_ptr = 0;
                *h2_ptr = 0;
            }
        }
    }

    // Adjust minutes
    if (!(PINC & (1 << MINUTE_PIN))) {
        if (currentMillis - lastMinutePress > 300) {
            lastMinutePress = currentMillis;
            uint8_t *m1_ptr = setting_alarm ? &alarm_m1 : &m1;
            uint8_t *m2_ptr = setting_alarm ? &alarm_m2 : &m2;

            (*m2_ptr)++;
            if (*m2_ptr >= 10) {
                *m2_ptr = 0;
                (*m1_ptr)++;
            }
            if (*m1_ptr >= 6) {
                *m1_ptr = 0;
            }
        }
    }

    // Adjust seconds
    if (!(PINC & (1 << SECOND_PIN))) {
        if (currentMillis - lastSecondPress > 300) {
            lastSecondPress = currentMillis;
            uint8_t *s1_ptr = setting_alarm ? &alarm_s1 : &s1;
            uint8_t *s2_ptr = setting_alarm ? &alarm_s2 : &s2;

            (*s2_ptr)++;
            if (*s2_ptr >= 10) {
                *s2_ptr = 0;
                (*s1_ptr)++;
            }
            if (*s1_ptr >= 6) {
                *s1_ptr = 0;
            }
        }
    }
}

//Function to check the alarm and activate the buzzer for 15 seconds
void checkAlarm(void) {
    static unsigned long last_toggle_time = 0;
    static uint8_t buzzer_state = 0;

    if (alarm_set && h1 == alarm_h1 && h2 == alarm_h2 && m1 == alarm_m1 && m2 == alarm_m2 && s1 == alarm_s1 && s2 == alarm_s2) {
        alarm_active = 1;
        alarm_start_time = millis();
    }

    if (alarm_active) {
        if (millis() - alarm_start_time < 15000) {
            if (millis() - last_toggle_time > 500) {
                last_toggle_time = millis();
                buzzer_state = !buzzer_state;
                if (buzzer_state) {
                    PORTC |= (1 << BUZZER_PIN);
                } else {
                    PORTC &= ~(1 << BUZZER_PIN);
                }
            }
        } else {
            alarm_active = 0;
            PORTC &= ~(1 << BUZZER_PIN);
        }
    }
}

//Function to update seconds minutes and hours in the clock
void updateClock(void) {
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate >= 1000) {
        lastUpdate = millis();
        s2++;
        if (s2 >= 10) { s2 = 0; s1++; }
        if (s1 >= 6) { s1 = 0; m2++; }
        if (m2 >= 10) { m2 = 0; m1++; }
        if (m1 >= 6) { m1 = 0; h2++; }
        if (h2 >= 10) { h2 = 0; h1++; }
        if (h1 == 2 && h2 == 4) { h1 = 0; h2 = 0; }
        checkAlarm();
    }
}

int main(void) {
    DDRD |= 0b11111100;
    DDRB |= 0b00111111;
    DDRC |= (1 << PC0) | (1 << BUZZER_PIN);
    PORTC |= (1 << HOUR_PIN) | (1 << MINUTE_PIN) | (1 << SECOND_PIN) | (1 << ALARM_PIN);

    timer0_init();

    while (1) {
        updateClock();
        handleButtons();
        displayClock();
        _delay_ms(2);
    }
}

