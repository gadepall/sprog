#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include <stdlib.h>

// LCD Connections
#define RS PD2
#define EN PD3
#define D4 PD4
#define D5 PD5
#define D6 PD6
#define D7 PD7

// Button Mappings
#define BUTTON_0 PB0
#define BUTTON_1 PB1
#define BUTTON_2 PB2
#define BUTTON_3 PB3
#define BUTTON_4 PB4   // sin(x)
#define BUTTON_5 PB5   // cos(x)
#define BUTTON_6 PC0
#define BUTTON_7 PC1
#define BUTTON_8 PC2
#define BUTTON_9 PC3
#define BUTTON_EQ PC4
#define BUTTON_2ND PC5

// LCD Functions
void lcd_command(uint8_t cmd) {
    PORTD = (PORTD & 0x0F) | (cmd & 0xF0);
    PORTD &= ~(1 << RS);
    PORTD |= (1 << EN);
    _delay_ms(2);
    PORTD &= ~(1 << EN);

    PORTD = (PORTD & 0x0F) | (cmd << 4);
    PORTD |= (1 << EN);
    _delay_ms(2);
    PORTD &= ~(1 << EN);
}

void lcd_init(void) {
    DDRD |= (1 << RS) | (1 << EN) | (1 << D4) | (1 << D5) | (1 << D6) | (1 << D7);
    _delay_ms(100);
    lcd_command(0x33);
    lcd_command(0x32);
    lcd_command(0x28);
    lcd_command(0x0C);
    lcd_command(0x06);
    _delay_ms(5);
}

void lcd_clear(void) {
    lcd_command(0x01);
    _delay_ms(5);
}

void lcd_char(char data) {
    PORTD = (PORTD & 0x0F) | (data & 0xF0);
    PORTD |= (1 << RS);
    PORTD |= (1 << EN);
    _delay_ms(2);
    PORTD &= ~(1 << EN);

    PORTD = (PORTD & 0x0F) | (data << 4);
    PORTD |= (1 << EN);
    _delay_ms(2);
    PORTD &= ~(1 << EN);
}

void lcd_print(char *str) {
    while (*str) {
        lcd_char(*str++);
    }
}

void lcd_print_mode(int sci_mode) {
    lcd_command(0xC0);  // Move to second line
    lcd_print(sci_mode ? "Mode: Scientific" : "Mode: Normal");
}

// Runge-Kutta 4th Order for sin(x)
long double rk4_sin(long double x) {
    long double y = 0.0, dy = 1.0;  // sin(0) = 0, cos(0) = 1
    long double h = 0.0001;          // Small step size
    int N = (int)(x / h);          // Number of steps

    for (int i = 0; i < N; i++) {
        long double k1y = h * dy;
        long double k1dy = -h * y;

        long double k2y = h * (dy + k1dy / 2);
        long double k2dy = -h * (y + k1y / 2);

        long double k3y = h * (dy + k2dy / 2);
        long double k3dy = -h * (y + k2y / 2);

        long double k4y = h * (dy + k3dy);
        long double k4dy = -h * (y + k3y);

        y += (k1y + 2*k2y + 2*k3y + k4y) / 6.0;
        dy += (k1dy + 2*k2dy + 2*k3dy + k4dy) / 6.0;
    }

    return y;
}

long double rk4_cos(long double x) {
    return rk4_sin(1.57079632679L - x);  // π/2 - x, assuming x is in radians
}

// Runge-Kutta approximation for ln(x)
long double rk4_ln(long double x) {
    if (x <= 0) return -1;  // Invalid input
    if (x == 1) return 0;   // ln(1) = 0

    long double y = 0.0;
    long double h = (x - 1) / 1000.0;  // Divide into 1000 steps for efficiency
    long double x0 = 1.0;

    for (int i = 0; i < 1000; i++) {
        long double k1 = h / x0;
        long double k2 = h / (x0 + h / 2);
        long double k3 = h / (x0 + h / 2);
        long double k4 = h / (x0 + h);
        y += (k1 + 2*k2 + 2*k3 + k4) / 6.0;
        x0 += h;
        if (x0 >= x) break;  // Stop early if we reach x
    }

    return y;
}



// Newton-Raphson method for sqrt(x)
long double rk4_sqrt(long double x) {
    if (x < 0) return -1;  // Handle negative input
    long double guess = x / 2.0;  
    for (int i = 0; i < 10; i++) {
        guess = (guess + x / guess) / 2.0;
    }
    return guess;
}
double rk4_arctan(double x) {
    double y = 0.0;

    double h = x / 1000.0;  
    double t = 0.0;
    for (int i = 0; i < 1000; i++) {
        double k1 = h / (1 + t*t);
        double k2 = h / (1 + (t + h/2)*(t + h/2));
        double k3 = h / (1 + (t + h/2)*(t + h/2));
        double k4 = h / (1 + (t + h)*(t + h));
        y += (k1 + 2*k2 + 2*k3 + k4) / 6.0;
        t += h;
    }
    return y;
}
// Button Reading
char read_button(int sci_mode) {
    if (!(PINB & (1 << BUTTON_0))) return sci_mode ? '+' : '0';
    if (!(PINB & (1 << BUTTON_1))) return sci_mode ? '-' : '1';
    if (!(PINB & (1 << BUTTON_2))) return sci_mode ? '*' : '2';
    if (!(PINB & (1 << BUTTON_3))) return sci_mode ? '/' : '3';
    if (!(PINB & (1 << BUTTON_4))) return sci_mode ? 'S' : '4';
    if (!(PINB & (1 << BUTTON_5))) return sci_mode ? 'C' : '5';
    if (!(PINC & (1 << BUTTON_6))) return sci_mode?'L':'6';
    if (!(PINC & (1 << BUTTON_7))) return sci_mode?'R':'7';
    if (!(PINC & (1 << BUTTON_8))) return sci_mode? 'T':'8';
    if (!(PINC & (1 << BUTTON_9))) return '9';
    if (!(PINC & (1 << BUTTON_2ND))) return 's';
    if (!(PINC & (1 << BUTTON_EQ))) return '=';
    return '\0';
}




