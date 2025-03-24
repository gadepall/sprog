#include <avr/io.h> 
#define F_CPU 16000000
#include <util/delay.h> 
#include <stdlib.h> 
#include <string.h>
#include <avr/eeprom.h>
#define cbi(x,y) x &= ~_BV(y) // equivalent to cbi(x,y)
#define sbi(x,y) x |= _BV(y) // equivalent to sbi(x,y)

#define EEPROM_W1 0
#define EEPROM_X1 2
#define EEPROM_Y1 4
#define EEPROM_Z1 6
#define EEPROM_A1 8
#define EEPROM_B1 10
#define EEPROM_C1 12
#define EEPROM_D1 14

#define EEPROM_W2 16
#define EEPROM_X2 18
#define EEPROM_Y2 20
#define EEPROM_Z2 22
#define EEPROM_A2 24
#define EEPROM_B2 26
#define EEPROM_C2 28
#define EEPROM_D2 30

#define EEPROM_W3 32
#define EEPROM_X3 34
#define EEPROM_Y3 36
#define EEPROM_Z3 38
#define EEPROM_A3 40
#define EEPROM_B3 42
#define EEPROM_C3 44
#define EEPROM_D3 46

#define EEPROM_W4 48
#define EEPROM_X4 50
#define EEPROM_Y4 52
#define EEPROM_Z4 54
#define EEPROM_A4 56
#define EEPROM_B4 58
#define EEPROM_C4 60
#define EEPROM_D4 62

#define EEPROM_W5 64
#define EEPROM_X5 66
#define EEPROM_Y5 68
#define EEPROM_Z5 70
#define EEPROM_A5 72
#define EEPROM_B5 74
#define EEPROM_C5 76
#define EEPROM_D5 78

#define EEPROM_W6 80
#define EEPROM_X6 82
#define EEPROM_Y6 84
#define EEPROM_Z6 86
#define EEPROM_A6 88
#define EEPROM_B6 90
#define EEPROM_C6 92
#define EEPROM_D6 94

// Continue this pattern for all variables


int W1 = 0,X1=0, Y1 = 0, Z1 = 0;
int W2 = 0,X2=0, Y2 = 0, Z2 = 0;
int W3 = 0,X3=0, Y3 = 0, Z3 = 0;
int W4 = 0,X4=0, Y4 = 0, Z4 = 0;
int W5 = 0,X5=0, Y5 = 0, Z5 = 0;
int W6 = 0,X6=0, Y6 = 0, Z6 = 0;

int A1 = 0, B1= 0, C1=0, D1= 0;
int A2 = 0, B2= 0, C2=0, D2= 0;
int A3 = 0, B3= 0, C3=0, D3= 0;
int A4 = 0, B4= 0, C4=0, D4= 0;
int A5 = 0, B5= 0, C5=0, D5= 0;
int A6 = 0, B6= 0, C6=0, D6= 0;
int temp1= 0,temp2;

void onedisp(int d, int c, int b, int a)
{
  PORTB = 0x01;
  
  if(a==1) sbi(PORTD, 4);
  else cbi(PORTD, 4);
  if(b==1) sbi(PORTD, 5);
  else cbi(PORTD, 5);

  if(c==1) sbi(PORTD, 6);
  else cbi(PORTD, 6);

  if(d==1) sbi(PORTD, 7);
  else cbi(PORTD, 7);
}
void twodisp(int d, int c, int b, int a)
{
  PORTB = 0x02;
  
  if(a==1) sbi(PORTD, 4);
  else cbi(PORTD, 4);
  if(b==1) sbi(PORTD, 5);
  else cbi(PORTD, 5);

  if(c==1) sbi(PORTD, 6);
  else cbi(PORTD, 6);

  if(d==1) sbi(PORTD, 7);
  else cbi(PORTD, 7);
}
void threedisp(int d, int c, int b, int a)
{
  PORTB = 0x04;
  
  if(a==1) sbi(PORTD, 4);
  else cbi(PORTD, 4);
  if(b==1) sbi(PORTD, 5);
  else cbi(PORTD, 5);

  if(c==1) sbi(PORTD, 6);
  else cbi(PORTD, 6);

  if(d==1) sbi(PORTD, 7);
  else cbi(PORTD, 7);
}
void fourdisp(int d, int c, int b, int a)
{
  PORTB = 0x08;
  
  if(a==1) sbi(PORTD, 4);
  else cbi(PORTD, 4);
  if(b==1) sbi(PORTD, 5);
  else cbi(PORTD, 5);

  if(c==1) sbi(PORTD, 6);
  else cbi(PORTD, 6);

  if(d==1) sbi(PORTD, 7);
  else cbi(PORTD, 7);
}
void fivedisp(int d, int c, int b, int a)
{
  PORTB = 0x10;
  
  if(a==1) sbi(PORTD, 4);
  else cbi(PORTD, 4);
  if(b==1) sbi(PORTD, 5);
  else cbi(PORTD, 5);

  if(c==1) sbi(PORTD, 6);
  else cbi(PORTD, 6);

  if(d==1) sbi(PORTD, 7);
  else cbi(PORTD, 7);
}
void sixdisp(int d, int c, int b, int a)
{
  PORTB = 0x20;
  
  if(a==1) sbi(PORTD, 4);
  else cbi(PORTD, 4);
  if(b==1) sbi(PORTD, 5);
  else cbi(PORTD, 5);

  if(c==1) sbi(PORTD, 6);
  else cbi(PORTD, 6);

  if(d==1) sbi(PORTD, 7);
  else cbi(PORTD, 7);
}

