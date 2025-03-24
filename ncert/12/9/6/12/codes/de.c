#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Function to calculate dx/dy for the equation (x + 3y^2) dy/dx = y
float dx_dy(float x, float y) {
    float numerator = x + 3 * y * y;
    float denominator = y;

    // Avoid division by zero or near-zero values in the denominator
    if (fabs(denominator) < 1e-6) {
        printf("Warning: Division by zero encountered at x = %.3f, y = %.3f\n", x, y);
        return 0.0;
    }
    return numerator / denominator;
}

// Function to calculate points using Euler's method and return them through arrays
void points(float x_0, float y_0, float y_end, float h, float *x_points, float *y_points, int steps) {
    float x_n = x_0;
    float y_n = y_0;

    for (int i = 0; i < steps; i++) {
        y_points[i] = y_n;
        x_points[i] = x_n;

        float y_n1 = y_n + h;
        float x_n1 = x_n + h * dx_dy(x_n, y_n);

        y_n = y_n1;
        x_n = x_n1;
    }
}

// Main function (for testing purposes only)
int main() {
    float x_0 = 1.0;      // Initial x value
    float y_0 = 1.0;      // Initial y value
    float y_end = 5.0;    // Final y value
    float step_size = 0.001;
    int steps = (int)((y_end - y_0) / step_size) + 1;

    // Allocate memory for arrays to store points
    float *x_points = (float *)malloc(steps * sizeof(float));
    float *y_points = (float *)malloc(steps * sizeof(float));

    if (x_points == NULL || y_points == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Call the points function
    points(x_0, y_0, y_end, step_size, x_points, y_points, steps);


    // Free allocated memory
    free(x_points);
    free(y_points);

    return 0;
}

