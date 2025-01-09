#include <stdio.h>
#include <math.h>

// Function representing the integrand sqrt(y/4)
double integrand(double y) {
    return sqrt(y / 4);
}

// Function to compute the area using the trapezoidal rule
double trapezoidal_rule(double (*f)(double), double a, double b, int n) {
    double h = (b - a) / n;  // Step size
    double sum = 0.5 * (f(a) + f(b));  // Initial and final terms

    // Sum the intermediate terms
    for (int i = 1; i < n; i++) {
        sum += f(a + i * h);
    }

    return sum * h;
}

int main() {
    double a = 1.0, b = 4.0;  // Limits of integration
    int n = 1000;  // Number of intervals for trapezoidal rule

    // Calculate the area
    double area = trapezoidal_rule(integrand, a, b, n);


    return 0;
}

