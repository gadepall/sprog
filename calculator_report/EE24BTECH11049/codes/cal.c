#include <avr/io.h> 
#include <avr/eeprom.h>
#include <util/delay.h> 
#include "parser.h"
#include "funcs.h"
#include <stdlib.h>
#include "string.h"
#include <stdio.h>

// ------------------
// TYPEDEFS
typedef uint8_t byte;
#define MAX_SIZE 128
#define ADDRESS 0

// ------------------
// MACROS
// CLR_BIT -> Clears bit number 'Y' on register 'X'
#define CLR_BIT(X, Y) X &= ~_BV(Y)

// SET_BIT -> Sets bit number 'Y' on register 'X'
#define SET_BIT(X, Y) X |= _BV(Y)

// LCD_RS -> R/S (Register Select)
#define LCD_RS 0 // PB0

// LCD_E -> LCD Enable pin
#define LCD_E 1 // PB1

// DATA PINS
#define DB4 2 // pin for DB4
#define DB5 3 // pin for DB5
#define DB6 4 // pin for DB6
#define DB7 5 // pin for DB7

// CLEAR_DISPLAY -> Instruction for clearing display
#define CLEAR_DISPLAY 0x01

// BUTTON_THRESHOLD -> Sets the button delay between taps
// equivalent to BUTTON_THRESHOLD*50 ms
//#define BUTTON_THRESHOLD 10
#define BUTTON_THRESHOLD 10

// N_BUTTONS -> Number of active buttons
#define N_BUTTONS 6

#define ROW_DDR DDRC
#define ROW_PORT PORTC
#define ROW_PIN PINC

// Digital pins (PD2-PD6) as columns
#define COL_DDR DDRD
#define COL_PORT PORTD
#define COL_PIN PIND

/* UTILITY FUNCTIONS */
void clear_buf(int* len, char* buf){
    for(int i = 0; i < *len; i++) buf[i] = '\0';
    *len = 0;
}

void set_buf(int* pos, char* buf, const char* str){
    *pos = 0;
    while(*str){
        buf[*pos] = *str++;
        *pos = *pos + 1;
    }
}

int is_func(char repr){
    return (repr == 's' || repr == 'c' || repr == 't' || repr == 'l' ||
            repr == '@' || repr == '#' || repr == '$');
}

/* LCD FUNCTIONS */
void pulse_enable_line(){
    SET_BIT(PORTB, LCD_E); // take LCD enable line high
    _delay_us(40); // wait 40 microseconds
    CLR_BIT(PORTB, LCD_E); // take LCD enable line low
}

void send_nibble(byte data){
    PORTB &= 0xC3; // 1100.0011 = clear 4 data lines
    if (data & _BV(4)) SET_BIT(PORTB, DB4);
    if (data & _BV(5)) SET_BIT(PORTB, DB5);
    if (data & _BV(6)) SET_BIT(PORTB, DB6);
    if (data & _BV(7)) SET_BIT(PORTB, DB7);
    pulse_enable_line(); // clock 4 bits into controller
}

void send_byte(byte data){
    send_nibble(data); // send upper 4 bits
    send_nibble(data << 4); // send lower 4 bits
    CLR_BIT(PORTB, 5); // turn off PIN 13 (Clk i guess)
}

void lcd_cmd(byte cmd){
    CLR_BIT(PORTB, LCD_RS); // R/S line 0 = command data
    send_byte(cmd); // send it
}

void lcd_char(byte ch){
    SET_BIT(PORTB, LCD_RS); // R/S line 1 = character data
    send_byte(ch); // send it
}

void lcd_init(){
    lcd_cmd(0x33); // initialize controller
    lcd_cmd(0x32); // set to 4-bit input mode
    lcd_cmd(0x28); // 2 line, 5x7 matrix
    lcd_cmd(0x0E); // turn cursor off (0x0E to enable)
    lcd_cmd(0x06); // cursor direction = right
    lcd_cmd(0x01); // start with clear display
    _delay_ms(3); // wait for LCD to initialize
}

void lcd_clear() {
    lcd_cmd(CLEAR_DISPLAY);
    _delay_ms(3); // wait for LCD to process command
}

void lcd_msg(const char *text){
    while (*text) 
        lcd_char(*text++); // send char & update char pointer
}

void lcd_int(int data){
    char st[8] = ""; // save enough space for result
    itoa(data,st,10); // convert integer to ascii 
    lcd_msg(st); // display in on LCD
}

