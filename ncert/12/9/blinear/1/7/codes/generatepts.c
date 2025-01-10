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
        A[order - 1][j] = (-1.0 * coefficients[j]) / coefficients[order]; // Last row for higher-order ODE coefficients
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

// Matrix addition/subtraction
void matrix_add(double* result, const double* A, const double* B, int order, double scaleA, double scaleB) {
    for (int i = 0; i < order; i++) {
        result[i] = scaleA * A[i] + scaleB * B[i];
    }
}

//Used to solve Ax=b System
void solve_linear_system(double** A, const double* b, double* x, int order, int iterations) {
    double* temp = (double*)malloc(order * sizeof(double));
    for (int i = 0; i < order; i++) x[i] = 0.0; // Initial guess

    for (int k = 0; k < iterations; k++) {
        for (int i = 0; i < order; i++) {
            temp[i] = b[i];
            for (int j = 0; j < order; j++) {
                if (j != i) temp[i] -= A[i][j] * x[j];
            }
            x[i] = temp[i] / A[i][i];
        }
    }
    free(temp);
}

// Bilinear transform trapezoidal method for solving ODE
double* bilinear_trapezoidal_method(int N, double h, const double* y0, const double* coefficients, int order) {
    // Allocate and create the coefficient matrix
    double** A = (double**)malloc(order * sizeof(double*));
    for (int i = 0; i < order; i++) {
        A[i] = (double*)malloc(order * sizeof(double));
    }
    create_coefficient_matrix(A, coefficients, order);

    // Compute B1 and B2 matrices for the bilinear transform
    double** B1 = (double**)malloc(order * sizeof(double*));
    double** B2 = (double**)malloc(order * sizeof(double*));
    for (int i = 0; i < order; i++) {
        B1[i] = (double*)malloc(order * sizeof(double));
        B2[i] = (double*)malloc(order * sizeof(double));
        for (int j = 0; j < order; j++) {
            B1[i][j] = (i == j ? 1.0 : 0.0) - (h / 2.0) * A[i][j];
            B2[i][j] = (i == j ? 1.0 : 0.0) + (h / 2.0) * A[i][j];
        }
    }

    // Allocate arrays for the solution
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

    // Iterative solution using bilinear transform
    for (int k = 0; k < N; k++) {
        double* rhs = (double*)malloc(order * sizeof(double));
        double* next_Y = (double*)malloc(order * sizeof(double));

        // Compute RHS: B2 * Y[k]
        matrix_multiply(B2, Y[k], rhs, order);

        // Solve for Y[k+1] using B1 * Y[k+1] = rhs
        solve_linear_system(B1, rhs, next_Y, order, 100); // Solve using an iterative method

        // Store the result
        for (int j = 0; j < order; j++) {
            Y[k + 1][j] = next_Y[j];
        }
        y1_values[k + 1] = Y[k + 1][0];

        free(rhs);
        free(next_Y);
    }

    // Free allocated memory
    for (int i = 0; i < order; i++) {
        free(A[i]);
        free(B1[i]);
        free(B2[i]);
    }
    free(A);
    free(B1);
    free(B2);

    for (int i = 0; i <= N; i++) {
        free(Y[i]);
    }
    free(Y);

    return y1_values;
}

int main() {
    // Example usage
    int order = 3;
    double coefficients[] = {1.0, 2.0, 1.0, 1.0}; // Coefficients of y''', y'', y', y
    double y0[] = {1.0, 0.0, 0.0};                // Initial conditions
    double h = 0.1;                               // Step size
    int N = 100;                                  // Number of steps

    double* y1_values = bilinear_trapezoidal_method(N, h, y0, coefficients, order);

    // Print the results
    for (int i = 0; i <= N; i++) {
        printf("Step %d: y = %f\n", i, y1_values[i]);
    }

    free(y1_values);
    return 0;
}

