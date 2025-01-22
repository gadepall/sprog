#include <stdio.h>
#include <math.h>
#include <complex.h>

// Function to evaluate the polynomial f(x) = 4x^2 + 3x + 5
complex double f(complex double x) {
    return 4 * x * x + 3 * x + 5;
}

// Function to evaluate the derivative f'(x) = 8x + 3
complex double f_prime(complex double x) {
    return 8 * x + 3;
}

// Newton's method for finding roots of a polynomial
complex double newton_method(complex double initial_guess, double tol, int max_iter) {
    complex double x = initial_guess;
    for (int i = 0; i < max_iter; i++) {
        complex double fx = f(x);
        complex double fpx = f_prime(x);

        if (cabs(fpx) < tol) {
            printf("Derivative too small. Stopping iteration.\n");
            break;
        }

        complex double x_next = x - fx / fpx;

        if (cabs(x_next - x) < tol) {
            return x_next;
        }

        x = x_next;
    }
    return x;
}

// Test function
void find_roots(double tol, int max_iter, double *real_parts, double *imag_parts) {
    complex double guess1 = 0 + 1.0 * I; // Initial guess for one root
    complex double guess2 = 0 - 1.0 * I; // Initial guess for the other root

    complex double root1 = newton_method(guess1, tol, max_iter);
    complex double root2 = newton_method(guess2, tol, max_iter);

    printf("Root 1: %.6f + %.6fi\n", creal(root1), cimag(root1));
    printf("Root 2: %.6f + %.6fi\n", creal(root2), cimag(root2));

    // Store the results
    real_parts[0] = creal(root1);
    imag_parts[0] = cimag(root1);
    real_parts[1] = creal(root2);
    imag_parts[1] = cimag(root2);
}

