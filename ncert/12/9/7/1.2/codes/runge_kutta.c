#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function definitions
double f1(double x, double y, double v) {
    return v; // dy/dx = v
}

double f2(double x, double y, double v) {
    return (cos(x) -7*v)/(3*v*v-8*v); // dv/dx
}

// Runge-Kutta function
void runge_kutta(double x0, double y0, double v0, double x_end, double h, double **x_array, double **y_array, int *size) {
    // Calculate the number of steps
    int n = (int)((x_end - x0) / h) + 1;
    *size = n;

    // Allocate memory for x and y arrays
    *x_array = (double *)malloc(n * sizeof(double));
    *y_array = (double *)malloc(n * sizeof(double));

    if (*x_array == NULL || *y_array == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    double x = x0, y = y0, v = v0;

    // Initialize the first elements
    (*x_array)[0] = x;
    (*y_array)[0] = y;

    for (int i = 1; i < n; i++) {
        // Compute k1 values
        double k1_y = h * f1(x, y, v);
        double k1_v = h * f2(x, y, v);

        // Compute k2 values
        double k2_y = h * f1(x + h / 2, y + k1_y / 2, v + k1_v / 2);
        double k2_v = h * f2(x + h / 2, y + k1_y / 2, v + k1_v / 2);

        // Compute k3 values
        double k3_y = h * f1(x + h / 2, y + k2_y / 2, v + k2_v / 2);
        double k3_v = h * f2(x + h / 2, y + k2_y / 2, v + k2_v / 2);

        // Compute k4 values
        double k4_y = h * f1(x + h, y + k3_y, v + k3_v);
        double k4_v = h * f2(x + h, y + k3_y, v + k3_v);

        // Update y and v
        y += (k1_y + 2 * k2_y + 2 * k3_y + k4_y) / 6;
        v += (k1_v + 2 * k2_v + 2 * k3_v + k4_v) / 6;

        // Update x
        x += h;

        // Store the results in the arrays
        (*x_array)[i] = x;
        (*y_array)[i] = y;
    }
}