void display_biline(int pos1, char* buf1, int pos2, char* buf2){
    lcd_clear();

    char buf_cpy[MAX_SIZE] = {'\0'};
    int offset = 0;

    if(
        buf1[0] == '+' ||
        buf1[0] == '-' ||
        buf1[0] == '*' ||
        buf1[0] == '/' 
    ) {
        buf_cpy[0] = 'A';
        buf_cpy[1] = 'n';
        buf_cpy[2] = 's';
        offset = 3;
    }

    for(int i = 0; i < pos1; i++){
        char ch = buf1[i];

        if(ch == 's'){
            buf_cpy[i + offset] = 's';
            buf_cpy[i + offset + 1] = 'i';
            buf_cpy[i + offset + 2] = 'n';
            offset += 2;
        }
        else if(ch == 'c'){
            buf_cpy[i + offset] = 'c';
            buf_cpy[i + offset + 1] = 'o';
            buf_cpy[i + offset + 2] = 's';
            offset += 2;
        }
        else if(ch == 'c'){
            buf_cpy[i + offset] = 'c';
            buf_cpy[i + offset + 1] = 'o';
            buf_cpy[i + offset + 2] = 's';
            offset += 2;
        }
        else if(ch == 't'){
            buf_cpy[i + offset] = 't';
            buf_cpy[i + offset + 1] = 'a';
            buf_cpy[i + offset + 2] = 'n';
            offset += 2;
        }
        else if(ch == 'l'){
            buf_cpy[i + offset] = 'l';
            buf_cpy[i + offset + 1] = 'n';
            offset += 1;
        }
        else if(ch == '@'){
            buf_cpy[i + offset] = 'a';
            buf_cpy[i + offset + 1] = 's';
            buf_cpy[i + offset + 2] = 'i';
            buf_cpy[i + offset + 3] = 'n';
            offset += 3;
        }
        else if(ch == '#'){
            buf_cpy[i + offset] = 'a'; buf_cpy[i + offset + 1] = 'c'; buf_cpy[i + offset + 2] = 'o';
            buf_cpy[i + offset + 3] = 's';
            offset += 3;
        }
        else if(ch == '$'){
            buf_cpy[i + offset] = 'a';
            buf_cpy[i + offset + 1] = 't';
            buf_cpy[i + offset + 2] = 'a';
            buf_cpy[i + offset + 3] = 'n';
            offset += 3;
        }
        else if(ch == 'p'){
            buf_cpy[i + offset] = (char) (247);
        }
        else if(ch == ','){
    buf_cpy[i + offset] = 's';
    buf_cpy[i + offset + 1] = 'i';
    buf_cpy[i + offset + 2] = 'n';
    buf_cpy[i + offset + 3] = 'h';
    offset += 3;
}
else if(ch == '?'){
    buf_cpy[i + offset] = 'c';
    buf_cpy[i + offset + 1] = 'o';
    buf_cpy[i + offset + 2] = 's';
    buf_cpy[i + offset + 3] = 'h';
    offset += 3;
}
else if(ch == ':'){
    buf_cpy[i + offset] = 't';
    buf_cpy[i + offset + 1] = 'a';
    buf_cpy[i + offset + 2] = 'n';
    buf_cpy[i + offset + 3] = 'h';
    offset += 3;
}

        else {
            buf_cpy[i + offset] = ch; 
        }
    }

    pos1 += offset;

    lcd_cmd(0x80);
    if(pos1 > 16) lcd_msg(buf_cpy + pos1 - 15);
    else lcd_msg(buf_cpy);

    lcd_cmd(0xC0);
    if(pos2 > 16) lcd_msg(buf2 + pos2 - 15);
    else lcd_msg(buf2);

    lcd_cmd(0x80);

    for(int i = 0; i < (pos1 > 16 ? 15: pos1); i++) lcd_cmd(0x14);
}

/* BUTTON FUNCTIONS */
void button_listener(int* button_x, int* button_y, int* debounce) {
    if(*debounce > BUTTON_THRESHOLD) {
        *button_x = -1;
        *button_y = -1;
    }

    if((*button_x != -1) && (*debounce < BUTTON_THRESHOLD)) return;

    // Set all columns high
    COL_PORT |= 0x7C;  // 0111 1100 (PD2-PD6)

    for(int row = 0; row < N_BUTTONS; row++) {
        // Set current row low
        ROW_PORT &= ~(1 << row);

        for(int col = 0; col < N_BUTTONS; col++) {
            // Check if button is pressed (column is pulled low)
            if(!(COL_PIN & (1 << (col + 2)))) {
                *button_x = col;
                *button_y = row;
                *debounce = 0;
                ROW_PORT |= (1 << row);  // Set row back to high
                return;
            }
        }

        ROW_PORT |= (1 << row);  // Set row back to high
    }
}