void seconds(void){
   A1= (!W1);
   B1 = (W1&&!X1&&!Z1)||(!W1&&X1);
   C1= (!X1&&Y1)||(!W1&&Y1)||(W1&&X1&&!Y1);
   D1 = (!W1&&Z1)||(W1&&X1&&Y1);
   temp1 = W1 && !X1 && !Y1 && Z1;
   A2 = (!W2);
   B2 = (!Y2&&!X2 && W2)||(!W2 && X2);
   C2 = (!W2&&Y2)||(X2 && W2);
   D2 = 0;
   A2 = (temp1&&A2) || (!temp1&&W2);
   B2 = (temp1&&B2) || (!temp1&&X2);
   C2 = (temp1&&C2) || (!temp1&&Y2);
   D2 = (temp1&&D2) || (!temp1&&Z2);
   temp1 = temp1 && W2 && !X2 && Y2 && !Z2;
}

void minutes(void){
   A3= (!W3);
   B3 = (W3&&!X3&&!Z3)||(!W3&&X3);
   C3= (!X3&&Y3)||(!W3&&Y3)||(W3&&X3&&!Y3);
   D3 = (!W3&&Z3)||(W3&&X3&&Y3);
   A3 = (temp1&&A3) || (!temp1&&W3);
   B3 = (temp1&&B3) || (!temp1&&X3);
   C3 = (temp1&&C3) || (!temp1&&Y3);
   D3 = (temp1&&D3) || (!temp1&&Z3);
   temp1 = temp1 && W3 && !X3 && !Y3 && Z3;
   
   A4 = (!W4);
   B4 = (!Y4&&!X4 && W4)||(!W4 && X4);
   C4 = (!W4&&Y4)||(X4 && W4);
   D4 = 0;
   A4 = (temp1&&A4) || (!temp1&&W4);
   B4 = (temp1&&B4) || (!temp1&&X4);
   C4 = (temp1&&C4) || (!temp1&&Y4);
   D4 = (temp1&&D4) || (!temp1&&Z4);
   
   temp1 = temp1 && W4 && !X4 && Y4 && !Z4;
}
void hours(void){
   A5= (!W5);
   B5 = (W5&&!X5&&!Z5)||(!W5&&X5);
   C5= (!X5&&Y5)||(!W5&&Y5)||(W5&&X5&&!Y5);
   D5 = (!W5&&Z5)||(W5&&X5&&Y5);
   A5 = (temp1&&A5) || (!temp1&&W5);
   B5 = (temp1&&B5) || (!temp1&&X5);
   C5 = (temp1&&C5) || (!temp1&&Y5);
   D5 = (temp1&&D5) || (!temp1&&Z5);
   
   temp1 = temp1 && W5 && !X5 && !Y5 && Z5;
   A6= (!W6);
   B6= (W6&&!X6&&!Z6)||(!W6&&X6);
   C6= (!X6&&Y6)||(!W6&&Y6)||(W6&&X6&&!Y6);
   D6 = (!W6&&Z6)||(W6&&X6&&Y6);
   A6 = (temp1&&A6) || (!temp1&&W6);
   B6 = (temp1&&B6) || (!temp1&&X6);
   C6 = (temp1&&C6) || (!temp1&&Y6);
   D6 = (temp1&&D6) || (!temp1&&Z6);
}

