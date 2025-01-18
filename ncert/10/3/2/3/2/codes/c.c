#include <stdlib.h>
#include <stdio.h>

// Function to generate points for a line given a slope-intercept equation
void gen_line_points(double (*line_func)(double), int num_points, double *points) {
    double left_lim = -10;  // Left limit for x-axis.
    double right_lim = 10;  // Right limit for x-axis.
    double h = (right_lim - left_lim) / num_points;  // Step size for x-values.

    // Generate points for the line
    for (int i = 0; i < num_points; i++) {
        points[2 * i] = left_lim;  // Store the X-coordinate
        points[2 * i + 1] = line_func(left_lim);  // Calculate Y-coordinate using the passed function
        left_lim += h;  // Increment the x-coordinate for the next point
    }
}

// Equation for the first line: y = (2x - 8) / 3
double line1_y(double x) {
    return (2.0 * x - 8.0) / 3.0;
}

// Equation for the second line: y = (4x - 9) / 6
double line2_y(double x) {
    return (4.0 * x - 9.0) / 6.0;
}

// Function to free dynamically allocated memory
void free_ptr(double *points) {
    free(points);
}

int main() {
    int num_points = 100;  // Number of points to generate for each line.

    // Allocate memory for storing points for both lines (2 * num_points for each line: x and y coordinates)
    double *points_line1 = (double *)malloc(2 * num_points * sizeof(double));  // Points for the first line
    double *points_line2 = (double *)malloc(2 * num_points * sizeof(double));  // Points for the second line

    if (points_line1 == NULL || points_line2 == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }

    // Generate points for both lines using their respective equations
    gen_line_points(line1_y, num_points, points_line1);
    gen_line_points(line2_y, num_points, points_line2);

    // Optionally: Print the first few points of the lines to verify (for debugging purposes)
    printf("Line 1 Points (first few):\n");
    for (int i = 0; i < 5; i++) {
        printf("x: %.2f, y: %.2f\n", points_line1[2 * i], points_line1[2 * i + 1]);
    }

    printf("\nLine 2 Points (first few):\n");
    for (int i = 0; i < 5; i++) {
        printf("x: %.2f, y: %.2f\n", points_line2[2 * i], points_line2[2 * i + 1]);
    }

    // Free the allocated memory
    free_ptr(points_line1);
    free_ptr(points_line2);

    return 0;
}

