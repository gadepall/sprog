#include <math.h>
#include <stdio.h>

typedef double (*func_ptr)(double);  // Function pointer type for the given function

double gradient_ascent(func_ptr f_prime, double initial_guess, double learning_rate, double tolerance, int max_iter) {
    double x = initial_guess;  // Start at the initial guess
    double grad = f_prime(x);  // Compute the gradient
    int iter = 0;

    while (fabs(grad) > tolerance && iter < max_iter) {
        x += learning_rate * grad;  // Update x using gradient ascent
        grad = f_prime(x);          // Recompute the gradient
        iter++;
    }
    return x;  // Return the converged value of x
}

