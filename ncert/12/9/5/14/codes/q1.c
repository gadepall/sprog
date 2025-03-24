#include <stdio.h>
#include <math.h>

#define POINTS 100  // Number of points to generate

// Function to compute the value of y using the given formula
double compute_y(double x) {
    return x * acos(1 + log(x));
}

// Function to generate points for the given function
// Parameters:
//   x_start: Starting value of x
//   x_end: Ending value of x
//   x: Array to store x values
//   y: Array to store y values
void generate_points(double x_start, double x_end, double x[], double y[]) {
    // Calculate the step size for x values to divide the range into POINTS-1 intervals
    double step = (x_end - x_start) / (POINTS - 1);

    // Generate points using the formula y = x * acos(1 + log(x))
    for (int i = 0; i < POINTS; i++) {
        // Compute the x value
        x[i] = x_start + i * step;

        // Compute the corresponding y value
        y[i] = compute_y(x[i]);
    }
}

// Example usage of the generate_points function
int main() {
    // Define the range of x and initial value of y
    double x_start = 0.2;
    double x_end = 1.0;

    // Arrays to store the generated x and y values
    double x[POINTS];
    double y[POINTS];

    // Generate the points for the given function
    generate_points(x_start, x_end, x, y);

    // Print the results in a readable format
    printf("x\ty\n");
    for (int i = 0; i < POINTS; i++) {
        printf("%.5f\t%.5f\n", x[i], y[i]);
    }

    return 0;
}

