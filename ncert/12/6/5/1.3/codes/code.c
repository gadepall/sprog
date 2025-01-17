#include <stdio.h>
#include <math.h>

// Function to calculate the derivative of y = -(x - 1)^2 + 10
double gradient(double x) {
    return -2 * (x - 1);
}

// Function to perform gradient ascent and return the optimal x
double find_maximum_x() {
    double x = 0.0;        // Initial value of x
    double learning_rate = 0.1; // Learning rate (step size)
    int max_iterations = 1000;  // Maximum number of iterations
    double tolerance = 1e-6;    // Tolerance for convergence

    for (int i = 0; i < max_iterations; i++) {
        double grad = gradient(x); // Calculate the gradient
        x = x + learning_rate * grad; // Update x (gradient ascent)
        if (fabs(grad) < tolerance) {
            break; // Stop if gradient is small
        }
    }
    
    return x; // Return the value of x
}

int main() {
    // Find the maximum value of x
    double max_x = find_maximum_x();
    double max_y = -(pow(max_x - 1, 2)) + 10; // Calculate the maximum value of y

    // Print the results
    printf("Maximum value found at:\n");
    printf("x = %.6f, y = %.6f\n", max_x, max_y);

    return 0;
}



