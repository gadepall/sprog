#include <stdio.h>
#include <math.h>

// Function definition
double f(double x) {
    return x * x * x - 3 * x + 3;
}

// Derivative of the function
double f_prime(double x) {
    return 3 * x * x - 3;
}

void gradient_descent(double start_point, double *result_x, double *result_y) {
    double x = start_point;
    int iter = 0;

    
    while (iter < 1000) {
        double grad = f_prime(x);
        double new_x = x - 0.1 * grad;
        
        
        if (fabs(new_x - x) < 1e-6) {
            break;
        }
        
        x = new_x;
        iter++;
    }

    *result_x = x;
    *result_y = f(x);
}

