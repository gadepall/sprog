#include <stdio.h>

#define N 2  // Size of the matrix

// LU Decomposition
void luDecomposition(float matrix[N][N], float L[N][N], float U[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = i; j < N; j++) {
            U[i][j] = matrix[i][j];
            for (int k = 0; k < i; k++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }
        for (int j = i; j < N; j++) {
            if (i == j) {
                L[i][i] = 1.0;  // Diagonal elements of L are 1
            } else {
                L[j][i] = matrix[j][i];
                for (int k = 0; k < i; k++) {
                    L[j][i] -= L[j][k] * U[k][i];
                }
                L[j][i] /= U[i][i];
            }
        }
    }
}

// Forward Substitution
void forwardSubstitution(float L[N][N], float b[N], float y[N]) {
    for (int i = 0; i < N; i++) {
        y[i] = b[i];
        for (int j = 0; j < i; j++) {
            y[i] -= L[i][j] * y[j];
        }
        y[i] /= L[i][i];
    }
}

// Backward Substitution
void backwardSubstitution(float U[N][N], float y[N], float x[N]) {
    for (int i = N - 1; i >= 0; i--) {
        x[i] = y[i];
        for (int j = i + 1; j < N; j++) {
            x[i] -= U[i][j] * x[j];
        }
        x[i] /= U[i][i];
    }
}

// Function to solve Ax = b and return the solution
void solve(float matrix[N][N], float b[N], float x[N]) {
    float L[N][N] = {0};  // Lower triangular matrix
    float U[N][N] = {0};  // Upper triangular matrix
    float y[N] = {0};      // Intermediate solution

    // Perform LU decomposition
    luDecomposition(matrix, L, U);

    // Solve L * y = b using forward substitution
    forwardSubstitution(L, b, y);

    // Solve U * x = y using backward substitution
    backwardSubstitution(U, y, x);
}