int main(void) {
    DDRB &= ~0x3F;
    DDRC &= ~0x3F;
    PORTB |= 0x3F;
    PORTC |= 0x3F;

    lcd_init();
    lcd_clear();
    lcd_print("Ready!");

    int num1 = 0, num2 = 0, state = 0, sci_mode = 0;
    char op = '\0', display[16];

    while (1) {
        lcd_print_mode(sci_mode);
        char key = read_button(sci_mode);

        if (key) {
            _delay_ms(200);  // Debounce

            if (key == 's') {  
                sci_mode = !sci_mode;
            } 
            else if (state == 0) {
                if (key >= '0' && key <= '9') {
                    num1 = num1 * 10 + (key - '0');
                    sprintf(display, "%d", num1);
                    lcd_clear();
                    lcd_print(display);
                } 
               if (key == 'S' || key == 'C') {
    op = key;   // Store operation ('S' for sin, 'C' for cos)
    state = 1;  // Move to waiting-for-number state

    sprintf(display, "%s(%d)", (op == 'S') ? "sin":"cos", num1);
    lcd_clear();
    lcd_print(display);
    sci_mode=0;
}
else if(key=='T'){
	op=key;
	state=1;
	sprintf(display, "%s(%d)","atan",num1);
	lcd_clear();
	lcd_print(display);
	sci_mode=0;
}
else if (key == 'L' || key == 'R') {
    op = key;   // Store operation ('L' for ln, 'R' for sqrt)
    state = 1;  // Move to waiting-for-number state

    sprintf(display, "%s(%d)", (op == 'L') ? "ln" : "sqrt", num1);
    lcd_clear();
    lcd_print(display);
    sci_mode=0;
}



                else if (key == '+' || key == '-' || key == '*' || key == '/') {
                    op = key;
                    sci_mode = 0;
                    state = 1;
                    sprintf(display, "%d %c", num1, op);
                    lcd_clear();
                    lcd_print(display);
                }
            }  
            else if (state == 1) {
                if (key >= '0' && key <= '9') {
                    num2 = num2 * 10 + (key - '0');
                    sprintf(display, "%d %c %d", num1, op, num2);
                    lcd_clear();
                    lcd_print(display);
                } 
               else if (key == '=') {
    if (op == 'S' || op == 'C') {
        double radians = num1 * (3.14159265358979323846 / 180.0);  // Convert degrees to radians
        double result = (op == 'S') ? rk4_sin(radians) : rk4_cos(radians);

        // Apply rounding correction
        result = ((int)(result * 10000 + 0.5)) / 10000.0;  // Round to 4 decimal places

        int int_part = (int)result;  // Integer part
        int frac_part = (int)((result - int_part) * 10000);  // Fractional part (4 decimal places)

        sprintf(display, "%s(%d)=%d.%04d", (op == 'S') ? "sin" : "cos", num1, int_part, abs(frac_part));
        lcd_clear();
        lcd_print(display);

        num1 = result;  // Store result for further operations
        state = 0;  // Reset state for next input
    } 
     else if (op == 'L' || op == 'R') {
        double result = (op == 'L') ? rk4_ln(num1) : rk4_sqrt(num1);

        result = ((int)(result * 10000 + 0.5)) / 10000.0;  // Round to 4 decimal places

        int int_part = (int)result;
        int frac_part = (int)((result - int_part) * 10000);

        sprintf(display, "%s(%d)=%d.%04d", (op == 'L') ? "ln" : "sqrt", num1, int_part, abs(frac_part));
        lcd_clear();
        lcd_print(display);
        state = 0;
        num1=0;
    }
    else if (op == 'T') {  
    double result = rk4_arctan(num1);  
    result = ((int)(result * 10000 + 0.5)) / 10000.0;   // Round to 4 decimal places  
    int int_part = (int)result;  
    int frac_part = (int)((result - int_part) * 10000);  

    sprintf(display, "atan(%d)=%d.%04d", num1, int_part, abs(frac_part));  
    lcd_clear();  
    lcd_print(display);  

    state = 0;  
    num1 = 0;  
}
     


    else {  // Normal arithmetic
    if (op == '/' && num2 == 0) {  // Handle division by zero
        sprintf(display, "%d/%d = undefined", num1, num2);
    } 
    else if (op == '/') {
        double result = (double)num1 / num2;  // Convert to double for proper division
        int int_part = (int)result;  // Get the integer part
        int frac_part = (int)((result - int_part) * 10000);  // Get fractional part (4 decimal places)

        sprintf(display, "%d %c %d = %d.%04d", num1, op, num2, int_part, abs(frac_part));
    } 
    else {
        int result_int = (op == '+') ? num1 + num2 :
                         (op == '-') ? num1 - num2 :
                         (op == '*') ? num1 * num2 :
                         (num2 != 0 ? num1 / num2 : 0);
        sprintf(display, "%d %c %d = %d", num1, op, num2, result_int);
    }
    
    lcd_clear();
    lcd_print(display);

    num1 = num2 = 0;
    state = 0;
}

    
    _delay_ms(3000);
    lcd_clear();
    _delay_ms(50);  // Small delay to ensure clearing happens

}
            }
        }
    }
}
