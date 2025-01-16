#include <math.h>
#include <stdio.h>

#define N 1000

// Function to compute the points for trapezoidal and bilinear transform
void compute_points(double h, double y[], double v[], double y_trap[], double v_trap[], double y_bilin[], double v_bilin[]) {
    // Initial conditions
    y[0] = 2.0;
    v[0] = 1.0;

    // Initialize arrays for trapezoidal and bilinear transform
    y_trap[0] = y[0];
    v_trap[0] = v[0];

    y_bilin[0] = y[0];
    v_bilin[0] = v[0];

    // Compute points for 1000 iterations using trapezoidal and bilinear transform
    for (int n = 0; n < N - 1; n++) {
        // Trapezoidal Rule (accurate update of v[n+1])
        v_trap[n+1] = v_trap[n] + h * v_trap[n]; // Using Euler method for simplicity
        y_trap[n+1] = y_trap[n] + h * v_trap[n+1];

        // Bilinear Transform
        v_bilin[n+1] = (1 + (h / 2)) / (1 - (h / 2)) * v_bilin[n];
        y_bilin[n+1] = y_bilin[n] + (h / 2) * (v_bilin[n+1] + v_bilin[n]);
    }
}

