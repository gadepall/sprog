#include <stdlib.h>
#include <math.h>

// Function to generate points for |x + 3| over the interval [-6, 0]
void generate_points(double *points, int n) {
    double h = 6.0 / n;  // Step size for the interval [-6, 0]
    double x = -6.0;
    for (int i = 0; i < n; i++) {
        points[2 * i] = x;              // Store x values
        points[2 * i + 1] = fabs(x + 3); // Store |x + 3| values
        x += h;
    }
}

// Function to calculate the integral of |x + 3| over the interval [-6, 0]
void calc_integral(int n, double *res) {
    double h = 6.0 / n;  // Step size
    double integral = 0;
    double x = -6.0;

    for (int i = 0; i < n; i++) {
        double f1 = fabs(x + 3);         // |x + 3| at the left endpoint
        double f2 = fabs(x + 3 + h);     // |x + 3| at the right endpoint
        integral += (f1 + f2) * h / 2;  // Trapezoidal rule
        x += h;
    }

    *res = integral;
}

// Function to free allocated memory
void free_ptr(double *points) {
    free(points);
}

