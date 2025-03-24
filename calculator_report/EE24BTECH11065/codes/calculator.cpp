#include <LiquidCrystal.h>
#include <math.h>  // Required for pow()
#include <string.h>  // Required for strcmp()
#include <ctype.h> 
#include<Arduino.h>
#include<stdbool.h>
#include<stdlib.h>

// Required for isdigit()

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define STEP_SIZE 0.0001  // Small step size for numerical accuracy
#define MAX_EXPR_LENGTH 50
#define MAX_STACK_SIZE 100

// Convert degrees to radians
double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

// Reduce angle to [0, 2π)
double reduce_angle(double rad) {
    if (rad < 0) rad += 2 * M_PI * ceil(fabs(rad) / (2 * M_PI));
    return fmod(rad, 2 * M_PI);
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
    if (strcmp(op, "**") == 0) return 4;
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
            if (infix[i] == '*' && infix[i + 1] == '*') {
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
            pushNum(s, sine_rk4(reduce_angle(1.570796326 -  deg2rad(val)), STEP_SIZE));
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

void setup() {
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  Serial.begin(9600);
  delay(500);
  lcd.begin(16, 2);
  lcd.clear();
  delay(1000);
  lcd.print("Enter: ");
}

void loop() {
  int number = analogRead(A0);
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
  else if(analogRead(A3) < 100){
    selectedChar = ')';
    num_brackets--;
  }
  else if(analogRead(A4) < 100){
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
      lcd.clear();
      delay(100);
      lcd.print(num);
      lcd.print("/");
      lcd.print(den);
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
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print(result, 5);
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
  else if(analogRead(A1) < 100){
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
  else if(analogRead(A2) < 100){
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

  Serial.println(inputPos);
  // Display the input string
  lcd.setCursor(0, 1);
  lcd.print(userInput);

  if(analogRead(A5) < 100){
    lcd.clear();
    for(int i=0;i<16;i++){
      userInput[i] = '\0';
    }
    inputPos = 0;
    lcd.print("Enter:");
    lcd.setCursor(0, 1);
  }

  if(inputPos == 1){
    if(selectedChar == '+' || selectedChar == '-' || selectedChar == '*' || selectedChar == '/'){
      userInput[0] = 'A';
      userInput[1] = selectedChar;
      inputPos++;
    }
  }
  Serial.println(inputPos);

  delay(250); // Simple debounce delay
}
