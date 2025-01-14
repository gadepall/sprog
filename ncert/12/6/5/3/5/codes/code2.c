#include <math.h>
#include <stdlib.h>

// f(x) = x^3 - 6x^2 + 9x + 15
double func(double x) {
    return x * x * x - 6 * x * x + 9 * x + 15;
}

// f'(x) = 3x^2 - 12x + 9
double func_deriv(double x) {
    return 3 * x * x - 12 * x + 9;
}

// Generate points for given function
void points_gen(double x_start, double x_end, double* x_vals, double* y_vals, double h, double (*func)(double)) {
    int i = 0;
    for (double x_i = x_start; x_i <= x_end; x_i += h) {  // Fixed loop condition to include x_end
        x_vals[i] = x_i;
        y_vals[i] = func(x_i);
        i++;
    }
}

// Gradient Descent/Ascent method to find local minima/maxima
void gradient_method(double x0, double learning_rate, double tol, double extremum[2], double (*func)(double), double (*func_deriv)(double)) {
    double x1 = x0 - learning_rate * func_deriv(x0);  // Initialize with first step
    
    // Iterative gradient method
    while (fabs(x1 - x0) >= tol) {
        x0 = x1;
        x1 = x0 - learning_rate * func_deriv(x0);  // Update step
        
        // Prevent very large values for x1 to avoid overflow or runaway behavior
        if (fabs(x1) > 1e6) {  
            break;  // Exit if the values become unreasonably large
        }
    }

    extremum[0] = x1;
    extremum[1] = func(x1);
}

