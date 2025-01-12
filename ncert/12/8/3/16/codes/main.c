#include <stdio.h>
#include <math.h>

// Function to compute y-coordinates for the curve y = x^3
double cubic_y(double x) {
    return pow(x, 3);
}

// Function to calculate the area under the curve y = x^3
// between x = x_start and x = x_end using numerical integration
void calculate_area(double x_start, double x_end, int n, double *area) {
    double h = (x_end - x_start) / n; // Step size for numerical integration
    *area = 0.0; // Initialize area

    // Use the trapezoidal rule to calculate the area
    for (int i = 0; i <= n; i++) {
        double x = x_start + i * h;       // Current x-coordinate
        double y = cubic_y(x);           // y-coordinate on the curve y = x^3

        if (i == 0 || i == n) {          // Apply trapezoidal rule
            *area += y / 2.0;
        } else {
            *area += y;
        }
    }

    *area *= h; // Multiply by step size to get the total area

    // Ensure area is non-negative
    if (*area < 0) {
        *area = -(*area);
    }
}

int main() {
    double x_start = -2.0; // Starting x-coordinate
    double x_end = 1.0;    // Ending x-coordinate
    int n = 1000;          // Number of subdivisions for numerical integration
    double area;

    // Calculate the area under the curve y = x^3
    calculate_area(x_start, x_end, n, &area);

    // Output the result
    printf("Area under the curve y = x^3 from x = %.2f to x = %.2f: %.4f square units\n",
           x_start, x_end, area);

    return 0;
}

