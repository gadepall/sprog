#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function definition for f(x) = |sin(4x) + 3|
double f(double x) {
    return fabs(sin(4 * x) + 3);
}

// Derivative of f(x), considering gradient descent and ascent are applied
// Note: Numerical approximation of the derivative is used
double df(double x, double delta) {
    return (f(x + delta) - f(x - delta)) / (2 * delta);
}

// Function to find local minima or maxima
// If is_max is 1, performs gradient ascent (maximum); if 0, performs gradient descent (minimum)
void find_extremum(double init_guess, double step_size, double tolerance, double delta, int is_max, double *extremum) {
    int count = 0;                 // Iteration counter
    double current_guess = init_guess;
    double grad;

    while (1) {
        grad = df(current_guess, delta); // Calculate numerical derivative
        if (fabs(grad) < tolerance) {
            break; // Stop when gradient magnitude is smaller than tolerance
        }
        if (is_max) {
            current_guess += step_size * grad; // Update using gradient ascent
        } else {
            current_guess -= step_size * grad; // Update using gradient descent
        }
        count++;
        if (count > 1000) { // Prevent infinite loop
            printf("Max iterations reached.\n");
            break;
        }
    }

    printf("Number of iterations: %d\n", count);
    *extremum = current_guess; // Store the found extremum point
}

// Free memory allocated for points array
void free_ptr(double *points) {
    free(points);
}

int main() {
    double step_size = 0.01;
    double tolerance = 1e-6;
    double delta = 1e-5;

    double init_guess_min = 0.5; // Initial guess for minimum
    double minimum;
    find_extremum(init_guess_min, step_size, tolerance, delta, 0, &minimum);
    printf("Local minimum found at x = %f, f(x) = %f\n", minimum, f(minimum));

    double init_guess_max = 0.0; // Initial guess for maximum
    double maximum;
    find_extremum(init_guess_max, step_size, tolerance, delta, 1, &maximum);
    printf("Local maximum found at x = %f, f(x) = %f\n", maximum, f(maximum));

    return 0;
}

