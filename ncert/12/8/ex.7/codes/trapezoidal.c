#include <stdio.h>

// Define the trapezoidal rule function
double trapezoidal_rule(double (*func)(double), double a, double b, int n) {
    // Calculate the width of each subinterval
    double h = (b - a) / n;

    // Initialize sum with the values of the first and last terms (half weight)
    double sum = 0.5 * (func(a) + func(b));

    // Add the values of the function at each intermediate point
    for (int i = 1; i < n; i++) {
        sum += func(a + i * h);
    }

    // Multiply the sum by the step size to get the final approximation
    sum *= h;

    return sum;
}

// Example function: f(x) = (3x + 12)/2 - 3x^2/4
// This function can be replaced with any other function you want to integrate
double example_function(double x) {
    return (3 * x + 12) / 2 - (3 * x * x) / 4;
}

// Function to be used by Python (returning the result of integration)
double integrate(double a, double b, int n) {
    return trapezoidal_rule(example_function, a, b, n);
}

