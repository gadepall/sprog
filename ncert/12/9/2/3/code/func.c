#include <stdio.h>
#include <math.h>

float h = 0.001;

// Derivative function
float derivative( float x) {
    return (-sin(x));
}

// Solution function
void solution(float *x, float *y, int n) {
    for (int i = 1; i <= n; i++) {
        // Calculate the cos(x)
        float cos_value = cos(*x);

        // Update y using the derivative 
        *y += derivative(*x) * h; 
        *x += h; // Increment x by h
    }
}