void checker(void){
   temp1 = !A1 && !B1 && !C1 && !D1 &&!A2 && !B2 && !C2 && !D2 && !W6 && X6 && !Y6 && !Z6 && W5 && X5 && !Y5 && !Z5 && W4 && !X4 && Y4 && !Z4 && W3 && !X3 && !Y3 && Z3;
   A6 = (!temp1)&&A6;
   B6 = (!temp1)&&B6;
   C6 = (!temp1)&&C6;
   D6 = (!temp1)&&D6;
   A5 = (!temp1)&&A5;
   B5 = (!temp1)&&B5;
   C5 = (!temp1)&&C5;
   D5 = (!temp1)&&D5;
   A4 = (!temp1)&&A4;
   B4 = (!temp1)&&B4;
   C4 = (!temp1)&&C4;
   D4 = (!temp1)&&D4;
   A3 = (!temp1)&&A3;
   B3 = (!temp1)&&B3;
   C3 = (!temp1)&&C3;
   D3 = (!temp1)&&D3;
   A2 = (!temp1)&&A2;
   B2 = (!temp1)&&B2;
   C2 = (!temp1)&&C2;
   D2 = (!temp1)&&D2;
   A1 = (!temp1)&&A1;
   B1 = (!temp1)&&B1;
   C1 = (!temp1)&&C1;
   D1 = (!temp1)&&D1;
}
void increment(void){
  seconds();
  minutes();
  hours();
  checker();
}
void update(void){
    W1 = A1; X1 = B1; Y1 = C1; Z1 = D1;
    W2 = A2; X2 = B2; Y2 = C2; Z2 = D2;
    W3 = A3; X3 = B3; Y3 = C3; Z3 = D3;
    W4 = A4; X4 = B4; Y4 = C4; Z4 = D4;
    W5 = A5; X5 = B5; Y5 = C5; Z5 = D5;
    W6 = A6; X6 = B6; Y6 = C6; Z6 = D6;
}
void saveToEEPROM() {
    eeprom_update_word((uint16_t*)EEPROM_W1, W1);
    eeprom_update_word((uint16_t*)EEPROM_X1, X1);
    eeprom_update_word((uint16_t*)EEPROM_Y1, Y1);
    eeprom_update_word((uint16_t*)EEPROM_Z1, Z1);
    eeprom_update_word((uint16_t*)EEPROM_A1, A1);
    eeprom_update_word((uint16_t*)EEPROM_B1, B1);
    eeprom_update_word((uint16_t*)EEPROM_C1, C1);
    eeprom_update_word((uint16_t*)EEPROM_D1, D1);

    eeprom_update_word((uint16_t*)EEPROM_W2, W2);
    eeprom_update_word((uint16_t*)EEPROM_X2, X2);
    eeprom_update_word((uint16_t*)EEPROM_Y2, Y2);
    eeprom_update_word((uint16_t*)EEPROM_Z2, Z2);
    eeprom_update_word((uint16_t*)EEPROM_A2, A2);
    eeprom_update_word((uint16_t*)EEPROM_B2, B2);
    eeprom_update_word((uint16_t*)EEPROM_C2, C2);
    eeprom_update_word((uint16_t*)EEPROM_D2, D2);

    eeprom_update_word((uint16_t*)EEPROM_W3, W3);
    eeprom_update_word((uint16_t*)EEPROM_X3, X3);
    eeprom_update_word((uint16_t*)EEPROM_Y3, Y3);
    eeprom_update_word((uint16_t*)EEPROM_Z3, Z3);
    eeprom_update_word((uint16_t*)EEPROM_A3, A3);
    eeprom_update_word((uint16_t*)EEPROM_B3, B3);
    eeprom_update_word((uint16_t*)EEPROM_C3, C3);
    eeprom_update_word((uint16_t*)EEPROM_D3, D3);

    eeprom_update_word((uint16_t*)EEPROM_W4, W4);
    eeprom_update_word((uint16_t*)EEPROM_X4, X4);
    eeprom_update_word((uint16_t*)EEPROM_Y4, Y4);
    eeprom_update_word((uint16_t*)EEPROM_Z4, Z4);
    eeprom_update_word((uint16_t*)EEPROM_A4, A4);
    eeprom_update_word((uint16_t*)EEPROM_B4, B4);
    eeprom_update_word((uint16_t*)EEPROM_C4, C4);
    eeprom_update_word((uint16_t*)EEPROM_D4, D4);

    eeprom_update_word((uint16_t*)EEPROM_W5, W5);
    eeprom_update_word((uint16_t*)EEPROM_X5, X5);
    eeprom_update_word((uint16_t*)EEPROM_Y5, Y5);
    eeprom_update_word((uint16_t*)EEPROM_Z5, Z5);
    eeprom_update_word((uint16_t*)EEPROM_A5, A5);
    eeprom_update_word((uint16_t*)EEPROM_B5, B5);
    eeprom_update_word((uint16_t*)EEPROM_C5, C5);
    eeprom_update_word((uint16_t*)EEPROM_D5, D5);

    eeprom_update_word((uint16_t*)EEPROM_W6, W6);
    eeprom_update_word((uint16_t*)EEPROM_X6, X6);
    eeprom_update_word((uint16_t*)EEPROM_Y6, Y6);
    eeprom_update_word((uint16_t*)EEPROM_Z6, Z6);
    eeprom_update_word((uint16_t*)EEPROM_A6, A6);
    eeprom_update_word((uint16_t*)EEPROM_B6, B6);
    eeprom_update_word((uint16_t*)EEPROM_C6, C6);
    eeprom_update_word((uint16_t*)EEPROM_D6, D6);
}

