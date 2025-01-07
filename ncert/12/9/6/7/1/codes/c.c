#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Function to calculate dy/dx based on the given equation
float dy_dx(float x, float y) {
    // Calculate the derivative of the given equation y = (-2/x)(1 + 1/ln(x))
    float term1 = 2 / (x * x) ;  // First term of the derivative
    float term2 = 2 / (x * x * log(x));             // Second term of the derivative
    float term3 = 2 / (x * x * log(x) * log(x));
    return term1 + term2 + term3;
}

// Function to calculate points using Euler's method and return them through arrays
void points(float x_0, float y_0, float x_end, float h, float *x_points, float *y_points, int steps) {
    float x_n = x_0;
    float y_n = y_0;

    for (int i = 0; i < steps; i++) {
        x_points[i] = x_n;
        y_points[i] = y_n;

        float x_n1 = x_n + h;
        float y_n1 = y_n + h * dy_dx(x_n, y_n);

        x_n = x_n1;
        y_n = y_n1;
    }
}

// Main function (for testing purposes only)
int main() {
    float x_0 = 0.1;
    float y_0 = (-2 / x_0) * (1 + 1 / log(x_0)); // Initial y based on the equation
    float x_end = 0.997;
    float step_size = 0.001;
    int steps = (int)((x_end - x_0) / step_size) + 1;

    // Allocate memory for arrays to store points
    float *x_points = (float *)malloc(steps * sizeof(float));
    float *y_points = (float *)malloc(steps * sizeof(float));

    if (x_points == NULL || y_points == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Call the points function to calculate x and y points
    points(x_0, y_0, x_end, step_size, x_points, y_points, steps);

    // Print the calculated x and y values for testing
    for (int i = 0; i < steps; i++) {
        printf("x = %.4f, y = %.4f\n", x_points[i], y_points[i]);
    }

    // Free allocated memory
    free(x_points);
    free(y_points);

    return 0;
}

