#include <stdio.h>
#include <math.h>

// Function to compute f(b) = b^2 - 400
double f(double b) {
    return b * b - 400;
}

// Derivative of f(b), f'(b) = 2b
double f_prime(double b) {
    return 2 * b;
}

// Newton-Raphson Method
double newton_raphson(double initial_guess, double tolerance, int max_iter) {
    double b = initial_guess;
    for (int i = 0; i < max_iter; i++) {
        double fb = f(b);
        double fpb = f_prime(b);

        // Avoid division by zero
        if (fabs(fpb) < 1e-10) {
            printf("Derivative is near zero. Method failed.\n");
            return b;
        }

        // Update using Newton-Raphson formula
        double b_next = b - fb / fpb;

        // Check for convergence
        if (fabs(b_next - b) < tolerance) {
            return b_next;
        }

        b = b_next;
    }

    printf("Method did not converge within %d iterations.\n", max_iter);
    return b;
}

// Generate y values for the quadratic equation for plotting
void generate_quadratic_values(double *x_values, double *y_values, int size) {
    for (int i = 0; i < size; i++) {
        y_values[i] = x_values[i] * x_values[i] - 400; // f(b) = b^2 - 400
    }
}

