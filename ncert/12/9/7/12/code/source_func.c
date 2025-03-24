#include <math.h>
#include <stdlib.h>

double f(double x, double y) {
    return (exp(-2 * sqrt(x)) - y) / sqrt(x);
}

void finite_difference_method(double x0, double y0, double h, int n, double *x_values, double *y_values) {
    x_values[0] = x0;
    y_values[0] = y0;
    
    for (int i = 0; i < n; i++) {
        x_values[i + 1] = x_values[i] + h;
        // Forward Euler method: y(n+1) = y(n) + h*f(x(n), y(n))
        y_values[i + 1] = y_values[i] + h * f(x_values[i], y_values[i]);
    }
}
