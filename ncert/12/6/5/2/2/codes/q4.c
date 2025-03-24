#include <stdio.h>
#include <math.h>

// Function to calculate the derivative of y = -|x + 1| + 3
double gradient(double x) {
    if (x > -1) {
        return -1; // Gradient for x > -1
    } else if (x < -1) {
        return 1; // Gradient for x < -1
    } else {
        return 0; // Gradient is zero at x = -1
    }
}

// Function to perform gradient ascent and return the optimal x
double find_maximum_x() {
    double x = 4.0;       // Initial value of x
    double learning_rate = 0.1; // Learning rate (step size)
    int max_iterations = 1000;  // Maximum number of iterations
    double tolerance = 1e-6;    // Tolerance for convergence

    for (int i = 0; i < max_iterations; i++) {
        double grad = gradient(x); // Calculate the gradient

        // Stop if x is close enough to -1 (maximum point)
        if (fabs(x + 1) < tolerance) {
            break;
        }

        // Update x based on the gradient (ascent direction)
        x = x + learning_rate * grad;
    }

    return x; // Return the value of x
}

// Main function to test the gradient ascent
int main() {
    double maximum_x = find_maximum_x();
    double maximum_y = -fabs(maximum_x + 1) + 3; // Calculate y at the maximum x

    printf("The maximum value of y is %lf at x = %lf\n", maximum_y, maximum_x);
    return 0;
}

