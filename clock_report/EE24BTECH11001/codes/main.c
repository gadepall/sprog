#define F_CPU 16000000UL  // 16 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*
  * digital pin 8 -> f
  * digital pin 9 -> e
  * digital pin 10 -> d
  * digital pin 11 -> c
  * digital pin 12 -> b
  * digital pin 13 -> a
*/
volatile uint8_t flag = 0;
void setup_timer(){
  // Configure Timer1 in CTC mode (Clear Timer on Compare Match)
  TCCR1B |= (1 << WGM12);  // Set CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Prescaler = 1024
  OCR1A = 15624;  // 1-second interval at 16MHz (16,000,000 / 1024 - 1)

  TIMSK1 |= (1 << OCIE1A);  // Enable Timer1 Compare Match A Interrupt
  sei();  // Enable global interrupts
}

ISR(TIMER1_COMPA_vect) {
  flag = 1;  // Set flag to signal main loop that 1 second has passed
}

int main() {
  // SETUP
  DDRD |= 0b00111100; // Set digital pins 2, 3, 4, 5 as output
  DDRB |= 0b00111111; // Set digital pins 8-13 as output

  PORTB &= ~0b00111111; 
  PORTD &= ~0b00111100; 

  int a = 1; // Hours tens
  int b = 7; // Hours ones
  int c = 5; // Minutes tens
  int d = 2; // Minutes ones
  int e = 3; // Seconds tens
  int f = 0; // Seconds ones
  setup_timer();
  while (1) {

    while (1) {
      // Display hours tens
      PORTB &= ~0b00111111;
      PORTB |= 0b00000001;
      PORTD &= ~0b00111100;
      PORTD |= (f << 2);
      _delay_ms(1);

      // Display hours ones
      PORTB &= ~0b00111111;
      PORTB |= 0b00000010;
      PORTD &= ~0b00111100;
      PORTD |= (e << 2);
      _delay_ms(1);

      // Display minutes tens
      PORTB &= ~0b00111111;
      PORTB |= 0b00000100;
      PORTD &= ~0b00111100;
      PORTD |= (d << 2);
      _delay_ms(1);

      // Display minutes ones
      PORTB &= ~0b00111111;
      PORTB |= 0b00001000;
      PORTD &= ~0b00111100;
      PORTD |= (c << 2);
      _delay_ms(1);

      // Display seconds tens
      PORTB &= ~0b00111111;
      PORTB |= 0b00010000;
      PORTD &= ~0b00111100;
      PORTD |= (b << 2);
      _delay_ms(1);

      // Display seconds ones
      PORTB &= ~0b00111111;
      PORTB |= 0b00100000;
      PORTD &= ~0b00111100;
      PORTD |= (a << 2);
      _delay_ms(1);

      if(flag){
        flag = 0;
        break;
      }
    }

    // Increment time
    f++;
    if (f == 10) { f = 0; e++; }
    if (e == 6) { e = 0; d++; }
    if (d == 10) { d = 0; c++; }
    if (c == 6) { c = 0; b++; }
    if (b == 10) { b = 0; a++; }  
    if (a == 2 && b == 4) { a = 0; b = 0; }  // Reset at 24:00
  }
  return 0;
}

