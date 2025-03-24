#include <stdio.h>
#include <math.h>

#define EPSILON 1e-6
#define MAX_ITERATIONS 100

double f(double x) {
    return 1/x - 1/(x-2) - 3;
}

double f_prime(double x) {
    return -1/(x*x) + 1/((x-2)*(x-2));
}

double newton_method(double x0) {
    double x = x0;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double fx = f(x);
        if (fabs(fx) < EPSILON) {
            return x;
        }
        double fx_prime = f_prime(x);
        if (fx_prime == 0) {
            printf("Derivative is zero. Newton's method fails.\n");
            return x;
        }
        x = x - fx / fx_prime;
    }
    printf("Newton's method did not converge within %d iterations.\n", MAX_ITERATIONS);
    return x;
}

int main() {
    double initial_guess1 = 0.5;
    double initial_guess2 = 3.0;

    printf("Root 1: %f\n", newton_method(initial_guess1));
    printf("Root 2: %f\n", newton_method(initial_guess2));

    return 0;
}

