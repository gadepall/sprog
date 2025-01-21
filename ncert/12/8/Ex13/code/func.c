#include <stdio.h>
#include <math.h>
#include <stdlib.h> // For fabs

// Function to calculate area under |cos(x)| using Trapezoidal rule
double trapezoidal_mod_cos(double x1, double x2, int n) {
    double h = fabs(x2 - x1) / n;
    double A = 0;

    // Correctly handle the case where x1 > x2
    double start = fmin(x1, x2);
    double end = fmax(x1, x2);
    
    // Optimization: Pre-calculate cos for endpoints
    double f_start = fabs(cos(start));  // Take absolute value of cos(x)
    double f_end = fabs(cos(end));      // Take absolute value of cos(x)
    
    A = (f_start + f_end) / 2.0; // First and last terms are divided by 2
    
    for (int i = 1; i < n; i++) {
        double x = start + i * h;
        A += fabs(cos(x));  // Take absolute value of cos(x)
    }
    A *= h;
    
    return fabs(A); // Return absolute value for area
}

