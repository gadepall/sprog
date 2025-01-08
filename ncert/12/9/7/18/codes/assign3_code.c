// C code to generate the points for the iterative process
#include <stdio.h>
#include <math.h>

// Function to compute the points
double* generate_points(int iterations, double h) {
    static double points[2000]; // Array to store x and y pairs (1000 points)
    double x = 0.0, y = 1.0;   // Initial conditions

    for (int i = 0; i < iterations; i++) {
        points[2 * i] = x;
        points[2 * i + 1] = y;
        y += h * (-y - 2 * x / exp(x));
        x += h;
    }
    return points;
}
