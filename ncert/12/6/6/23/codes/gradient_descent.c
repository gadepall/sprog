#include <stdio.h>
#include <math.h>

#define LEARNING_RATE 0.01
#define ITERATIONS 1000
#define TOLERANCE 1e-6

// Function to calculate the gradient
void gradients(double x0, double y0, double *grad_x0, double *grad_y0) {
    double normal_slope = -2.0 / x0;
    double normal_intercept = (2 * x0 / x0) + y0;
    double dist = normal_slope * 1 + normal_intercept - 2;
    double denom = sqrt(normal_slope * normal_slope + 1);

    *grad_x0 = (2 * dist * (-1 / (x0 * x0))) / denom;
    *grad_y0 = (2 * dist) / denom;
}

// Function for gradient descent
void gradient_descent(double x0_init, double y0_init, double *x0_result, double *y0_result) {
    double x0 = x0_init, y0 = y0_init;
    double grad_x0, grad_y0;

    for (int i = 0; i < ITERATIONS; i++) {
        gradients(x0, y0, &grad_x0, &grad_y0);
        x0 -= LEARNING_RATE * grad_x0;
        y0 -= LEARNING_RATE * grad_y0;

        // Stop if gradients are small (convergence)
        if (fabs(grad_x0) < TOLERANCE && fabs(grad_y0) < TOLERANCE) {
            break;
        }
    }

    *x0_result = x0;
    *y0_result = y0;
}

// Exported function to be used in Python
__attribute__((visibility("default"))) __attribute__((used))
void get_normal(double *x0_result, double *y0_result) {
    gradient_descent(2.0, 1.0, x0_result, y0_result);
}

