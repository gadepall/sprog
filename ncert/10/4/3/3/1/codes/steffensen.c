#include<stdio.h>
#include<math.h>

double steff1(double x) {
    double f = pow(x, 2) - 3 * x - 1;   // Function value
    double g = (pow(x + f, 2) - 3 * (x + f) - 1 - f) / f; // Approximation of derivative
    return x - f / g; // Update step
}


double steff2(double x) {
    double f = pow(x, 2) - 3 * x - 1;   // Function value
    double g = (pow(x + f, 2) - 3 * (x + f) - 1 - f) / f; // Approximation of derivative
    return x - f / g;     
}


