#include <stdlib.h>
#include <math.h>

// Function to generate points for x^2 + 1 over the interval [0, 1]
void generate_pointsf_1(double *points, int n) {
    double h = 1.0 / n;  // Step size for the interval [0, 1]
    double x = 0;
    for (int i = 0; i < n; i++) {
        points[2 * i] = x;              // Store x values
        points[2 * i + 1] = fabs(pow(x, 2) + 1); // Store |x^2 + 1| value
        x += h;
    }
}

// Function to calculate the integral of |x^2 + 1| over the interval [0, 1]
void calc_integralf_1(int n, double *res) {
    double h = 1.0 / n;  // Step size
    double integral = 0;
    double x = 0;

    for (int i = 0; i < n; i++) {
        double f1 = fabs(pow(x, 2) + 1);        // f(x) at the left endpoint
        double f2 = fabs(pow(x + h, 2) + 1);   // f(x) at the right endpoint
        integral += (f1 + f2) * h / 2;  // Trapezoidal rule
        x += h;
    }

    *res = integral;
}

// Function to generate points for x + 1 over the interval [1, 2]
void generate_pointsf_2(double *points, int n) {
    double h = 1.0 / n;  // Step size for the interval [1, 2]
    double x = 1;
    for (int i = 0; i < n; i++) {
        points[2 * i] = x;              // Store x values
        points[2 * i + 1] = fabs(x + 1); // Store |x + 1| values
        x += h;
    }
}

// Function to calculate the integral of |x + 1| over the interval [1, 2]
void calc_integral(int n, double *res) {
    double h = 1.0 / n;  // Step size
    double integral = 0;
    double x = 1;

    for (int i = 0; i < n; i++) {
        double f1 = fabs(x + 1);        // |x + 1| at the left endpoint
        double f2 = fabs(x + 1 + h);    // |x + 1| at the right endpoint
        integral += (f1 + f2) * h / 2;  // Trapezoidal rule
        x += h;
    }

    *res = integral;
}

// Function to free allocated memory
void free_ptr(double *points) {
    free(points);
}