void loadFromEEPROM() {
    W1 = eeprom_read_word((uint16_t*)EEPROM_W1);
    X1 = eeprom_read_word((uint16_t*)EEPROM_X1);
    Y1 = eeprom_read_word((uint16_t*)EEPROM_Y1);
    Z1 = eeprom_read_word((uint16_t*)EEPROM_Z1);
    A1 = eeprom_read_word((uint16_t*)EEPROM_A1);
    B1 = eeprom_read_word((uint16_t*)EEPROM_B1);
    C1 = eeprom_read_word((uint16_t*)EEPROM_C1);
    D1 = eeprom_read_word((uint16_t*)EEPROM_D1);

    W2 = eeprom_read_word((uint16_t*)EEPROM_W2);
    X2 = eeprom_read_word((uint16_t*)EEPROM_X2);
    Y2 = eeprom_read_word((uint16_t*)EEPROM_Y2);
    Z2 = eeprom_read_word((uint16_t*)EEPROM_Z2);
    A2 = eeprom_read_word((uint16_t*)EEPROM_A2);
    B2 = eeprom_read_word((uint16_t*)EEPROM_B2);
    C2 = eeprom_read_word((uint16_t*)EEPROM_C2);
    D2 = eeprom_read_word((uint16_t*)EEPROM_D2);

    W3 = eeprom_read_word((uint16_t*)EEPROM_W3);
    X3 = eeprom_read_word((uint16_t*)EEPROM_X3);
    Y3 = eeprom_read_word((uint16_t*)EEPROM_Y3);
    Z3 = eeprom_read_word((uint16_t*)EEPROM_Z3);
    A3 = eeprom_read_word((uint16_t*)EEPROM_A3);
    B3 = eeprom_read_word((uint16_t*)EEPROM_B3);
    C3 = eeprom_read_word((uint16_t*)EEPROM_C3);
    D3 = eeprom_read_word((uint16_t*)EEPROM_D3);

    W4 = eeprom_read_word((uint16_t*)EEPROM_W4);
    X4 = eeprom_read_word((uint16_t*)EEPROM_X4);
    Y4 = eeprom_read_word((uint16_t*)EEPROM_Y4);
    Z4 = eeprom_read_word((uint16_t*)EEPROM_Z4);
    A4 = eeprom_read_word((uint16_t*)EEPROM_A4);
    B4 = eeprom_read_word((uint16_t*)EEPROM_B4);
    C4 = eeprom_read_word((uint16_t*)EEPROM_C4);
    D4 = eeprom_read_word((uint16_t*)EEPROM_D4);

    W5 = eeprom_read_word((uint16_t*)EEPROM_W5);
    X5 = eeprom_read_word((uint16_t*)EEPROM_X5);
    Y5 = eeprom_read_word((uint16_t*)EEPROM_Y5);
    Z5 = eeprom_read_word((uint16_t*)EEPROM_Z5);
    A5 = eeprom_read_word((uint16_t*)EEPROM_A5);
    B5 = eeprom_read_word((uint16_t*)EEPROM_B5);
    C5 = eeprom_read_word((uint16_t*)EEPROM_C5);
    D5 = eeprom_read_word((uint16_t*)EEPROM_D5);

    W6 = eeprom_read_word((uint16_t*)EEPROM_W6);
    X6 = eeprom_read_word((uint16_t*)EEPROM_X6);
    Y6 = eeprom_read_word((uint16_t*)EEPROM_Y6);
    Z6 = eeprom_read_word((uint16_t*)EEPROM_Z6);
    A6 = eeprom_read_word((uint16_t*)EEPROM_A6);
    B6 = eeprom_read_word((uint16_t*)EEPROM_B6);
    C6 = eeprom_read_word((uint16_t*)EEPROM_C6);
    D6 = eeprom_read_word((uint16_t*)EEPROM_D6);
}
void initEEPROM() {
    if (!(eeprom_read_word((uint16_t*)EEPROM_W6) ^ 0xFFFF)) {
        W1 = 0; X1 = 0; Y1 = 0; Z1 = 0;
        A1 = 0; B1 = 0; C1 = 0; D1 = 0;

        W2 = 0; X2 = 0; Y2 = 0; Z2 = 0;
        A2 = 0; B2 = 0; C2 = 0; D2 = 0;

        W3 = 0; X3 = 0; Y3 = 0; Z3 = 0;
        A3 = 0; B3 = 0; C3 = 0; D3 = 0;

        W4 = 0; X4 = 0; Y4 = 0; Z4 = 0;
        A4 = 0; B4 = 0; C4 = 0; D4 = 0;

        W5 = 0; X5 = 0; Y5 = 0; Z5 = 0;
        A5 = 0; B5 = 0; C5 = 0; D5 = 0;

        W6 = 0; X6 = 0; Y6 = 0; Z6 = 0;
        A6 = 0; B6 = 0; C6 = 0; D6 = 0;

        saveToEEPROM(); // Save default values to EEPROM
    }
}

