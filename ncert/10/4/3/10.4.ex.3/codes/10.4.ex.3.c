#include <stdio.h>
#include <math.h>

// Function for f(x) = 2x^2 - 5x + 3
double f(double x) {
    return (2 * x * x) - (5 * x) + 3;
}

// Derivative of f(x), which is f'(x) = 4x - 5
double f_prime(double x) {
    return (4 * x) - 5;
}

// Newton-Raphson iteration function
// Arguments: initial guess (x0), max iterations, tolerance
double newton_raphson(double x0, int max_iter, double tolerance) {
    double x = x0;
    int iter = 0;
    
    while (iter < max_iter) {
        double fx = f(x);
        double fx_prime = f_prime(x);
        
        // Update x using the Newton-Raphson formula
        double x_new = x - fx / fx_prime;
        
        // Check if the change is within the tolerance
        if (fabs(x_new - x) < tolerance) {
            return x_new;
        }
        
        x = x_new;
        iter++;
    }
    
    return x;  // Return the final approximation after max_iter iterations
}

