#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// f(x) = sin(x) - cos(x)
double func(double x) {
    return sin(x) - cos(x);
}

// f'(x) = cos(x) + sin(x)
double func_deriv(double x) {
    return cos(x) + sin(x);
}

// Keep x positive by wrapping into the range [0, 2π)
double keep_positive(double x) {
    while (x < 0) {
        x += 2 * M_PI;
    }
    return x;
}

// Find extrema (maxima and minima)
void find_extremum(double x0, double learning_rate, double tol, double extremum[4]) {
    double x_max = x0;
    double x_min = x0;
    double x1_max, x1_min;

    // Find maximum
    x1_max = x_max + learning_rate * func_deriv(x_max);
    while (fabs(func_deriv(x_max)) >= tol) {
        x_max = keep_positive(x1_max);
        x1_max = x_max + learning_rate * func_deriv(x_max);
    }
    extremum[0] = x_max;     // x_max
    extremum[1] = func(x_max); // f(x_max)

    // Find minimum
    x1_min = x_min - learning_rate * func_deriv(x_min);
    while (fabs(func_deriv(x_min)) >= tol) {
        x_min = keep_positive(x1_min);
        x1_min = x_min - learning_rate * func_deriv(x_min);
    }
    extremum[2] = x_min;     // x_min
    extremum[3] = func(x_min); // f(x_min)
}


