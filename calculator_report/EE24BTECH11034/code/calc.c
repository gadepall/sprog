#include <avr/io.h> 
#include <util/delay.h> 
#include <stdlib.h> 
#include <math.h>  // Required for pow()
#include <string.h>  // Required for strcmp()
#include <ctype.h>   // Required for isdigit()


// TYPEDEFS
typedef uint8_t byte; // changed the name


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

#define STEP_SIZE 0.0001  // Small step size for numerical accuracy
#define MAX_EXPR_LENGTH 50
#define MAX_STACK_SIZE 100


#define ClearBit(x,y) x &= ~_BV(y) // equivalent to cbi(x,y)
#define SetBit(x,y) x |= _BV(y) // equivalent to sbi(x,y)
#define LCD_RS 2 // pin for LCD R/S (eg PB0)
#define LCD_E 3 // pin for LCD enable
#define DAT4 4 // pin for d4
#define DAT5 5 // pin for d5
#define DAT6 6 // pin for d6
#define DAT7 7 // pin for d7
//// The following defines are controller commands
#define CLEARDISPLAY 0x01

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
 itoa(data,st,10); // 
 LCD_Message(st); // display in on LCD
}

void initADC() {
    ADMUX = (1 << REFS0);  // Select AVcc as reference voltage
    ADCSRA = (1 << ADEN)   // Enable ADC
           | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128 (16MHz / 128 = 125kHz)
}

uint16_t analogRead(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);  // Select ADC channel (0-7)
    ADCSRA |= (1 << ADSC);  // Start conversion
    while (ADCSRA & (1 << ADSC));  // Wait for conversion to finish
    return ADC;  // Return 10-bit result
}

void lcd_setCursor(uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54}; // 16x2 LCD row offsets
    lcd_command(0x80 | (col + row_offsets[row]));  // Set DDRAM address
}

void LCD_Float(float number, uint8_t decimalPlaces) {
    char buffer[10];  // Buffer to store converted float
    dtostrf(number, 1, decimalPlaces, buffer);  // Convert float to string
    LCD_Message(buffer);  // Print the string on LCD
}

int digitalRead(uint8_t pin) {
    if (pin < 8) {  
        return (PIND & (1 << pin)) ? 1 : 0;  // Read from PORTD (pins 0-7)
    } else if (pin < 14) {  
        return (PINB & (1 << (pin - 8))) ? 1 : 0;  // Read from PORTB (pins 8-13)
    } else {  
        return (PINC & (1 << (pin - 14))) ? 1 : 0;  // Read from PORTC (A0-A5 as digital)
    }
}

float temp = 0;

// Character Stack for Operators
struct Stack {
    char arr[MAX_STACK_SIZE][3]; // Store operators (including ** as a string)
    int top;
};

// Number Stack for Evaluation
struct NumStack {
    float arr[MAX_STACK_SIZE];
    int top;
};

// Stack Functions
void initStack(Stack &s) { s.top = -1; }
void initNumStack(NumStack &s) { s.top = -1; }
bool isEmpty(Stack &s) { return s.top == -1; }
bool isEmptyNum(NumStack &s) { return s.top == -1; }

const char* peek(Stack &s) { return isEmpty(s) ? "" : s.arr[s.top]; }
void push(Stack &s, const char* val) { if (s.top < MAX_STACK_SIZE - 1) strcpy(s.arr[++s.top], val); }
void pop(Stack &s) { if (!isEmpty(s)) s.top--; }

void pushNum(NumStack &s, float val) { if (s.top < MAX_STACK_SIZE - 1) s.arr[++s.top] = val; }
float popNum(NumStack &s) { return isEmptyNum(s) ? 0 : s.arr[s.top--]; }

// Derivative function: d^2y/dx^2 = -y
double f(double x, double y, double dy) {
    return -y;
}

// RK4 step for a second-order ODE
void rk4_step(double (*f)(double, double, double), double x, double *y, double *dy, double h) {
    double k1_y = *dy, k1_dy = f(x, *y, *dy);
    double k2_y = *dy + 0.5 * h * k1_dy, k2_dy = f(x + 0.5 * h, *y + 0.5 * h * k1_y, *dy + 0.5 * h * k1_dy);
    double k3_y = *dy + 0.5 * h * k2_dy, k3_dy = f(x + 0.5 * h, *y + 0.5 * h * k2_y, *dy + 0.5 * h * k2_dy);
    double k4_y = *dy + h * k3_dy, k4_dy = f(x + h, *y + h * k3_y, *dy + h * k3_dy);

    *y += (h / 6.0) * (k1_y + 2 * k2_y + 2 * k3_y + k4_y);
    *dy += (h / 6.0) * (k1_dy + 2 * k2_dy + 2 * k3_dy + k4_dy);
}

