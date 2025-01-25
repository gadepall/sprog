#include <stdio.h>
#include <math.h>

#define TOL 1e-6
#define MAX_ITER 100

// Define the function and its derivative for the equation p^2 + 11p - 1452 = 0
double f(double p) {
    return p * p + 11 * p - 1452;
}

double f_prime(double p) {
    return 2 * p + 11;
}

// Bisection Method
double bisection_method(double (*f)(double), double a, double b) {
    if (f(a) * f(b) > 0) {
        printf("No root in this interval, the function does not change sign.\n");
        return -1; // Error code
    }

    double c;
    for (int i = 0; i < MAX_ITER; i++) {
        c = (a + b) / 2;
        if (fabs(f(c)) < TOL) {
            return c;
        }
        if (f(a) * f(c) < 0) {
            b = c;
        } else {
            a = c;
        }
    }
    return c;
}

// Newton's Method
double newton_method(double (*f)(double), double (*f_prime)(double), double p0) {
    double p1;
    for (int i = 0; i < MAX_ITER; i++) {
        p1 = p0 - f(p0) / f_prime(p0);
        if (fabs(p1 - p0) < TOL) {
            return p1;
        }
        p0 = p1;
    }
    return p0;
}

// Combined Method to find both roots
void find_roots(double (*f)(double), double (*f_prime)(double), double *root1, double *root2) {
    // Step 1: Use Bisection to find the first root (in the interval [-50, 0])
    double p_bisection1 = bisection_method(f, -50, 0);
    if (p_bisection1 != -1) {
        *root1 = newton_method(f, f_prime, p_bisection1);
    }

    // Step 2: Use Bisection to find the second root (in the interval [0, 50])
    double p_bisection2 = bisection_method(f, 0, 50);
    if (p_bisection2 != -1) {
        *root2 = newton_method(f, f_prime, p_bisection2);
    }
}

// Export the function to find both roots
__attribute__((visibility("default"))) void find_roots_from_c(double *root1, double *root2) {
    find_roots(f, f_prime, root1, root2);
}

