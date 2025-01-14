#include <stdio.h>
#include <math.h>

// Functions to define the curves
double circle1_top(double x) {
    return sqrt(4 - x * x);
}

double circle2_top(double x) {
    return sqrt(4 - pow(x - 2, 2));
}

// Numerical integration for the enclosed area
double Area(double x1, double x2) {
    double n = 300000; // Number of slices
    double h = fabs(x1 - x2) / n; // Slice width
    double A = 0; // Accumulated area
    double x = fmin(x1, x2);

    for (int i = 0; i < n; i++) {
        double y_diff = circle1_top(x) - circle2_top(x); // Difference between curves
        A += fabs(y_diff) * h; // Increment the area
        x += h;
    }
    return fabs(A);
}
