#include <stdio.h>
#include <math.h>

void quadraticFormula(double a, double b, double c);
void fixedPointIteration(double initial_guess, int max_iter, double tolerance);
double g(double x);  // g(x) for Fixed Point Iteration

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
        double next_x = g(x);  
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
    return sqrt(4*sqrt(3) * x - 4);
}

int main() {
    // Quadratic formula example
    quadraticFormula(3, -4 * sqrt(3), 4);

    // Fixed Point Iteration example
    fixedPointIteration(2.0, 20, 1e-6);

    return 0;
}