char button_map(int button_x, int button_y, int is_mode){
    /*
     * @ -> arcsin
     * # -> arccos
     * $ -> arctan
     * & -> mode change
	
     */
    char button_map[6][6] = {
        { '0', '1', '2', '3', '4', '5'}, // yellow
        { '6', '7', '8', '9', ',', '?'}, // white
        { '(', 'f', 's', 'c', 't', ':'}, // black
        { '+', '-', '*', '/', '^', '!'}, // blue
        { '=', '.', '_', '<', '&', 'M'}, // red
        { '@', '#', '$', 'l', 'e', 'p'}  // green
    };

    char button_map_mode[6][6] = {
        { '0', '1', '2', '3', '4', '5'}, // yellow
        { '6', '7', '8', '9', ',', '?'}, // white
        { 'm', 'M', '@', '#', '$', ':'}, // black
        { '!', 'e', 'p', '%', 'l', '('}, // blue
        { '=', '.', '_', '<', '&', ')'}, // red
        { 's', 'c', 't', ',', '?', ':'}  // green
    };

    if(is_mode) return button_map_mode[button_y][button_x];
    return button_map[button_y][button_x];
}

int main(void){
    // Disable ADC (Convert Analog -> Digital)
    ADCSRA &= ~(1 << ADEN);
 // Setting input pull up
    
    // use PortB for LCD interface
    DDRB = 0xFF; // 1111.1111; set PB0-PB7 as outputs	
    
    lcd_init();
    lcd_msg("Calculator ready"); 
    _delay_ms(5000);
    lcd_clear();


    ROW_DDR |= 0xFF;   // 0001 1111
    ROW_PORT |= 0xFF;  // Set all rows high initially

    // Configure digital pins (PD2-PD6) as inputs with pull-ups (columns)
    COL_DDR &= ~0x00;  // 1000 0011 (set PD2-PD6 as inputs)
    COL_PORT |= 0xFF;  // Enable pull-ups on PD2-PD6

 // initialize LCD controller
    
    int button_x = -1;
    int button_y = -1;
    int debounce = 0;
    int is_answer_loop = 0;

    char buf1[MAX_SIZE] = {'\0'};
    int pos1 = 0;

    char buf2[MAX_SIZE] = {'\0'};
    int pos2 = 0;

    double ans = 0;
    int is_mode = 0;

    int braks = 0;

    while(1){
        button_listener(&button_x, &button_y, &debounce);

        if(button_x != -1 && debounce == 0){
            char mapped_button = button_map(button_x, button_y, is_mode);
            
            if(mapped_button == '='){
                is_answer_loop = 1;
                lcd_clear();

                for(int i = 0; i < braks; i++){
                    buf1[pos1] = ')';
                    pos1 += 1;
                }

                ans = eval(buf1, ans);
                if(ans > 1e15) sprintf(buf2, "%16.*e", ans);
                else dtostrf(ans, 16, 5, buf2);

                pos2 = 16;
                debounce += 1;
                display_biline(pos1, buf1, pos2, buf2);
                _delay_ms(50);
                continue;
            }

            if(mapped_button == 'm'){
                if(is_answer_loop) eeprom_write_block((const void*) &ans, (void*) ADDRESS, sizeof(double));
                debounce += 1;
                _delay_ms(50);
                continue;
            }
            else if(mapped_button == 'f'){
                if(is_answer_loop){
                    int numerator;
                    int denominator;
                    dtf(ans, &numerator, &denominator);

                    char temp[16];
                    sprintf(buf2, "%d/%d", (int) numerator, (int) denominator);
                    //clear_buf(&pos2, buf2);

                    //sprintf(buf2, "%16s", temp);
                    //pos2 = 16;
                    display_biline(pos1, buf1, pos2, buf2);
                }

                debounce += 1;
                _delay_ms(50);
                continue;
            }

            if(is_answer_loop){
                if(mapped_button != '<' && mapped_button != '(') clear_buf(&pos1, buf1);
                clear_buf(&pos2, buf2);
                is_answer_loop = 0;
                braks = 0;
            }

            if(mapped_button == '<'){
                if(pos1 >= 2 && buf1[pos1 - 1] == '(' && is_func(buf1[pos1 - 2])) { 
                    buf1[pos1 - 2] = '\0';
                    pos1 = pos1 - 1;
                }
                buf1[pos1 - 1] = '\0';
                pos1 = pos1 - 1;
            }
            else if(mapped_button == '_'){
                clear_buf(&pos1, buf1);
                clear_buf(&pos2, buf2);
            }
            else if(is_func(mapped_button)){
                buf1[pos1] = mapped_button;
                buf1[pos1 + 1] = '(';
                braks++;
                pos1 += 2;
            }
            else if(mapped_button == '&'){
                is_mode = !is_mode;
            }
            else if(mapped_button == '('){
                if(pos1 > 0){
                    char prev_ch = buf1[pos1 - 1];
                    if((prev_ch == ')') || (prev_ch >= '0' && prev_ch <= '9') || (prev_ch == 'M')) mapped_button = ')';
                }

                if(mapped_button == ')') braks -= 1;
                else braks += 1;

                buf1[pos1] = mapped_button;
                pos1 += 1;
            }
            else {
                buf1[pos1] = mapped_button;
                pos1 += 1;
            }

            display_biline(pos1, buf1, pos2, buf2);
        }

        debounce += 1;
        _delay_ms(20);
    }
}
