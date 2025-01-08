#include <stdio.h>
#include <math.h>

// Function to compute y-coordinates on the circle x^2 + y^2 = radius^2
// Parameters:
// x: x-coordinate
// radius: Radius of the circle
// Returns: y-coordinate on the circle
double circle_y(double x, double radius) {
    return sqrt(radius * radius - x * x); // Calculate y using the circle equation
}

// Function to calculate the area in the first quadrant bounded by the circle
// Parameters:
// area: Pointer to store the computed area
// radius: Radius of the circle
// x1, x2: Bounds for the x-coordinate (x = 0 to x = 2)
// n: Number of subdivisions for numerical integration
void calculate_area(double *area, double radius, double x1, double x2, int n) {
    double h = (x2 - x1) / n; // Step size for numerical integration
    *area = 0.0; // Initialize area

    // Apply numerical integration (trapezoidal rule)
    for (int i = 0; i <= n; i++) {
        double x = x1 + i * h;                // Current x-coordinate
        double y_circle = circle_y(x, radius); // y-coordinate on the circle

        // Apply the trapezoidal rule
        if (i == 0 || i == n) {
            *area += y_circle / 2.0;
        } else {
            *area += y_circle;
        }
    }

    *area *= h; // Multiply by step size to get the total area
}

int main() {
    double area;
    double radius = 2.0;  // Radius of the circle
    double x1 = 0.0;      // Lower bound of x (x = 0)
    double x2 = 2.0;      // Upper bound of x (x = 2)
    int n = 1000;         // Number of subdivisions for numerical integration

    // Calculate the area
    calculate_area(&area, radius, x1, x2, n);

    // Print the result
    printf("Area in the first quadrant bounded by the circle: %.4f\n", area);

    return 0;
}

