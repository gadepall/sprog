#include <stdio.h>
#include <math.h>

// Function to compute x-coordinates for a given y on the parabola y = x^2 / 4
// Parameters:
// y: y-coordinate
// Returns: x-coordinate corresponding to the given y
double parabola_x(double y) {
    return sqrt(4.0 * y); // Calculate x using the inverse parabola equation
}

// Function to calculate the area under the parabola
// Parameters:
// y1, y2: Limits of integration (start and end points on the y-axis)
// n: Number of subdivisions for numerical integration
// Returns: Calculated area
double calculate_area(double y1, double y2, int n) {
    double x1 = parabola_x(y1); // Find x for y = y1
    double x2 = parabola_x(y2); // Find x for y = y2
    double h = (y2 - y1) / n; // Step size for numerical integration
    double area = 0.0; // Initialize area

    // Perform trapezoidal integration
    for (int i = 0; i <= n; i++) {
        double y = y1 + i * h;  // Current y-coordinate
        double x = parabola_x(y); // Corresponding x-coordinate on the parabola
        double current_area = x;  // Width of the strip at the given y

        if (i == 0 || i == n) { // Apply trapezoidal rule
            area += current_area / 2.0; // Half-weight for the first and last terms
        } else {
            area += current_area; // Full weight for intermediate terms
        }
    }

    area *= h; // Multiply by step size to get the total area

    return area; // Return the calculated area
}

int main() {
    double y1 = 2.0; // Start y-coordinate (y = 2)
    double y2 = 4.0; // End y-coordinate (y = 4)
    int n = 1000;     // Number of subdivisions

    // Calculate the area
    double area = calculate_area(y1, y2, n);

    // Output the result
    printf("The area under the parabola from y = 2 to y = 4 is approximately %.5f square units.\n", area);

    return 0;
}

