#include <stdio.h>
#include <math.h>

// Function to solve the quadratic equation ax^2 + bx + c = 0
// Parameters:
// a, b, c: Coefficients of the quadratic equation
// x1, x2: Pointers to store the roots of the equation
// Returns:
// 0 if no real roots, 1 if one real root, 2 if two real roots
int solveQuadratic(double a, double b, double c, double *x1, double *x2) {
    double discriminant = b * b - 4 * a * c; // Calculate the discriminant
    if (discriminant < 0) {
        return 0; // No real roots
    } else if (discriminant == 0) {
        *x1 = *x2 = -b / (2 * a); // Single root (repeated)
        return 1; // One real root
    } else {
        *x1 = (-b + sqrt(discriminant)) / (2 * a); // First root
        *x2 = (-b - sqrt(discriminant)) / (2 * a); // Second root
        return 2; // Two real roots
    }
}

// Function to compute y-coordinates on the circle x^2 + y^2 = radius^2
// Parameters:
// x: x-coordinate
// c: Radius of the circle
// Returns: y-coordinate on the circle
double circle_y(double x, double c) {
    return sqrt(c * c - x * x); // Calculate y using the circle equation
}

// Function to compute y-coordinates on the line y = mx + c
// Parameters:
// x: x-coordinate
// m: Slope of the line
// c: y-intercept of the line
// Returns: y-coordinate on the line
double line_y(double x, double m, double c) {
    return m * x + c; // Calculate y using the line equation
}

// Function to calculate intersection points and area between the line and the circle
// Parameters:
// x1, y1, x2, y2: Pointers to store intersection points
// area: Pointer to store the computed area
// m, c: Slope and y-intercept of the line
// radius: Radius of the circle
// n: Number of subdivisions for numerical integration
// Returns:
// Number of intersection points (0, 1, or 2)
int calculate_intersection_and_area(
    double *x1, double *y1, double *x2, double *y2, double *area, double m, double c, double radius, int n) {

    // Coefficients for the quadratic equation to find intersection points
    double A = 1.0 + m * m;         // Coefficient of x^2
    double B = 2 * m * c;           // Coefficient of x
    double C = c * c - radius * radius; // Constant term

    // Solve the quadratic equation for x-coordinates
    int num_solutions = solveQuadratic(A, B, C, x1, x2);

    if (num_solutions > 0) { // If there are intersection points
        *y1 = line_y(*x1, m, c); // Calculate y-coordinate for first intersection
        if (num_solutions == 2) {
            *y2 = line_y(*x2, m, c); // Calculate y-coordinate for second intersection
        }

        // Compute area between the circle and the line
        if (num_solutions == 2) { // Only compute area if there are two intersection points
            double h = (*x2 - *x1) / n; // Step size for numerical integration
            *area = 0.0; // Initialize area

            for (int i = 0; i <= n; i++) {
                double x = *x1 + i * h;           // Current x-coordinate
                double y_circle = circle_y(x, radius); // y-coordinate on the circle
                double y_line = line_y(x, m, c);       // y-coordinate on the line

                // Include area where the circle is above the line
                if (y_circle > y_line) {
                    double height = y_circle - y_line; // Height of the strip
                    if (i == 0 || i == n) { // Apply trapezoidal rule
                        *area += height / 2.0;
                    } else {
                        *area += height;
                    }
                }
            }

            *area *= h; // Multiply by step size to get the total area

            // Ensure area is non-negative
            if (*area < 0) {
                *area = -(*area);
            }
        } else {
            *area = 0.0; // No area if there is only one intersection point
        }
    }

    return num_solutions; // Return the number of intersection points
}


