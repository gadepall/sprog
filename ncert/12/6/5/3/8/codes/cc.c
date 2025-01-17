#include <stdio.h>
#include <math.h>

// Function to calculate the value of y = x * sqrt(1 - x)
double function(double x) {
    return x * sqrt(1 - x);
}

// Function to calculate the derivative of y = x * sqrt(1 - x)
double gradient(double x) {
    return sqrt(1 - x) - x / (2 * sqrt(1 - x)); // Derivative of x * sqrt(1 - x)
}

// Function to perform gradient ascent and return the optimal x
double find_maximum_x() {
    double x = 0.5;            // Initial value of x (starting at x = 0.5)
    double learning_rate = 0.01; // Learning rate (step size)
    int max_iterations = 1000;  // Maximum number of iterations
    double tolerance = 1e-6;    // Tolerance for convergence

    for (int i = 0; i < max_iterations; i++) {
        double grad = gradient(x); 
        x = x + learning_rate * grad; 

        if (x < 0) {
            x = 0;
        } else if (x > 1) {
            x = 1;
        }

        if (fabs(grad) < tolerance) {
            break; // Stop if gradient is small
        }
    }

    return x; 
}

int main() {
    double x_max = find_maximum_x(); 
    double y_max = function(x_max); 
    printf("Maximum occurs at x = %.6f\n", x_max);
    printf("Maximum value of the function is y = %.6f\n", y_max);
    return 0;
}

