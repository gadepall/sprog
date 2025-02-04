#include <math.h>
#include <stdio.h>

// Function definition for f(x)
double f(double x) {
    return  x * x - 3*x - 4;
}

// Derivative definition for f'(x)
double f_prime(double x) {
    return 2 * x - 3;
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

int main() {
    double initial_guess = -1.0;
    double tolerance = 1e-6;
    int max_iterations = 100;

    // Using Newton-Raphson method
    double root_newton = newton_raphson(initial_guess, tolerance, max_iterations);
    if (!isnan(root_newton)) {
        printf("Root found using Newton-Raphson method: %f\n", root_newton);
    } else {
        printf("Newton-Raphson method did not converge.\n");
    }

    return 0;
}

