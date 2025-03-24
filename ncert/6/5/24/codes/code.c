#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Generate points for the loss function plot
void generate_points(double *points, int n) {
    double x_0 = 0.1; // Avoid division by zero
    double x_max = 20;
    double h = (x_max - x_0) / (n - 1);
    for (int i = 0; i < n; i++) {
        double r = x_0 + i * h;
        points[i] = r; // x-coordinate (radius)
        //points[2 * i + 1] = M_PI * r * sqrt(r * r + (3 * 100) / (M_PI * r * r)); // CSA formula
    }
}

// Perform gradient descent
void run_gradient_descent(double init_guess, double step_size, double (*f)(double), double tolerance, double *minimum) {
    int count = 0;
    double current = init_guess;

    while (1) {
        current -= f(current) * step_size; // Gradient descent update
        count++;
        if (count > 10000) { // Add a safety exit to prevent infinite loops
            printf("Gradient descent did ran 10000 times.\n");
            *minimum = current;
            return;
        }
    }

    printf("Number of iterations: %d\n", count);
    *minimum = current;
}

// Free memory (if needed)
void free_ptr(double *points) {
    free(points);
}

