#include <stdio.h>
#include <math.h>

// Function to compute y'(x) based on the given piecewise function
double y_prime(double x) {
    if (x >= 1 && x <= 2) {
        return 7.0 / 3.0;
    } else if (x > 2 && x <= 4) {
        return -3.0 / 2.0;
    } else {
        printf("Error: x is out of range!\n");
        return 0.0;
    }
}

// Function to compute f(x) based on the given piecewise function
double f(double x) {
    if (x >= 1 && x <= 2) {
        return (7.0 / 3.0) * x - (7.0 / 3.0);
    } else if (x > 2 && x <= 4) {
        return (-3.0 / 2.0) * x + (14.0 / 3.0);
    } else {
        printf("Error: x is out of range!\n");
        return 0.0;
    }
}

// Function to compute the area A_n iteratively between x_start and x_end
double compute_area(double x_start, double x_end, double h) {
    double A_n = 0.0;  // Initialize area
    double x_n = x_start;

    while (x_n < x_end) {
        double y_n = f(x_n);
        double y_prime_n = y_prime(x_n);

        // Update A_n using the given difference equation
        A_n += h * (2 * y_n + h * y_prime_n) / 2;

        // Increment x_n by step size
        x_n += h;
    }

    return A_n;
}

// Function to expose compute_area for Python bindings
double compute_area_wrapper(double x_start, double x_end, double h) {
    return compute_area(x_start, x_end, h);
}

