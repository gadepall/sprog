#include <stdio.h>  
#include <math.h>   
// Define a constant N to count the number of readings
#define N 99

// Function to calculate y'' (second derivative of y with respect to x)
double y_prime_prime(double y_prime) {
    return cbrt(-(y_prime * y_prime) - sin(y_prime) - 1);
}

// Function to generate a 2D vector (a vector with 2 elements) from y and y'
void generate_2d_vec(double *vec, double y, double y_prime) {
    // Assign the values of y and y' to the vector
    vec[0] = y;
    vec[1] = y_prime;
}

// Function to initialize a 2D vector (vec) into a matrix at a given position
// This function stores the vector values into the matrix at a specified position
void init_vec_to_mat(double **matrix, double *vec, int position) {
    if (position < N) {
        // Store vec[0] and vec[1] into the matrix at the given position
        matrix[0][position] = vec[0];  // First row of matrix (y values)
        matrix[1][position] = vec[1];  // Second row of matrix (y' values)
    }
}

// Euler's method for solving second-order ordinary differential equations (ODEs)
// This method approximates the solution by discretizing the domain
void euler_method_2nd_order(double **matrix, double h, int x0, int xf, double *init_condition) {
    double y, y_prime;  // Variables to store the current values of y and y'
    int mat_pos = 0;    // Matrix position tracker

    // Loop through the x values from x0 to xf with a step size of h
    for (double x = x0; x <= xf; x += h) {
        if (x == x0) {
            // If it's the first iteration, initialize the matrix with the initial conditions
            init_vec_to_mat(matrix, init_condition, mat_pos);
            mat_pos++;  // Move to the next matrix position
        } else {
            // Calculate the new values of y and y' using Euler's method
            // y = y(previous) + h * y'(previous)
            y = matrix[0][mat_pos - 1] + h * matrix[1][mat_pos - 1];
            
            // y' = y'(previous) + h * y''(previous)
            // y'' is calculated using the `y_prime_prime` function
            y_prime = matrix[1][mat_pos - 1] + h * y_prime_prime(matrix[1][mat_pos - 1]);

            // Generate the 2D vector [y, y'] and store it in the matrix at the current position
            double vec[2];  // Vector to hold the new values of y and y'
            generate_2d_vec(vec, y, y_prime);
            init_vec_to_mat(matrix, vec, mat_pos);
            mat_pos++;  // Move to the next matrix position
        }
    }
}
