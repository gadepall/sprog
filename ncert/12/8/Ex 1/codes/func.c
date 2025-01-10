#include <stdio.h>
#include <math.h>

double integrated(double x1, double x2) {
    int N = 30000; // Number of iterations
    double h = (x2 - x1) / N; // Step size
    double A = 0.0; // Accumulated area
    double x = x1; // Starting point of integration

    for (int i = 0; i < N; i++) {
        double y1 = sqrt(1 - x * x);         // Height at x
        double y2 = sqrt(1 - (x + h) * (x + h)); // Height at x + h
        A += 4*((y1 + y2) / 2) * h; // Trapezoid area
        x += h; // Increment x
    }
    return A;
}

