#include <math.h>

// Function to compute y = x * sin(x)
double compute_y(double x) {
    return x * sin(x);
}

// Function to compute dy/dx = sin(x) + x * cos(x)
double compute_dydx(double x) {
    return sin(x) + x * cos(x);
}

