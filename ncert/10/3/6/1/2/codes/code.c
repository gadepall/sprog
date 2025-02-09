#include <stdio.h>
#include <stdlib.h>

#define N 2

// Function to perform LU decomposition and solve the system
float** LUsolver(int n) {
    float A[N][N] = {
        {2, 3},
        {4, -9}
    };

    // Right-hand side vector b
    float b[N] = {2, -1};

    // Allocate memory for L and U matrices
    float L[N][N] = {0};
    float U[N][N] = {0};

    // Initialize L as identity matrix and copy A to U
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            L[i][j] = (i == j) ? 1.0 : 0.0;
            U[i][j] = A[i][j];
        }
    }

    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            float factor = U[j][i] / U[i][i];
            L[j][i] = factor;
            for (int k = i; k < N; k++) {
                U[j][k] -= factor * U[i][k];
            }
        }
    }
    // Solve Ly = b using forward substitution
    float y[N] = {0};
    for (int i = 0; i < N; i++) {
        y[i] = b[i];
        for (int j = 0; j < i; j++) {
            y[i] -= L[i][j] * y[j];
        }
    }
    // Solve Ux = y using back substitution
    float** x = (float**)malloc(N * sizeof(float*));
    for (int i = 0; i < N; i++) {
        x[i] = (float*)malloc(sizeof(float));
    }
    for (int i = N - 1; i >= 0; i--) {
        x[i][0] = y[i];
        for (int j = i + 1; j < N; j++) {
            x[i][0] -= U[i][j] * x[j][0];
        }
        x[i][0] /= U[i][i];
    }
    return x;
}
