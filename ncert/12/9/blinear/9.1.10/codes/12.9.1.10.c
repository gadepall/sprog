#include <stdio.h>
#include <math.h>

#define H 0.01

// Function to compute v' = -2v - sin(y)
double secdif(double y, double v) {
    return -2 * v - sin(y);
}

// Euler's method for solving the system
void euler_method(double *x, double *y, double *v, double r, int n, double h) {
    for (int i = 0; i < n; i++) {
        *y += *v * h;              // Euler method for y
        *v += secdif(*y, *v) * h;  // Euler method for v
        *x += h;                   // Increment x
    }
}

// Trapezoidal method for solving the system
void trapezoidal_method(double *x, double *y, double *v, double r, int n, double h) {
    double y_next, v_next;
    
    for (int i = 0; i < n; i++) {
        // Euler's prediction step
        y_next = *y + *v * h;
        v_next = *v + secdif(*y, *v) * h;

        // Trapezoidal correction step
        *y = *y + 0.5 * h * (*v + v_next);
        *v = *v + 0.5 * h * (secdif(*y, *v) + secdif(y_next, v_next));

        *x += h;
    }
}

