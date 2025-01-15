#include <stdio.h>
#include <math.h>

// Function to calculate the derivative of f(x) = |x + 2| - 1
double gradient(double x) {
    if (x > -2) {
        return 1; // Gradient for x > -2
    } else if (x < -2) {
        return -1; // Gradient for x < -2
    } else {
        return 0; // Gradient is undefined at x = -2 (corner point)
    }
}

// Function to perform gradient descent to find the minimum x
double find_minimum_x() {
    double x = 0.0;          // Initial value of x
    double learning_rate = 0.1; // Learning rate (step size)
    int max_iterations = 1000;  // Maximum number of iterations
    double tolerance = 1e-6;    // Tolerance for convergence

    for (int i = 0; i < max_iterations; i++) {
        double grad = gradient(x); // Calculate the gradient

        // Stop if x is close enough to -2 (minimum point)
        if (fabs(x + 2) < tolerance) {
            break;
        }

        // Update x based on the gradient (descent direction)
        x = x - learning_rate * grad;
    }

    return x; // Return the value of x
}

// Main function to test the gradient descent
int main() {
    double minimum_x = find_minimum_x();
    double minimum_y = fabs(minimum_x + 2) - 1; // Calculate y at the minimum x

    printf("The minimum value of y is %lf at x = %lf\n", minimum_y, minimum_x);
    return 0;
}

