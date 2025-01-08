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

// Function to compute points on the ellipse y^2/b^2 + x^2/a^2 = 1
double ellipse_y(double x, double a, double b) {
    return sqrt((b * b) * (1 - (x * x) / (a * a)));
}

// Function to compute points on the line y = mx + c
double line_y(double x, double m, double c) {
    return m * x + c;
}

// Function to calculate intersection points and area
int calculate_intersection_and_area(
    double *x1, double *y1, double *x2, double *y2, double *area,
    double a, double b, double m, double c, int n) {

    // Quadratic equation coefficients
    double A = 1.0 / (a * a) + (m * m) / (b * b);
    double B = (2 * m * c) / (b * b);
    double C = (c * c) / (b * b) - 1;

    // Solve quadratic equation for x-coordinates
    int num_solutions = solveQuadratic(A, B, C, x1, x2);

    if (num_solutions > 0) {
        // Calculate y-values for the intersection points
        *y1 = line_y(*x1, m, c);
        if (num_solutions == 2) {
            *y2 = line_y(*x2, m, c);
        }

        // Calculate area if there are two intersection points
        if (num_solutions == 2) {
            double h = (*x2 - *x1) / n;
            *area = 0.0;

            for (int i = 0; i <= n; i++) {
                double x = *x1 + i * h;
                double y_ellipse = ellipse_y(x, a, b);
                double y_line = line_y(x, m, c);

                // Consider only region where ellipse is above the line
                if (y_ellipse > y_line) {
                    double height = y_ellipse - y_line;
                    if (i == 0 || i == n) {
                        *area += height / 2.0;
                    } else {
                        *area += height;
                    }
                }
            }

            *area *= h;
        } else {
            *area = 0.0; // No area if there's only one intersection point
        }
    }

    return num_solutions;
}

