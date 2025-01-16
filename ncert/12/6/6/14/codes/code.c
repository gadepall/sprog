#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define h 0.001       // Step size for numerical derivative
#define tol 1e-10     // Tolerance for convergence

// Function: f(x) = cos^2(x) + sin(x)
double function(double x) 
{
    return pow(cos(x), 2) + sin(x);
}

// Numerical derivative: f'(x)
double derivative(double x) 
{
    return (function(x + h) - function(x)) / h;
}

// Generates points for plotting f(x)
void pointGen(double start, double end, double* x, double* y) 
{
    int i = 0;
    for (double val = start; val <= end; val += h, i++) {
        x[i] = val;           // Store x-values
        y[i] = function(x[i]); // Store corresponding y-values
    }
}

// Gradient Ascent/Descent to find critical points
void gradient_descent(double x0, double alpha, double* extremum, int is_ascent) 
{
    double x1 = x0 + (is_ascent ? alpha : -alpha) * derivative(x0);
    while (fabs(x1 - x0) >= tol) {
        x0 = x1;
        x1 = x0 + (is_ascent ? alpha : -alpha) * derivative(x0);
    }
    extremum[0] = x1;               // Store the critical point
    extremum[1] = function(extremum[0]); // Store the function value at critical point
}