void disp(int n){

for(int i=0;i<n/12;i++){

 onedisp(D1,C1,B1,A1);
 _delay_ms(2);
 twodisp(D2,C2,B2,A2);
 _delay_ms(2);
 threedisp(D3,C3,B3,A3);
 _delay_ms(2);
 fourdisp(D4,C4,B4,A4);
 _delay_ms(2);
 fivedisp(D5,C5,B5,A5);
 _delay_ms(2);
 sixdisp(D6,C6,B6,A6);
 _delay_ms(2);
}
}
int u=1;
void loop(void){
  if(u==1){
    disp(120);
    if(!(PIND & (1 << PD2))){
      for(int i=0;i<60;i++){
        increment();
        update();
      }
    }
    else if(!(PIND & (1 << PD3))){
      for(int i=0;i<3600;i++){
        increment();
        update();
      }
    }
    else if(!(PIND & (1 << PD1))){
      for(int i=0;i<1;i++){
        increment();
        update();
      }
    }
    else if(!(PIND & (1 << PD0))){
      u = 0;
    }
  }
  else{
    for(int i=0;i<10;i++){
    disp(100);
    if(!(PIND & (1 << PD0))){
      u = 1;
    }}
    increment();
    update();
  }
}
int main(void)
{
 TCCR0B = 0x05;
 initEEPROM();
  loadFromEEPROM();
// use PortB for LCD interface
 DDRB = 0xFF; // 1111.1111; set PB0-PB7 as outputs	 
 DDRD = 0xF0;
 //DDRD &= ~((1 << PD1) | (1 << PD2));  // Set PD1 & PD2 as inputs
 PORTD |= (1 << PD1) | (1 << PD2) | (1<<PD3) | (1<<PD0);  // Enable pull-up resistors for PD1 & PD2
 while(1)
 {
  loop();
  saveToEEPROM();
}
}
