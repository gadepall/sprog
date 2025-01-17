#include <stdio.h>
#include <math.h>

// Function to evaluate the quadratic equation f(x) = ax^2 + bx + c
double func(double x, double a, double b, double c) {
    return a * x * x + b * x + c;
}

// Derivative of the quadratic equation f'(x) = 2ax + b
double func_deriv(double x, double a, double b) {
    return 2 * a * x + b;
}

// Newton-Raphson method to find a single root
int newton_raphson(double a, double b, double c, double initial_guess, double tol, int max_iter, double* root) {
    double x0 = initial_guess;
    double x1;
    int iter = 0;

    while (iter < max_iter) {
        double fx = func(x0, a, b, c);
        double dfx = func_deriv(x0, a, b);

        if (fabs(dfx) < tol) {
            return 0; // Failure due to derivative near zero
        }

        x1 = x0 - fx / dfx;

        if (fabs(x1 - x0) < tol) {
            *root = x1;
            return 1; // Success
        }

        x0 = x1;
        iter++;
    }

    return 0; // Failure due to lack of convergence
}

