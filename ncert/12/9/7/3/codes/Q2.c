#include <stdio.h>
#include <math.h>

double h = 0.0001;

// Derivative function
double derivative(double y, double x) {
    return (-(x*x)+(2*y*y))/(4*x*y);
}

// Solution function
void solution(double *x, double *y, int n) {
    for (int i = 1; i <= n; i++) {
                // Update y using the derivative 
        *y += derivative(*y, *x) * h; 
        *x += h; // Increment x by h
    }
}
