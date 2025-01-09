#include <stdio.h>

float f(float x) {
    return -x * x + x - 2;
}

void compute_area(float h, float *A) {
    *A = 0.0;  // Initialize the area to 0
    float x = 0.0;

    // Loop to compute the area using the trapezoidal rule
    while (x + h <= 3.0) { 
        *A += 0.5 * h * (f(x) + f(x + h));  // Update the value pointed to by A
        x += h;
    }
}

