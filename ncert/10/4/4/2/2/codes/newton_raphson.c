#include <stdio.h>
#include <math.h>

// Function f(x) = x^2 - 2x + 1
double f(double x) {
    return x * x - 2 * x + 1;
}

// Derivative f'(x) = 2x - 2
double f_prime(double x) {
    return 2 * x - 2;
}

// Newton-Raphson method
double newton_raphson(double x0, double tolerance, int max_iterations) {
    double x = x0;
    int iteration = 0;
    
    while (iteration < max_iterations) {
        double fx = f(x);
        double fx_prime = f_prime(x);

        // Avoid division by zero
        if (fx_prime == 0) {
            printf("Derivative is zero, cannot proceed with Newton-Raphson method.\n");
            return x;
        }

        // Update x using Newton-Raphson formula
        double x_new = x - fx / fx_prime;

        // Check for convergence
        if (fabs(x_new - x) < tolerance) {
            return x_new;
        }

        x = x_new;
        iteration++;
    }

    printf("Max iterations reached, method did not converge.\n");
    return x;
}

int main() {
    double x0 = 0.5;  // Initial guess
    double tolerance = 1e-6;  // Convergence tolerance
    int max_iterations = 100;  // Maximum number of iterations

    double root = newton_raphson(x0, tolerance, max_iterations);
    printf("Root: %.6f\n", root);

    return 0;
}