// Compute sine using RK4
double sine_rk4(double x, double h) {
    double y = 0.0, dy = 1.0; // Initial conditions: y(0) = 0, dy/dx(0) = 1
    for (double t = 0.0; t < x; t += h) {
        rk4_step(f, t, &y, &dy, h);
    }
    return y;
}

// Function to compute tangent using RK4
double tangent_rk4(double radians, double h) {
    // Compute sine and cosine using RK4
    double sine_value = sine_rk4(radians, h);
    double cosine_radians = radians + M_PI / 2; // Add π/2 radians (90 degrees)
    cosine_radians = reduce_angle(cosine_radians); // Reduce to [0, 2π)
    double cosine_value = sine_rk4(cosine_radians, h);

    // Handle division by zero (or very close to zero)
    if (fabs(cosine_value) < 1e-10) {
        return NAN; // Return NaN (Not a Number) for undefined cases
    }

    return sine_value / cosine_value;
}

// Convert degrees to radians
double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

// Reduce angle to [0, 2π)
double reduce_angle(double rad) {
    if (rad < 0) rad += 2 * M_PI * ceil(fabs(rad) / (2 * M_PI));
    return fmod(rad, 2 * M_PI);
}

// Function to compute x^n using Euler's Method on dy/dx = n*y
double power(double x, double n) {
    if (x <= 0) return -1;  // Only valid for positive x

    double y = 1.0;  // Initial condition: y(0) = 1
    double log_x = log(x);  // Compute ln(x)
    int steps = abs(log_x / STEP_SIZE);  // Compute number of steps dynamically

    double dx = log_x / steps;  // Adjust step size to match ln(x)


    for (int i = 0; i < steps; i++) {
        y += dx * (n * y);  // Euler's method: y_k+1 = y_k + h*f(x, y)
    }

    return y;
}

// Trapezoidal Rule for numerical integration
double trapezoidal_rule(double a, double b, int n) {
    double h = (b - a) / n;  // Step size
    double sum = 0.5 * ((1/a) + (1/b));  // First and last terms

    // Sum intermediate terms
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        sum += 1/x;
    }

    return h * sum;  // Final result
}

// Function to compute ln(x) using numerical integration
double compute_ln(double x) {
    if (x <= 0) {
        printf("Error: ln(x) is undefined for x <= 0.\n");
        return NAN;  // Return NaN for invalid input
    }

    int n = 1000;  // Number of intervals for integration
    return trapezoidal_rule(1.0, x, n);  // Integrate from 1 to x
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to convert decimal to fraction
void decimal_to_fraction(double decimal, int *numerator, int *denominator) {
    double tolerance = 1e-6;  // Precision control
    int max_denominator = 10000;  // Limit denominator size

    *numerator = 1;
    *denominator = 1;

    // If decimal is zero, return 0/1
    if (decimal == 0) {
        *numerator = 0;
        *denominator = 1;
        return;
    }

    int sign = (decimal < 0) ? -1 : 1;
    decimal = fabs(decimal);

    int temp_denominator = 1;
    while (fabs(decimal * temp_denominator - round(decimal * temp_denominator)) > tolerance && temp_denominator < max_denominator) {
        temp_denominator *= 10;
    }

    *numerator = round(decimal * temp_denominator) * sign;
    *denominator = temp_denominator;

    // Simplify using GCD
    int common_divisor = gcd(*numerator, *denominator);
    *numerator /= common_divisor;
    *denominator /= common_divisor;
}


// Operator precedence
int precedence(const char* op) {
    if(strcmp(op, "e") == 0 || strcmp(op, "p") == 0 || strcmp(op, "A") == 0) return 5;
    if (strcmp(op, "") == 0) return 4;
    if (strcmp(op, "s") == 0 || strcmp(op, "c") == 0 || strcmp(op, "t") == 0) return 3;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0) return 2;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
    return 0;
}

