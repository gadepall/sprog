#include <math.h>
#include <stdio.h>

// Function to compute f(x) = sin(x) + cos(x)
double func(double x) {
    return sin(x) + cos(x);
}

// Function to compute f'(x) = cos(x) - sin(x)
double func_deriv(double x) {
    return cos(x) - sin(x);
}

// Gradient ascent method to find the extremum
void gradient_method(double x0, double learning_rate, double tol, double extremum[2], double (*func)(double), double (*func_deriv)(double)) {
    double x1 = x0 + learning_rate * func_deriv(x0);
    while (fabs(x1 - x0) >= tol) {
        x0 = x1;
        x1 = x0 + learning_rate * func_deriv(x0);
    }
    extremum[0] = x1;
    extremum[1] = func(x1);
}

// Generate points for plotting
void generate_points(double start, double end, int num_points, double x_vals[], double y_vals[], double (*func)(double)) {
    double step = (end - start) / (num_points - 1);
    for (int i = 0; i < num_points; i++) {
        x_vals[i] = start + i * step;
        y_vals[i] = func(x_vals[i]);
    }
}

