#include <stdio.h>
#include<math.h>
int a=1;
float f(float x) {
    return sqrt(a*x);
}

void compute_area(float h, float *A) {
    *A = 0.0;  // Initialize the area to 0
    float x = 0.0;

    // Loop to compute the area using the trapezoidal rule
    while (x + h <= a) { 
        *A += 0.5 * h * (f(x) + f(x + h));  // Update the value pointed to by A
        x += h;
    }
}
