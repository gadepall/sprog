#include <stdio.h>
#include <stdlib.h>

// Function to create matrix to form derivatives at each step
void create_coefficient_matrix(double** A, const double* coefficients, int order) {
    // Initialize the matrix with zeros
    for (int i = 0; i < order; i++) {
        for (int j = 0; j < order; j++) {
            A[i][j] = 0.0;
        }
    }

    // Fill the matrix
    for (int i = 0; i < order - 1; i++) {
        A[i][i + 1] = 1.0; // Set the superdiagonal to 1
    }
    for (int j = 0; j < order; j++) {
        A[order - 1][j] = (-1.0*coefficients[j])/coefficients[order]; // Last row for higher-order ODE coefficients
    }
}

// Matrix multiplication
void matrix_multiply(double** A, const double* Y, double* result, int order) {
    for (int i = 0; i < order; i++) {
        result[i] = 0.0;
        for (int j = 0; j < order; j++) {
            result[i] += A[i][j] * Y[j];
        }
    }
}

// Matrix method to compute each derivative upto order of differential equation of each x in the range given with step-size h given
double* matrix_method(int N, double h, const double* y0, const double* coefficients, int order) {
    // Allocate and create the coefficient matrix
    double** A = (double**)malloc(order * sizeof(double*));
    for (int i = 0; i < order; i++) {
        A[i] = (double*)malloc(order * sizeof(double));
    }
    create_coefficient_matrix(A, coefficients, order);

    double* y1_values = (double*)malloc((N + 1) * sizeof(double));
    double** Y = (double**)malloc((N + 1) * sizeof(double*));
    for (int i = 0; i <= N; i++) {
        Y[i] = (double*)malloc(order * sizeof(double));
    }

    // Set initial conditions
    for (int j = 0; j < order; j++) {
        Y[0][j] = y0[j];
    }
    y1_values[0] = Y[0][0];

    // Perform Euler's method
    for (int i = 0; i < N; i++) {
        double* dydt = (double*)malloc(order * sizeof(double));
        matrix_multiply(A, Y[i], dydt, order);

        for (int j = 0; j < order; j++) {
            Y[i + 1][j] = Y[i][j] + h * dydt[j];
        }
        y1_values[i + 1] = Y[i + 1][0];
        free(dydt);
    }

    // Free allocated memory for the coefficient matrix and intermediate arrays
    for (int i = 0; i < order; i++) {
        free(A[i]);
    }
    free(A);
    for (int i = 0; i <= N; i++) {
        free(Y[i]);
    }
    free(Y);

    return y1_values;
}



