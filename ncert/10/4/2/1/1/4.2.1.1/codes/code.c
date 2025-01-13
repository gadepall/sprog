#include <stdio.h>
#include <math.h>

// Function prototypes
void quadraticFormula(double a, double b, double c);
void fixedPointIteration(double initial_guess, int max_iter, double tolerance);
void newtonRaphson(double initial_guess, int max_iter, double tolerance);
double g(double x);  // g(x) for Fixed Point Iteration
double f(double x);  // f(x) for Newton-Raphson
double f_prime(double x); // f'(x) for Newton-Raphson

// Function to solve using the quadratic formula
void quadraticFormula(double a, double b, double c) {
    double discriminant = b * b - 4 * a * c;
    if (discriminant >= 0) {
        double root1 = (-b + sqrt(discriminant)) / (2 * a);
        double root2 = (-b - sqrt(discriminant)) / (2 * a);
        printf("Root 1 = %.6f\n", root1);
        printf("Root 2 = %.6f\n", root2);
    } else {
        double real_part = -b / (2 * a);
        double imaginary_part = sqrt(-discriminant) / (2 * a);
        printf("Root 1 = %.6f + %.6fi\n", real_part, imaginary_part);
        printf("Root 2 = %.6f - %.6fi\n", real_part, imaginary_part);
    }
}

// Function to solve using Fixed Point Iteration
void fixedPointIteration(double initial_guess, int max_iter, double tolerance) {
    double x = initial_guess;
    for (int i = 0; i < max_iter; i++) {
        double next_x = g(x);  // g(x) = sqrt(3x + 10)
        printf("Iteration %d: x = %.6f\n", i + 1, next_x);
        if (fabs(next_x - x) < tolerance) {
            printf("Converged to root: %.6f\n", next_x);
            return;
        }
        x = next_x;
    }
    printf("Did not converge within %d iterations.\n", max_iter);
}

// Function to solve using Newton-Raphson Method
void newtonRaphson(double initial_guess, int max_iter, double tolerance) {
    double x = initial_guess;
    for (int i = 0; i < max_iter; i++) {
        double fx = f(x);
        double fpx = f_prime(x);
        if (fabs(fpx) < 1e-8) {
            printf("Derivative too small. Stopping iterations.\n");
            return;
        }
        double next_x = x - fx / fpx;
        printf("Iteration %d: x = %.6f\n", i + 1, next_x);
        if (fabs(next_x - x) < tolerance) {
            printf("Converged to root: %.6f\n", next_x);
            return;
        }
        x = next_x;
    }
    printf("Did not converge within %d iterations.\n", max_iter);
}

// g(x) function for Fixed Point Iteration
double g(double x) {
    return sqrt(3 * x + 10);
}

// f(x) for Newton-Raphson
double f(double x) {
    return x * x - 3 * x - 10;
}

// f'(x) for Newton-Raphson
double f_prime(double x) {
    return 2 * x - 3;
}

int main() {
    // Quadratic formula example
    quadraticFormula(1, -3, -10);

    // Fixed Point Iteration example
    fixedPointIteration(2.0, 20, 1e-6);

    // Newton-Raphson example
    newtonRaphson(2.0, 20, 1e-6);

    return 0;
}

