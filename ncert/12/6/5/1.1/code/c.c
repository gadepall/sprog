#include <stdio.h>
#include <math.h>

// Function to calculate the derivative of y = (2x - 1)^2 + 3
double gradient(double x) {
    return 4 * (2 * x - 1);
}

// Function to perform gradient descent and return the optimal x
double find_minimum_x() {
    double x = 4.0;       // Initial value of x
    double learning_rate = 0.1; // Learning rate (step size)
    int max_iterations = 1000;  // Maximum number of iterations
    double tolerance = 1e-6;    // Tolerance for convergence

    for (int i = 0; i < max_iterations; i++) {
        double grad = gradient(x); // Calculate the gradient
        x = x - learning_rate * grad; // Update x
        if (fabs(grad) < tolerance) {
            break; // Stop if gradient is small
        }
    }
    
    return x; // Return the value of x
}

