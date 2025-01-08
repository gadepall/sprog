#include <stdio.h>
#include <math.h>
#define N 99

// Function to calculate y'' (second derivative of y with respect to x)
double y_prime_prime(double x, double y, double y_prime) {
    return (3 * y_prime - y) / (2 * x * x);
}

// Function to generate a 2D vector (a vector with 2 elements) from y and y'
void generate_2d_vec(double *vec, double y, double y_prime) {
    vec[0] = y;
    vec[1] = y_prime;
}

// Function to initialize a 2D vector (vec) into a matrix at a given position
void init_vec_to_mat(double **matrix, double *vec, int position) {
    if (position < N) {
        matrix[0][position] = vec[0];  // First row (y values)
        matrix[1][position] = vec[1];  // Second row (y' values)
    }
}

// Euler's method for solving second-order ODEs
void euler_method_2nd_order(double **matrix, double h, int x0, int xf, double *init_condition) {
    double y, y_prime, x;
    int mat_pos = 0;

    for (x = x0; x <= xf; x += h) {
        if (x == x0) {
            // Initialize with initial conditions
            init_vec_to_mat(matrix, init_condition, mat_pos);
            mat_pos++;
        } else {
            // Update y and y' using Euler's method
            y = matrix[0][mat_pos - 1] + h * matrix[1][mat_pos - 1];
            y_prime = matrix[1][mat_pos - 1] + h * y_prime_prime(x, matrix[0][mat_pos - 1], matrix[1][mat_pos - 1]);

            double vec[2];
            generate_2d_vec(vec, y, y_prime);
            init_vec_to_mat(matrix, vec, mat_pos);
            mat_pos++;
        }
    }
}

