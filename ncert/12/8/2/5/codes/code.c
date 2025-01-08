#include <stdio.h>
#include <math.h>

int main() {
    // Line equations: x = ay + b
    double l[2][2] = {{2, 1}, {3, 1}};
    double h = 0.0001; // Step size for integration
    double area = 0;

    // Function for calculating the area between two lines
    double f(double y) {
        double a = l[1][0] - l[0][0];
        double b = l[1][1] - l[0][1];
        return (a*y + b); // Horizontal difference between the lines
    }
    // Integral: Between the lowest line and the line before the highest point
    for (double y = 0; y <= 4; y += h) {
        if (y == 0 || y == 4) {
            area += h*f(y) / 2.0;
        } else {
            area += h*f(y);
        }
    }

    // Output the computed area
    printf("Area of the triangle: %lf\n", area);
    return 0;
}

