#include <stdio.h>
#include <math.h>

// Function to solve the quadratic equation ax^2 + bx + c = 0
int solveQuadratic(double a, double b, double c, double *x1, double *x2) {
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return 0; // No real roots
    } else if (discriminant == 0) {
        *x1 = *x2 = -b / (2 * a);
        return 1; // One real root
    } else {
        *x1 = (-b + sqrt(discriminant)) / (2 * a);
        *x2 = (-b - sqrt(discriminant)) / (2 * a);
        return 2; // Two real roots
    }
}

// Function to compute y-coordinates on the parabola y^2 = 4ax
double parabola_y(double x, double a) {
    return sqrt(4 * a * x);
}

// Function to compute y-coordinates on the line y = x
double line_y(double x) {
    return x;
}

// Function to calculate intersection points and area between the line and the parabola
int calculate_intersection_and_area(
    double *x1, double *y1, double *x2, double *y2, double *area, double a, int n) {

    // Coefficients for the quadratic equation to find intersection points
    double A = 1.0;               // Coefficient of x^2
    double B = -4.0 * a;          // Coefficient of x
    double C = 0.0;               // Constant term

    // Solve the quadratic equation for x-coordinates
    int num_solutions = solveQuadratic(A, B, C, x1, x2);

    if (num_solutions > 0) { // If there are intersection points
        *y1 = line_y(*x1); // Calculate y-coordinate for first intersection
        if (num_solutions == 2) {
            *y2 = line_y(*x2); // Calculate y-coordinate for second intersection
        }

        // Compute area between the parabola and the line
        if (num_solutions == 2) { // Only compute area if there are two intersection points
            double h = (*x2 - *x1) / n; // Step size for numerical integration
            *area = 0.0; // Initialize area

            for (int i = 0; i <= n; i++) {
                double x = *x1 + i * h;               // Current x-coordinate
                double y_parabola = parabola_y(x, a); // y-coordinate on the parabola
                double y_line = line_y(x);            // y-coordinate on the line

                // Include area where the parabola is above the line
                if (y_parabola > y_line) {
                    double height = y_parabola - y_line; // Height of the strip
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

int main() {
    double x1, y1, x2, y2, area;
    double a = 1.0;  // Coefficient of the parabola y^2 = 4ax
    int n = 1000;    // Number of subdivisions for numerical integration

    int num_points = calculate_intersection_and_area(&x1, &y1, &x2, &y2, &area, a, n);

    if (num_points == 0) {
        printf("No real intersection points.\n");
    } else {
        printf("Intersection Points:\n");
        printf("Point 1: (%.4f, %.4f)\n", x1, y1);
        if (num_points == 2) {
            printf("Point 2: (%.4f, %.4f)\n", x2, y2);
        }
        printf("Area between the parabola and the line: %.4f square units\n", area);
    }

    return 0;
}

