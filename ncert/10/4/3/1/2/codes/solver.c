#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "functions.h"
#include <stdint.h>
#include <openssl/rand.h>

// Function to solve a polynomial of a given degree and return its roots
double complex *solve_polynomial(int degree, double complex A[degree+1]) {
    // Check if the leading coefficient is zero
    if (cabs(A[0]) < 1e-12) {
        fprintf(stderr, "Error: Coefficient 'an' cannot be zero.\n");
        return NULL;  // Return NULL to indicate failure
    }

    // Create a companion matrix to find polynomial roots
    double complex **matrix = createMat(degree, degree);

    // Populate the companion matrix
    for (int i = 0; i < degree - 1; i++) {
        matrix[i][i + 1] = 1;  // Set subdiagonal elements to 1
    }
    for (int i = 0; i < degree; i++) {
        matrix[degree - 1][i] = -A[degree - i] / A[0];  // Set last row based on coefficients
    }

    // Find and return the eigenvalues of the companion matrix (roots of the polynomial)
    return eigenvalues(matrix, degree);
}

// Function to solve a quadratic equation ax^2 + bx + c = 0
double complex *solve_quadratic(double complex a, double complex b, double complex c) {
    // Create an array of coefficients for the quadratic polynomial
    double complex coeffs[3] = {a, b, c};

    // Compute the roots using the general polynomial solver
    double complex *result = solve_polynomial(2, coeffs);

    return result;  // Return the roots
}

// Function to generate a random number in the range [min, max]
double randomgen(double min, double max) {
    // Check for invalid range
    if (min > max) {
        fprintf(stderr, "Invalid range: min should be less than or equal to max.\n");
        return -1;  // Return error indicator
    }

    // Generate 8 random bytes
    unsigned char random_bytes[8];
    uint64_t random_value;
    if (RAND_bytes(random_bytes, sizeof(random_bytes)) != 1) {
        fprintf(stderr, "Error generating random bytes\n");
        return -1;  // Return error indicator
    }

    // Interpret the random bytes as a 64-bit integer
    random_value = *((uint64_t *)random_bytes);

    // Normalize to the range [0, 1], then scale to [min, max]
    double normalized = (random_value / (double)(UINT64_MAX));
    double scaled_value = normalized * (max - min) + min;
    return scaled_value;  // Return the random number
}

// Function to find a root of the quadratic equation using Newton-Raphson's method
double complex newton_raphson(double complex a, double complex b, double complex c) {
    // Small constant used for perturbation if the derivative becomes too small
    double complex epsilon = 1e-3 + (1e-3) * I;

    // Helper function to compute the derivative of the quadratic equation
    double complex quadder(double complex x) {
        return 2 * a * x + b;  // Derivative: 2ax + b
    }

    // Helper function to compute the quadratic equation value
    double complex quad(double complex x) {
        return a * x * x + b * x + c;  // Equation: ax^2 + bx + c
    }

    // Generate a random initial guess for the root
    double complex x_n = randomgen(-(cabs(a) + cabs(b) + cabs(c)), cabs(a) + cabs(b) + cabs(c)) +
                         randomgen(-(cabs(a) + cabs(b) + cabs(c)), cabs(a) + cabs(b) + cabs(c)) * I;

    // Iteratively improve the root estimate using Newton-Raphson's method
    while (cabs(quad(x_n)) >= 1e-8) {  // Continue until the function value is close to zero
        if (cabs(quadder(x_n)) < 1e-13) {  // Check if the derivative is too small
            x_n += epsilon;  // Perturb the guess to avoid division by zero
        }
        printf("This is x_n now, %lf\n",creal(x_n));
        x_n = x_n - (quad(x_n) / quadder(x_n));  // Update the estimate using Newton-Raphson
    }

    return x_n;  // Return the computed root
}
