#include <stdio.h>
#include <stdlib.h>

// Function to compute points
double* generate_points(int n_points, double x0, double y0, double h) {
    static double results[20000]; // Max points: 10000 (x, y pairs)
    double x = x0, y = y0;

    for (int i = 0; i < n_points; i++) {
        results[2 * i] = x;       // Store x value
        results[2 * i + 1] = y;   // Store y value

        double denom = x * x - x * y - y * y; // Avoid division by zero
        if (denom != 0.0) {
            double dydx = (-y * y) / denom;
            y = y + h * dydx;
        }
        x = x + h; // Update x
    }
    return results;
}

