#include "tinyexpr.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
// #include <stdlib.h> 

#define ClearBit(x,y) x &= ~_BV(y) // equivalent to cbi(x,y)
#define SetBit(x,y) x |= _BV(y) // equivalent to sbi(x,y)
#define LCD_RS 0 // pin for LCD R/S (eg PB0)
#define LCD_E 1 // pin for LCD enable
#define DAT4 2 // pin for d4
#define DAT5 3 // pin for d5
#define DAT6 4 // pin for d6
#define DAT7 5 // pin for d7
//// The following defines are controller commands
#define CLEARDISPLAY 0x01

typedef uint8_t byte;

void PulseEnableLine ()
{
    SetBit(PORTB,LCD_E); // take LCD enable line high
    _delay_us(40); // wait 40 microseconds
    ClearBit(PORTB,LCD_E); // take LCD enable line low
}
void SendNibble(byte data)
{
    PORTB &= 0xC3; // 1100.0011 = clear 4 data lines
    if (data & _BV(4)) SetBit(PORTB,DAT4);
    if (data & _BV(5)) SetBit(PORTB,DAT5);
    if (data & _BV(6)) SetBit(PORTB,DAT6);
    if (data & _BV(7)) SetBit(PORTB,DAT7);
    PulseEnableLine(); // clock 4 bits into controller
}
void SendByte (byte data)
{
    SendNibble(data); // send upper 4 bits
    SendNibble(data<<4); // send lower 4 bits
    ClearBit(PORTB,5); // turn off boarduino LED
}
void LCD_Cmd (byte cmd)
{
    ClearBit(PORTB,LCD_RS); // R/S line 0 = command data
    SendByte(cmd); // send it
}
void LCD_Char (byte ch)
{
    SetBit(PORTB,LCD_RS); // R/S line 1 = character data
    SendByte(ch); // send it
}
void LCD_Init()
{
    LCD_Cmd(0x33); // initialize controller
    LCD_Cmd(0x32); // set to 4-bit input mode
    LCD_Cmd(0x28); // 2 line, 5x7 matrix
    LCD_Cmd(0x0C); // turn cursor off (0x0E to enable)
    LCD_Cmd(0x06); // cursor direction = right
    LCD_Cmd(0x01); // start with clear display
    _delay_ms(3); // wait for LCD to initialize
}
void LCD_Clear() // clear the LCD display
{
    LCD_Cmd(CLEARDISPLAY);
    _delay_ms(3); // wait for LCD to process command
}

void LCD_Message(const char *text) // display string on LCD
{
    while (*text) // do until /0 character
    LCD_Char(*text++); // send char & update char pointer
}

void LCD_Integer(int data)
// displays the integer value of DATA at current LCD cursor position
{
    char st[8] = ""; // save enough space for result
    // itoa(data,st,10); // 
    sprintf(st, "%d", data);
    LCD_Message(st); // display in on LCD
}

void LCD_Float(float data){
    char st[20] = "";
    sprintf(st, "%.4f", data);
    LCD_Message(st);
}

void eval(const char* expr, float* ans){
    int err;
    *ans = te_interp(expr, &err);
    // if(!err){
    //     LCD_Cmd(0xC0);
    //     LCD_Float(ans);
    // }
    if(err){
        // byte check = 0;
        DDRD = (1<<7);
        while(1){
            if(!(PIND & (1<<5))){
                break;
            }
            LCD_Clear();
            LCD_Cmd(0xC0);
            LCD_Message("Syntax Err.");
            LCD_Cmd(0x80);
            LCD_Message("Press AC to go back");
            _delay_ms(100);
        }
        DDRD = 0x00;
    }
    return;
}

