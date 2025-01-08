#include <math.h>

// Function for finite differences to compute x for a given range of y values
void finite_differences(double y_start, double y_end, double h, double* y_values, double* x_values, int* n_points) {
    double y = y_start;
    double x = 0.0;
    int index = 0;

    while (y <= y_end) {
        y_values[index] = y;
        x_values[index] = x;

        double dx_dy = -y * y / (1 + y * y); // dx/dy = -y^2 / (1 + y^2)
        x += h * dx_dy; // Update x
        y += h;         // Increment y by step size
        index++;
    }
    *n_points = index;
}

// Function for exact solution x = tan^-1(y) - y
double exact_solution(double y) {
    return atan(y) - y;
}
