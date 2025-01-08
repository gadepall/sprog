#include <math.h>
#include <stdlib.h>

void euler_method(double y0, double x0, double xn, double h, double* x_values, double* y_values, double* y_approx, int num_points) {
    // Derivative function: dy/dx = 2x + 2
    double deriv(double x) {
        return 2 * x + 2;
    }

    // Theoretical solution: y = x^2 + 2x + C
    double theoretical_solution(double x, double C) {
        return x * x + 2 * x + C;
    }

    // Euler computation: y[n+1] = y[n] + h * f'(x)
    double eulerfunc(double y_prev, double x_prev, double h) {
        return y_prev + h * deriv(x_prev);
    }

    // Initialize values
    x_values[0] = x0;
    y_approx[0] = y0;
    y_values[0] = theoretical_solution(x0, y0);

    int i = 1;
    double x_current = x0 + h;

    // Compute using Euler's method
    while (x_current <= xn) {
        x_values[i] = x_current;
        y_approx[i] = eulerfunc(y_approx[i - 1], x_values[i - 1], h);
        y_values[i] = theoretical_solution(x_current, y0);

        x_current += h;
        i++;
    }
}

int main() {
    double x0 = 0;        // Initial x value
    double y0 = 1;        // Initial y value (C = 1)
    double xn = 2;        // Final x value
    double h = 0.1;       // Step size

    int num_points = (int)((xn - x0) / h) + 1; // Number of points
    double x_values[num_points];
    double y_values[num_points];  // Exact solution values
    double y_approx[num_points];  // Euler approximation values

    // Call Euler method
    euler_method(y0, x0, xn, h, x_values, y_values, y_approx, num_points);

    // Use a while loop to verify results
    int i = 0;
    while (i < num_points) {
        if (fabs(y_approx[i] - y_values[i]) > 1e-6) {
            // If approximation differs significantly from exact, indicate failure
            return -1;  // Exit with error code
        }
        i++;
    }
    return 0;
}