int main(){

    DDRB = 0xFF;
    DDRD = 0x00; // 1111.1111; set PB0-PB7 as outputs	 
    PORTD = 0xFF;

    LCD_Init(); // initialize LCD controller

    byte buttonArray[36] = {0};
    char expr[50] = "";
    byte inv = 0;
    byte but1 = 99, but2 = 99;
    float ans = 0.0;

    while (1)
    {
        byte n = 0;
        // byte but1 = 99, but2 = 99;
        // _delay_ms(500);
        for(byte i=0;i<=5;i++){

            SetBit(DDRD, i+2);
            _delay_us(40);
            ClearBit(PORTD, i+2);

            for(byte j=0;j<=5;j++){
                if(i^j){
                    if(((!(PIND & (1<<(j+2)))) != buttonArray[6*i + j])){
                        buttonArray[6*i + j] ^= 1;
                        if((!(PIND & (1<<(j+2))))){
                            // LCD_Clear();
                            byte but = 6*i + j;
                            char *curs = expr + strlen(expr);
                            switch (but)
                            {
                                case 1:
                                    sprintf(curs, "sqrt(");
                                    break;
                                case 2:
                                    sprintf(curs, "^");
                                    break;
                                case 3:
                                    if(inv) sprintf(curs, "atan(");
                                    else sprintf(curs, "tan(");
                                    break;
                                case 4:
                                    if(inv) sprintf(curs, "acos(");
                                    else sprintf(curs, "cos(");
                                    break;

                                case 5:
                                    if(inv) sprintf(curs, "asin(");
                                    else sprintf(curs, "sin(");
                                    break;
                                case 6:
                                    sprintf(curs, "fac(");
                                    break;
                                case 8:
                                    if(inv) sprintf(curs, "pi");
                                    else sprintf(curs, "1");
                                    break;

                                case 9:
                                    sprintf(curs, "4");
                                    break;
                                case 10:
                                    sprintf(curs, "7");
                                    break;
                                case 11:
                                    if(inv) sprintf(curs, "e^");
                                    else sprintf(curs, "ln(");
                                    break;
                                case 12:
                                    if(inv) sprintf(curs, "pi");
                                    else sprintf(curs, "0");
                                    break;

                                case 13:
                                    sprintf(curs, "9");
                                    break;
                                case 15:
                                    sprintf(curs, "5");
                                    break;
                                case 16:
                                    sprintf(curs, "8");
                                    break;
                                case 17:
                                    if(inv) sprintf(curs, "10^");
                                    else sprintf(curs, "log(");
                                    break;

                                case 18:
                                    sprintf(curs, ".");
                                    break;
                                case 19:
                                    sprintf(curs, "3");
                                    break;
                                case 20:
                                    sprintf(curs, "6");
                                    break;

                                case 22:
                                    sprintf(curs, "9");
                                    break;
                                case 23:
                                    sprintf(curs, "(");
                                    break;
                                case 24:
                                    sprintf(curs, "%f", ans);
                                    break;
                                case 25:
                                    sprintf(curs, "+");
                                    break;
                                case 26:
                                    sprintf(curs, "*");
                                    break;
                                case 27:
                                    sprintf(curs - 1, "\0");
                                    break;
                                case 29:
                                    sprintf(curs, ")");
                                    break;
                                case 30:
                                    // sprintf()
                                    eval(expr, &ans);
                                    break;
                                case 31:
                                    sprintf(curs, "-");
                                    break;
                                case 32:
                                    sprintf(curs, "/");
                                    break;
                                case 33:
                                    expr[0] = '\0';
                                    break;
                                case 34:
                                    inv ^= 1;
                                    break;
                                default:
                                    break;
                            }
                            but1 = 6 * i + j;
                            but2 = PIND;
                        }
                        
                        // if(!())
                    }
                }
            }

            ClearBit(DDRD,i+2);
            _delay_us(40);
            SetBit(PORTD, i+2);

        }

        // char a[] = "2+2^2";

        // float result = te_interp(a, 0);

        LCD_Clear();
        LCD_Cmd(0x80);
        LCD_Message(expr);
        LCD_Cmd(0xC0);
        LCD_Float(ans);
        _delay_ms(100);     // set animation speed
    }

}


// TYPEDEFS
// typedef uint8_t byte; // changed the name


// ------------------
//LCD DRIVER ROUTINES
//
// Routines:
// LCD_Init initializes the LCD controller
// LCD_Cmd sends LCD controller command
// LCD_Char sends single ascii character to display
// LCD_Clear clears the LCD display & homes cursor
// LCD_Integer displays an integer value
// LCD_Message displays a string
// PortB is used for data communications with the HD44780-controlled LCD.
// The following defines specify which port pins connect to the controller:


// // MAIN PROGRAM
// int main(void)
// {
// // use PortB for LCD interface
//  DDRD = 0xFF; // 1111.1111; set PB0-PB7 as outputs	 

//  LCD_Init(); // initialize LCD controller

//  while(1)
//  {
//  LCD_Clear();
//  LCD_Integer(23);  // show counter 
// _delay_ms(600);     // set animation speed
// }
// }