// Convert Infix to Postfix
void infixToPostfix(const char* infix, char* postfix) {
    Stack s;
    initStack(s);
    int j = 0;

    for (int i = 0; infix[i] != '\0'; i++) {
        char c = infix[i];
        
        if (isdigit(c) || c == '.') {
            while (isdigit(infix[i]) || infix[i] == '.') {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' ';
            i--;
        } 
        else if (c == '(') {
            push(s, "(");
        } 
        else if (c == ')') {
            while (!isEmpty(s) && strcmp(peek(s), "(") != 0) {
                strcpy(&postfix[j], peek(s));
                j += strlen(peek(s));
                postfix[j++] = ' ';
                pop(s);
            }
            pop(s);
        }
        else if(c == 'e'){
          push(s, "e");
        }
        else if(c == 'p'){
          push(s, "p");
        }
        else if (c == 's' && infix[i + 1] == '(') {
            push(s, "s");
        }
        else if(c == 'l' && infix[i + 1] == '(') {
            push(s, "l");
        }
        else if(c == 'L' && infix[i + 1] == '(') {
            push(s, "L");
        }
        else if(c == 'A'){
          push(s, "A");
        }
        else {
            char op[3] = {c, '\0', '\0'};
            if (infix[i] == '' && infix[i + 1] == '') {
                op[1] = '*';
                i++;
            }
            while (!isEmpty(s) && precedence(peek(s)) >= precedence(op)) {
                strcpy(&postfix[j], peek(s));
                j += strlen(peek(s));
                postfix[j++] = ' ';
                pop(s);
            }
            push(s, op);
        }
    }
    while (!isEmpty(s)) {
        strcpy(&postfix[j], peek(s));
        j += strlen(peek(s));
        postfix[j++] = ' ';
        pop(s);
    }
    postfix[j] = '\0';
}

float evaluatePostfix(const char* postfix) {
    NumStack s;
    initNumStack(s);
    int i = 0;
    
    while (postfix[i] != '\0') {
        if (isdigit(postfix[i]) || postfix[i] == '.') {
            pushNum(s, atof(&postfix[i]));
            while (isdigit(postfix[i]) || postfix[i] == '.') i++;
        } 
        else if (postfix[i] == 'e') {
          float val = popNum(s);
          pushNum(s, 2.71828);
        }
        else if (postfix[i] == 'p') {
          float val = popNum(s);
          pushNum(s, 3.14159);
        }
        else if (postfix[i] == 's') {
            float val = popNum(s);
            pushNum(s, sine_rk4(reduce_angle(deg2rad(val)), STEP_SIZE));
        }
        else if (postfix[i] == 'c') {
            float val = popNum(s);
            pushNum(s, sine_rk4(reduce_angle(90 - deg2rad(val)), STEP_SIZE));
        }
        else if (postfix[i] == 't') {
            float val = popNum(s);
            pushNum(s, tangent_rk4(deg2rad(val), STEP_SIZE));
        }
        else if (postfix[i] == 'l') {
          float val = popNum(s);
          pushNum(s, compute_ln(val));
        }
        else if(postfix[i] == 'L') {
          float val = popNum(s);
          pushNum(s, compute_ln(val) / 2.30258);
        }
        else if(postfix[i] == 'A') {
          float val = popNum(s);
          pushNum(s, temp);
        }
        else if (postfix[i] == '+') {
            float b = popNum(s);
            float a = popNum(s);
            pushNum(s, a + b);
        }
        else if (postfix[i] == '-') {
            float b = popNum(s);
            float a = popNum(s);
            pushNum(s, a - b);
        }
        else if (postfix[i] == '*') {
            float b = popNum(s);
            float a = popNum(s);
            pushNum(s, a * b);
        }
        else if (postfix[i] == '/') {
            float b = popNum(s);
            float a = popNum(s);
            pushNum(s, a / b);
        }
        i++;
    }
    return popNum(s);
}




char userInput[MAX_EXPR_LENGTH]; // Holds up to 5 digits
int inputPos = 0; // Current input position
int num_brackets = 0;

int main(){

 // Set A0, A1, A2, A3, A4, A5 as INPUT with PULLUP (Analog pins are on PORTC)
DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5)); 
PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5);

