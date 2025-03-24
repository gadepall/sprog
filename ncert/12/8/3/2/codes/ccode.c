#include <stdio.h>
#include <math.h>

// Function for the line equation y = mx + c
double line_y(double x, double m, double c) {
    return m * x + c;
}

// Quadratic solver (returns the number of solutions)
int solveQuadratic(double A, double B, double C, double *x1, double *x2) {
    double discriminant = B * B - 4 * A * C;
    
    if (discriminant > 0) {
        // Two real solutions
        *x1 = (-B + sqrt(discriminant)) / (2 * A);
        *x2 = (-B - sqrt(discriminant)) / (2 * A);
        return 2;
    } else if (discriminant == 0) {
        // One real solution
        *x1 = -B / (2 * A);
        return 1;
    } else {
        // No real solutions
        return 0;
    }
}

// Function to calculate the area between two curves using the trapezoidal rule
double area(double lower_limit, double upper_limit, double stepsize) {
    double total_area = 0;
    for (double i = lower_limit; i < upper_limit; i += stepsize) {
        double f1 = i - i * i;  // y1 - y2 = x - x^2
        double f2 = (i + stepsize) - (i + stepsize) * (i + stepsize);
        total_area += 0.5 * stepsize * (f1 + f2);
    }
    return total_area;
}

// Function to calculate intersection points and area
int calculate_intersection_and_area(
    double *x1, double *y1, double *x2, double *y2, double *area_result,
    double a, double b, double m, double c, int n) {

    // Quadratic equation coefficients for x^2 - x = 0, simplified as:
    double A = 1.0;
    double B = -1.0;
    double C = 0.0;

    // Solve quadratic equation for x-coordinates
    int num_solutions = solveQuadratic(A, B, C, x1, x2);

    if (num_solutions > 0) {
        // Calculate y-values for the intersection points
        *y1 = line_y(*x1, m, c);  // For y = x
        if (num_solutions == 2) {
            *y2 = line_y(*x2, m, c);
        }
        
        // Calculate area between the curves from x1 to x2
        *area_result = area(*x1, *x2, 0.01);  // Assuming a step size of 0.01 for the area calculation
    }

    return num_solutions;
}

int main() {
    double x1, y1, x2, y2, result_area;
    
    // Set the slope and intercept for the line y = mx + c (y = x in this case)
    double m = 1.0, c = 0.0;

    // Calculate intersection points and area between the curves
    int num_intersections = calculate_intersection_and_area(&x1, &y1, &x2, &y2, &result_area, 0, 0, m, c, 2);

    if (num_intersections > 0) {
        printf("Intersection Points: (%.2f, %.2f) and (%.2f, %.2f)\n", x1, y1, x2, y2);
        printf("Area between the curves: %.5f\n", result_area);
    } else {
        printf("No intersection points found.\n");
    }

    return 0;
}

