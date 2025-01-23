#include <stdio.h>
#include <math.h>

// Absolute value function
double absolute(double a, double b) {
    return a > b ? a - b : b - a;
}

// Gradient descent function (for minimization)
void gradientdesc(int n, double x[n], double alpha, double x_0, double threshold) {
    x[0] = x_0; // Initial guess
    int i = 1;
    double x_prev = x_0; // Previous value

    while (i < n) {
        // Calculate next value
        x[i] = x_prev - alpha * (2 * (x_prev * x_prev - 1)) / pow((1 + x_prev + x_prev * x_prev), 2);

        // Check convergence using the threshold
        if (absolute(x[i], x_prev) <= threshold) {
            break;
        }

        // Update for next iteration
        x_prev = x[i];
        i++;
    }

    // Fill remaining values with the last computed value if convergence is reached
    for (; i < n; i++) {
        x[i] = x_prev;
    }
}

// Gradient ascent function (for maximization)
void gradientasc(int n, double x[n], double alpha, double x_0, double threshold) {
    x[0] = x_0; // Initial guess
    int i = 1;
    double x_prev = x_0; // Previous value

    while (i < n) {
        // Calculate next value
        x[i] = x_prev + alpha * (2 * (x_prev * x_prev - 1)) / pow((1 + x_prev + x_prev * x_prev), 2);

        // Check convergence using the threshold
        if (absolute(x[i], x_prev) <= threshold) {
            break;
        }

        // Update for next iteration
        x_prev = x[i];
        i++;
    }

    // Fill remaining values with the last computed value if convergence is reached
    for (; i < n; i++) {
        x[i] = x_prev;
    }
}

