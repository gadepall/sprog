#include <math.h>

double x_n(double x_prev, double h) {
    return x_prev - h;  // Compute the next x value
}

double y_n(double x_prev, double y_prev, double h) {
    double dy_dx;
    if (sin(y_prev / x_prev) != 0) {
        dy_dx = (y_prev / x_prev) - 1.0 / sin(y_prev / x_prev);
    } else {
        dy_dx = 0.0;  // Handle division by zero gracefully
    }
    return y_prev + h * dy_dx;
}

