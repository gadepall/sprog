#include <stdio.h>
#include <math.h>

// Function declaration
double newton_method(double x0, double tolerance, int max_iter);

// Implementing the function to be exported
double newton_method(double x0, double tolerance, int max_iter) {
    double x = x0; // Initial guess
    int iter = 0;

    while (iter < max_iter) {
        double f = x * x - 7 * x - 60;         // The function f(x) = x^2 - 7x - 60
        double f_prime = 2 * x - 7;           // Its derivative f'(x) = 2x - 7

        if (fabs(f_prime) < 1e-10) {          // Avoid division by zero
            printf("Derivative too small, stopping.\n");
            return x;
        }

        double x_new = x - f / f_prime;       // Newton's method update
        if (fabs(x_new - x) < tolerance) {   // Convergence check
            return x_new;
        }

        x = x_new;
        iter++;
    }

    printf("Max iterations reached.\n");
    return x;
}

// Main function to test the Newton's method implementation
int main() {
    double initial_guess = 10.0;  // Initial guess for root
    double tolerance = 1e-6;      // Tolerance for convergence
    int max_iter = 100;           // Maximum number of iterations

    double root = newton_method(initial_guess, tolerance, max_iter);

    printf("Root of the equation: %.6f\n", root);

    return 0;
}

