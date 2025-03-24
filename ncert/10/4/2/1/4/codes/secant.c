// Secant Method 

#include <stdio.h>
#include <math.h>

// Define the function f(x) = 16x^2 - 8x + 1
double f(double x) {
    return 16 * pow(x, 2) - 8 * x + 1;
}

int main(void) {
    double x0 = 0.5, x1 = 0.4; // Initial guesses
    double x2, tolerance = 1e-6;
    int max_iterations = 100, i = 1;

    printf("Iteration\t x_n\t\t f(x_n)\n");
    printf("%d\t\t %.10lf\t %.10lf\n", i, x0, f(x0));
    printf("%d\t\t %.10lf\t %.10lf\n", i + 1, x1, f(x1));

    // Secant Method Iterations
    for (i = 2; i < max_iterations; i++) {
        // Secant formula: x2 = x1 - f(x1) * (x1 - x0) / (f(x1) - f(x0))
        x2 = x1 - f(x1) * (x1 - x0) / (f(x1) - f(x0));

        printf("%d\t\t %.10lf\t %.10lf\n", i, x2, f(x2));

        // Check convergence
        if (fabs(x2 - x1) < tolerance) {
            printf("Converged at iteration %d: x = %.10lf\n", i, x2);
            break;
        }

        // Update variables for the next iteration
        x0 = x1;
        x1 = x2;
    }

    if (i == max_iterations) {
        printf("Failed to converge within %d iterations.\n", max_iterations);
    }

    return 0;
}

