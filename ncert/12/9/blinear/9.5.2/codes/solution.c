#include <math.h>
#include <stdlib.h>

// Function to compute the theoretical solution y = x*ln(x) + Cx
double theoretical_solution(double x, double C) {
    return x * log(x) + C * x;
}

// Function to compute the numerical solution using Improved Euler's method
void numerical_solution(double x_start, double x_end, double h, double C, 
                        double* x_values, double* y_values, int* n_points) {
    int index = 0;
    double x = x_start;
    double y = theoretical_solution(x, C);  // Initial y based on theoretical solution

    while (x <= x_end) {
        // Store x and y values
        x_values[index] = x;
        y_values[index] = y;

        // Improved Euler method: y[n+1] = y[n] + h*(1 + y[n]/x[n])
        double k1 = 1 + y / x;
        double k2 = 1 + (y + h * k1) / (x + h);
        y += h * (0.5 * (k1 + k2));  // Update y using the average slope

        x += h;  // Increment x by the step size
        index++;
    }

    *n_points = index;  // Set the number of points computed
}
