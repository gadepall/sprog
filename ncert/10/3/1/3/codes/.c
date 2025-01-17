#include <stdio.h>
#include <math.h>

// Function declaration
double newton_method(double x0, double tolerance, int max_iter);

// Implementing the function to be exported
double newton_method(double x0, double tolerance, int max_iter) {
    double x = x0;
    int iter = 0;

    while (iter < max_iter) {
        double f = 100 * x * x - 20 * x + 1;         // The function f(x) = 100x^2 - 20x + 1
        double f_prime = 200 * x - 20;              // Its derivative f'(x) = 200x - 20

        if (fabs(f_prime) < 1e-10) {                // Avoid division by zero
            printf("Derivative too small, stopping.\n");
            return x;
        }

        double x_new = x - f / f_prime;             // Newton's method update
        if (fabs(x_new - x) < tolerance) {          // Convergence check
            return x_new;
        }

        x = x_new;
        iter++;
    }

    printf("Max iterations reached.\n");
    return x;
}

