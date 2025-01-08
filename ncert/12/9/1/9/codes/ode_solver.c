#include <stdio.h>
#include <stdlib.h>

void solve_ode(double y0, double y_prime0, double dt, double t_max, double* t_values, double* y_values, int size) {
    // Initialize variables
    y_values[0] = y0;
    double y_prime = y_prime0;

    // Numerical integration using Euler's method
    for (int i = 1; i < size; i++) {
        double t = t_values[i - 1];
        double y = y_values[i - 1];
        double y_double_prime = -(y_prime * y_prime) - 2 * y;

        // Update y and y'
        double y_new = y + y_prime * dt;
        y_prime = y_prime + y_double_prime * dt;

        // Store result
        y_values[i] = y_new;
    }
}
