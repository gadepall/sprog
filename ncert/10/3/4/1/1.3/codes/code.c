#include <stdio.h>
#include <stdlib.h>

// Function to create and allocate memory for an m x n matrix
float** create_mat(int M, int N) {
    float** matrix = malloc(sizeof(float*) * M);
    for (int i = 0; i < M; i++) {
        matrix[i] = malloc(sizeof(float) * N);
    }
    return matrix;
}

// Function to print a matrix
void print_mat(float** A, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }
}

// Function to solve the system of equations using LU decomposition
float** LUsolver(int n) {
    // Coefficient matrix A
    float A[2][2] = {
        {3, -5},
        {9, -2}
    };

    // Right-hand side vector B
    float B[2] = {4, 7};

    // Allocate memory for L and U matrices
    float** L = create_mat(n, n);
    float** U = create_mat(n, n);

    // Initialize L as identity matrix and copy A to U
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            L[i][j] = (i == j) ? 1 : 0;
            U[i][j] = A[i][j];
        }
    }

    // Perform LU decomposition
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            float factor = U[j][i] / U[i][i];
            L[j][i] = factor;
            for (int k = i; k < n; k++) {
                U[j][k] -= factor * U[i][k];
            }
        }
    }

    // Print L and U matrices
    printf("L matrix:\n");
    print_mat(L, n, n);

    printf("U matrix:\n");
    print_mat(U, n, n);

    // Solve Ly = B using forward substitution
    float* y = malloc(sizeof(float) * n);
    for (int i = 0; i < n; i++) {
        y[i] = B[i];
        for (int j = 0; j < i; j++) {
            y[i] -= L[i][j] * y[j];
        }
    }

    // Print y vector
    printf("y vector:\n");
    for (int i = 0; i < n; i++) {
        printf("%f\n", y[i]);
    }

    // Solve Ux = y using back substitution
    float** x = create_mat(n, 1);
    for (int i = n - 1; i >= 0; i--) {
        x[i][0] = y[i];
        for (int j = i + 1; j < n; j++) {
            x[i][0] -= U[i][j] * x[j][0];
        }
        x[i][0] /= U[i][i];
    }

    // Print x vector (solution)
    printf("x vector (solution):\n");
    for (int i = 0; i < n; i++) {
        printf("%f\n", x[i][0]);
    }

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        free(L[i]);
        free(U[i]);
    }
    free(L);
    free(U);
    free(y);

    return x;
}

int main() {
    int n = 2; // Size of the system (2x2 matrix)
    float** solution = LUsolver(n);

    // Free solution memory
    for (int i = 0; i < n; i++) {
        free(solution[i]);
    }
    free(solution);

    return 0;
}
