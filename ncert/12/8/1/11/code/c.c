#include <stdio.h>
#include <math.h>

// Function to calculate y = sqrt(4x) based on y^2 = 4x
double curve_function(double x) {
    return sqrt(4 * x);
}

// Function to calculate the total area under the curve using the trapezoidal rule
__attribute__((visibility("default")))
__attribute__((used))
double compute_total_area(double a, double b, int n) {
    double h = (b - a) / n;  // Step size
    double total_area = 0.0;

    // Calculate the total area using the trapezoidal rule
    for (int i = 0; i < n; i++) {
        double x1 = a + i * h;
        double x2 = a + (i + 1) * h;
        double y1 = curve_function(x1);
        double y2 = curve_function(x2);
        total_area += (y1 + y2) * h / 2.0;  // Add area of each trapezoid
    }

    return total_area;
}

