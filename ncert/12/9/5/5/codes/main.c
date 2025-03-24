#include <stdio.h>
#include <math.h>

// Step size
float h = 0.00001;

// Derivative function
float derivative(float x, float y) {
    if (x == 0.0) {
        printf("Error: Division by zero in derivative.\n");
        return 0.0; // Handle division by zero gracefully
    }
    return (x * x - 2 * y * y + x * y) / (x * x);
}

// Solution function using Euler's method
void solution(float *x, float *y, int n) {
    for (int i = 1; i <= n; i++) {
        *y += derivative(*x, *y) * h; // Increment y
        *x += h;                      // Increment x
    }
}


