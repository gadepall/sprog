#include <stdio.h>
#include <math.h>

// Constants
#define L 10.0  // cm

// Function to calculate volume directly in terms of r and sqrt(l^2 - r^2)
double calculate_volume(double r) {
    // Volume = (1/3) * pi * r^2 * (l^2 - r^2)^(1/2)
    return (1.0 / 3.0) * M_PI * r * r * sqrt(L * L - r * r);
}

// Gradient descent function to find max and min volume
void gradient_descent(double *optimal_r_max, double *optimal_volume_max, double *optimal_r_min, double *optimal_volume_min) {
    double r_max = 1.0;  // Initial guess for r (starting point for max)
    double r_min = 1.0;  // Initial guess for r (starting point for min)
    double learning_rate = 0.01;
    double tolerance = 1e-6;
    double max_iterations = 10000;

    // Maximize volume using gradient descent
    for (int i = 0; i < max_iterations; i++) {
        // Calculate the volume
        double volume_max = calculate_volume(r_max);
        // Derivative of the volume with respect to r: dV/dr = (1/3) * pi * [2r * sqrt(L^2 - r^2) - r^3 / sqrt(L^2 - r^2)]
        double dV_dr_max = (1.0 / 3.0) * M_PI * (2 * r_max * sqrt(L * L - r_max * r_max) - r_max * r_max * r_max / sqrt(L * L - r_max * r_max));
        double r_max_new = r_max + learning_rate * dV_dr_max;  // Update rule
        if (fabs(r_max_new - r_max) < tolerance) break;  // Check for convergence
        r_max = r_max_new;
    }

    // Minimize volume using gradient descent
    for (int i = 0; i < max_iterations; i++) {
        // Calculate the volume
        double volume_min = calculate_volume(r_min);
        // Derivative of the volume with respect to r: dV/dr = (1/3) * pi * [2r * sqrt(L^2 - r^2) - r^3 / sqrt(L^2 - r^2)]
        double dV_dr_min = (1.0 / 3.0) * M_PI * (2 * r_min * sqrt(L * L - r_min * r_min) - r_min * r_min * r_min / sqrt(L * L - r_min * r_min));
        double r_min_new = r_min - learning_rate * dV_dr_min;  // Update rule
        if (fabs(r_min_new - r_min) < tolerance) break;  // Check for convergence
        r_min = r_min_new;
    }

    // Store the optimal values
    *optimal_r_max = r_max;
    *optimal_volume_max = calculate_volume(r_max);

    *optimal_r_min = r_min;
    *optimal_volume_min = calculate_volume(r_min);
}

int main() {
    double optimal_r_max, optimal_volume_max;
    double optimal_r_min, optimal_volume_min;

    // Call gradient descent to find max and min volume
    gradient_descent(&optimal_r_max, &optimal_volume_max, &optimal_r_min, &optimal_volume_min);

    // Output the results of gradient descent
    printf("Max volume found at r = %f cm with volume = %f cm^3\n", optimal_r_max, optimal_volume_max);
    printf("Min volume found at r = %f cm with volume = %f cm^3\n", optimal_r_min, optimal_volume_min);

    return 0;
}