// Set 8, 9, 10, 11, 12, 13 as INPUT with PULLUP (Digital pins 8-13 are on PORTB)
DDRB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5));
PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);


  LCD_Init();
  LCD_Clear();
  _delay_ms(1000);
  LCD_Message("Enter: ");

  int number = analogRead(0);

  char selectedChar = '_';  // Default character
  if (number > 900) {
    selectedChar = '_';  // No number pressed
  }
  else if (number > 800) {
    selectedChar = '0';
  }
  else if (number > 775) {
    selectedChar = '1';
  }
  else if (number > 750) {
    selectedChar = '2';
  }
  else if (number > 705) {
    selectedChar = '3';
  }
  else if (number > 660) {
    selectedChar = '4';
  }
  else if (number > 600) {
    selectedChar = '5';
  }
  else if (number > 550) {
    selectedChar = '6';
  }
  else if (number > 450) {
    selectedChar = '7';
  }
  else if (number > 300) {
    selectedChar = '8';
  }
  else if (number < 300){
    selectedChar = '9';
  }
  if(!digitalRead(13)){
    selectedChar = '+';
  }
  else if(!digitalRead(12)){
    selectedChar = '-';
  }
  else if(!digitalRead(11)){
    selectedChar = '*';
  }
  else if(!digitalRead(10)){
    selectedChar = '/';
  }
  else if(!digitalRead(9)){
    selectedChar = '(';
    num_brackets++;
  }
  else if(analogRead(3) < 100){
    selectedChar = ')';
    num_brackets--;
  }
  else if(analogRead(4) < 100){
    if(userInput[inputPos - 1] == 'l'){
      userInput[inputPos - 1] = 'L';
    }
    else if(userInput[inputPos - 1] == 'L'){
      userInput[inputPos - 1] = 'l';
    }
    else{
      selectedChar = 'l';
    }
  }
  else if(!digitalRead(8)){
    if(inputPos == 0){
      int num, den;
      decimal_to_fraction(temp, &num, &den);
      LCD_Clear();
      _delay_ms(100);
      LCD_Integer(num);
      LCD_Message("/");
      LCD_Integer(den);
    }
    else{
      char postfix[MAX_EXPR_LENGTH];
      if(num_brackets > 0){
        for(int i=inputPos;i<num_brackets + inputPos;i++){
          userInput[i] = ')';
        }
      }
      infixToPostfix(userInput, postfix);
      float result = evaluatePostfix(postfix);
      temp = result;
      lcd_setCursor(0, 0);
      LCD_Clear();
      LCD_Float(result, 5);
      inputPos = 0;
      for(int i=0;i<16;i++){
        userInput[i] = '\0';
      }
    }
  }
  else if(!digitalRead(1)){
    selectedChar = '.';
  }
  else if(!digitalRead(0)){
    userInput[inputPos - 1] = ' ';
    inputPos--;
  }
  else if(analogRead(1) < 100){
    if(userInput[inputPos - 1] == 'e'){
      userInput[inputPos - 1] = 'p';
    }
    else if(userInput[inputPos - 1] == 'p'){
      userInput[inputPos - 1] = 'e';
    }
    else{
      selectedChar = 'e';
    }
  }
  else if(analogRead(2) < 100){
    if(userInput[inputPos - 1] == 's'){
      userInput[inputPos - 1] = 'c';
    }
    else if(userInput[inputPos - 1] == 'c'){
      userInput[inputPos - 1] = 't';
    }
    else if(userInput[inputPos - 1] == 't'){
      userInput[inputPos - 1] = 's';
    }
    else{
      selectedChar = 's';
    }
  }
  // Only update if a number is pressed
  if (selectedChar != '_') {
    if (inputPos < 16) {
      userInput[inputPos] = selectedChar;
      inputPos++;
    }
  }
  // Display the input string
  lcd_setCursor(0, 1);
  LCD_Message(userInput);

  if(analogRead(5) < 100){
    LCD_Clear();
    for(int i=0;i<16;i++){
      userInput[i] = '\0';
    }
    inputPos = 0;
    LCD_Message("Enter:");
    lcd_setCursor(0, 1);
  }

  if(inputPos == 1){
    if(selectedChar == '+' || selectedChar == '-' || selectedChar == '*' || selectedChar == '/'){
      userInput[0] = 'A';
      userInput[1] = selectedChar;
      inputPos++;
    }
  }

  _delay_ms(250); // Simple debounce delay
}
