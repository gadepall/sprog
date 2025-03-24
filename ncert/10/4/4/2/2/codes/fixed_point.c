#include <stdio.h>
#include <math.h>

// Define the function g(x) for fixed-point iteration.
// Rearrange x^2 - 2x + 1 = 0 to x = g(x).
// For example, x = sqrt(2x - 1) can be one such g(x).
#define g(x) (sqrt(2 * (x) - 1))

// Function to perform the fixed-point iteration
void fixed_point_iteration(double initial_guess, double tolerance, int max_iterations) {
    double x0 = initial_guess, x1; // Initial guess and next approximation
    int iteration = 0;            // Iteration counter

    do {
        x1 = g(x0); // Compute the next approximation

        // Print iteration details
        printf("Iteration %d: x0 = %.7f, x1 = %.7f\n", iteration + 1, x0, x1);

        iteration++;

        // Check if convergence is achieved
        if (fabs(x1 - x0) < tolerance) {
            printf("\nThe root is approximately: %.7f\n", x1);
            printf("Converged in %d iterations.\n", iteration);
            return;
        }

        x0 = x1; // Update x0 for the next iteration

    } while (iteration < max_iterations);

    // If the method fails to converge within max_iterations
    printf("\nThe method did not converge within %d iterations.\n", max_iterations);
}

int main() {
    double initial_guess;
    double tolerance = 1e-6;      // Tolerance for convergence
    int max_iterations = 1000;   // Maximum number of iterations

    // Prompt user for initial guess
    printf("Enter the initial guess: ");
    scanf("%lf", &initial_guess);

    // Call the fixed-point iteration function
    fixed_point_iteration(initial_guess, tolerance, max_iterations);

    return 0;
}

