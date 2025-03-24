#include <stdio.h>
#include <math.h>

// Function to calculate the derivative of f(x) = sin(x) + cos(x)
double gradient(double x) {
    return cos(x) - sin(x); // Derivative of f(x)
}

// Function to perform gradient ascent and return the optimal x for maximum
double find_maximum_x() {
    double x = 0.5;        // Initial value of x (starting within the domain)
    double learning_rate = 0.1; // Learning rate (step size)
    int max_iterations = 1000;  // Maximum number of iterations
    double tolerance = 1e-6;    // Tolerance for convergence

    for (int i = 0; i < max_iterations; i++) {
        double grad = gradient(x); // Calculate the gradient

        // Stop if the gradient is close to zero (convergence)
        if (fabs(grad) < tolerance) {
            break;
        }

        // Update x based on the gradient (ascent direction)
        x = x + learning_rate * grad;

        // Ensure x stays within [0, pi]
        if (x < 0) x = 0;
        if (x > M_PI) x = M_PI;
    }

    return x; // Return the value of x for the maximum
}

// Function to perform gradient descent and return the optimal x for minimum
double find_minimum_x() {
    double x = 2.5;        // Initial value of x (starting within the domain)
    double learning_rate = 0.1; // Learning rate (step size)
    int max_iterations = 1000;  // Maximum number of iterations
    double tolerance = 1e-6;    // Tolerance for convergence

    for (int i = 0; i < max_iterations; i++) {
        double grad = gradient(x); // Calculate the gradient

        // Stop if the gradient is close to zero (convergence)
        if (fabs(grad) < tolerance) {
            break;
        }

        // Update x based on the gradient (descent direction)
        x = x - learning_rate * grad;

        // Ensure x stays within [0, pi]
        if (x < 0) x = 0;
        if (x > M_PI) x = M_PI;
    }

    return x; // Return the value of x for the minimum
}

// Main function to test the gradient ascent and gradient descent
int main() {
    // Find the x values for the maximum and minimum
    double maximum_x = find_maximum_x();
    double minimum_x = find_minimum_x();

    // Calculate the corresponding y values
    double maximum_y = sin(maximum_x) + cos(maximum_x); // f(x) at maximum
    double minimum_y = sin(minimum_x) + cos(minimum_x); // f(x) at minimum

    // Print the results
    printf("The maximum value of f(x) is %lf at x = %lf\n", maximum_y, maximum_x);
    printf("The minimum value of f(x) is %lf at x = %lf\n", minimum_y, minimum_x);

    return 0;
}
