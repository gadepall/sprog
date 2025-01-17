#include <math.h>
#include <stdio.h>

// Function definition for f(x)
double f(double x) {
    return sqrt(2) * x * x + 7 * x + 5 * sqrt(2);
}

// Derivative definition for f'(x)
double f_prime(double x) {
    return 2 * sqrt(2) * x + 7;
}

// Newton-Raphson method
double newton_raphson(double initial_guess, double tolerance, int max_iterations) {
    double x = initial_guess;
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        double fx = f(x);
        double fpx = f_prime(x);
        if (fabs(fpx) < 1e-12) {  // Avoid division by zero
            return NAN; // Return NaN if derivative is too small
        }
        double x_new = x - fx / fpx;
        if (fabs(x_new - x) < tolerance) {  // Convergence check
            return x_new;
        }
        x = x_new;
    }
    return NAN; // Return NaN if no convergence
}

// Secant method
double secant_method(double x0, double x1, double tolerance, int max_iterations) {
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        double f_x0 = f(x0);
        double f_x1 = f(x1);
        if (fabs(f_x1 - f_x0) < 1e-12) {  // Avoid division by zero
            return NAN; // Return NaN if denominator is too small
        }
        double x2 = x1 - f_x1 * (x1 - x0) / (f_x1 - f_x0);
        if (fabs(x2 - x1) < tolerance) {  // Convergence check
            return x2;
        }
        x0 = x1;
        x1 = x2;
    }
    return NAN; // Return NaN if no convergence
}

