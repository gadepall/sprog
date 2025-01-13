#include <stdio.h>
#include <math.h>

// Function to calculate the derivative of g(x) = x^3 - 3x
double gradient(double x) {
    return 3 * x * x - 3;
}

// Function to calculate g(x) = x^3 - 3x
double function(double x) {
    return x * x * x - 3 * x;
}

// Function to find critical points using gradient ascent or descent
double find_critical_point(double x, double learning_rate, int max_iterations, double tolerance, int ascent) {
    for (int i = 0; i < max_iterations; i++) {
        double grad = gradient(x);
        if (ascent) {
            x = x + learning_rate * grad; // Update x for gradient ascent
        } else {
            x = x - learning_rate * grad; // Update x for gradient descent
        }
        if (fabs(grad) < tolerance) {
            break; // Stop if gradient is small
        }
    }
    return x;
}

int main() {
    double learning_rate = 0.1;  // Learning rate (step size)
    int max_iterations = 1000;  // Maximum number of iterations
    double tolerance = 1e-6;    // Tolerance for convergence

    // Finding local minima
    double x_min_start = 0.5; // Initial guess near the minimum
    double x_min = find_critical_point(x_min_start, learning_rate, max_iterations, tolerance, 0);
    double g_min = function(x_min);

    // Finding local maxima
    double x_max_start = -1.5; // Initial guess near the maximum
    double x_max = find_critical_point(x_max_start, learning_rate, max_iterations, tolerance, 1);
    double g_max = function(x_max);

    // Printing results
    printf("Local Minimum:\n");
    printf("x = %.6f, g(x) = %.6f\n", x_min, g_min);

    printf("Local Maximum:\n");
    printf("x = %.6f, g(x) = %.6f\n", x_max, g_max);

    return 0;
}

