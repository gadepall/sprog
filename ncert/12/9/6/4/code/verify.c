#include <stdio.h>
#include <math.h>

float h = 0.001;

// Derivative function
float derivative(float y, float x) {
    return (tan(x))-y*(1.0/cos(x)) ;
}

// Solution function
void solution(float *x, float *y, int n) {
    for (int i = 1; i <= n; i++) {
       
        // Update y using the derivative 
        *y += derivative(*y, *x) * h; 
        *x += h; // Increment x by h
    }
}

