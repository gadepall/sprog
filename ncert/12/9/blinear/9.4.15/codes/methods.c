#include <math.h>

// Function to compute f(x) = e^x * sin(x)
double f(double x) {
    return exp(x) * sin(x);
}

// Trapezoidal Method
void trapezoidal_method(double x_start, double x_end, double h, double y0, int n, double *x, double *y) {
    x[0] = x_start;
    y[0] = y0;
    for (int i = 1; i < n; i++) {
        x[i] = x[i - 1] + h;
        y[i] = y[i - 1] + (h / 2) * (f(x[i]) + f(x[i - 1]));
    }
}

// Laplace Analytical Solution: y = 1 - e^x * cos(x) + e^x * sin(x)
void laplace_method(double x_start, double x_end, double h, int n, double *x, double *y) {
    for (int i = 0; i < n; i++) {
        x[i] = x_start + i * h;
        y[i] = (1 - exp(x[i]) * cos(x[i]) + exp(x[i]) * sin(x[i]))/(2.0);
    }
}

