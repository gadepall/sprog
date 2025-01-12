#include <stdio.h>
#include <math.h>

// Function to calculate the value of y = 1 / (x^2 + 2)
double function(double x) {
    return 1.0 / (x * x + 2);
}

// Function to calculate the derivative of y = 1 / (x^2 + 2)
double gradient(double x) {
    return -2.0 * x / pow(x * x + 2, 2); // Derivative of 1 / (x^2 + 2)
}

// Function to perform gradient ascent and return the optimal x
double find_maximum_x() {
    double x = -3.0;       // Initial value of x (starting at x = -3)
    double learning_rate = 0.1; // Learning rate (step size)
    int max_iterations = 1000;  // Maximum number of iterations
    double tolerance = 1e-6;    // Tolerance for convergence

    for (int i = 0; i < max_iterations; i++) {
        double grad = gradient(x); // Calculate the gradient
        x = x + learning_rate * grad; // Update x (gradient ascent step)
        if (fabs(grad) < tolerance) {
            break; // Stop if gradient is small
        }
    }
    
    return x; // Return the value of x where maximum occurs
}

int main() {
    double x_max = find_maximum_x(); // Find the x that maximizes the function
    double y_max = function(x_max); // Calculate the maximum value of the function
    printf("Maximum occurs at x = %.6f\n", x_max);
    printf("Maximum value of the function is y = %.6f\n", y_max);
    return 0;
}

