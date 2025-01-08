#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to compute secant difference
double secdif(double y, double r) {
    return -(sin(y) + 2 * r); // Finding the value of y"
}

// Function that will compute the solution
void sol(double *x, double *y, double *v, double r, int n, double h) {
    for (int i = 0; i < n; i++) {
        *y += *v * h;               // Euler method for y
        *v += secdif(*y, *v) * h;    // Updating y' using secdif
        *x += h;                    // Increment position
    }
}

