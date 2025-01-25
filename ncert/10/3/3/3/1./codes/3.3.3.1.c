#include <stdio.h>

#define N 2  // Size of the matrix (2x2 system)

// Function to perform LU decomposition
void luDecomposition(double A[N][N], double L[N][N], double U[N][N]) {
    for (int i = 0; i < N; i++) {
        // Upper triangular matrix U
        for (int j = i; j < N; j++) {
            U[i][j] = A[i][j];
            for (int k = 0; k < i; k++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }

        // Lower triangular matrix L
        for (int j = i; j < N; j++) {
            if (i == j)
                L[i][i] = 1; // Diagonal elements of L are 1
            else {
                L[j][i] = A[j][i];
                for (int k = 0; k < i; k++) {
                    L[j][i] -= L[j][k] * U[k][i];
                }
                L[j][i] /= U[i][i];
            }
        }
    }
}

// Function to perform forward substitution to solve L*y = b
void forwardSubstitution(double L[N][N], double b[N], double y[N]) {
    for (int i = 0; i < N; i++) {
        y[i] = b[i];
        for (int j = 0; j < i; j++) {
            y[i] -= L[i][j] * y[j];
        }
        y[i] /= L[i][i];
    }
}

// Function to perform backward substitution to solve U*x = y
void backwardSubstitution(double U[N][N], double y[N], double x[N]) {
    for (int i = N - 1; i >= 0; i--) {
        x[i] = y[i];
        for (int j = i + 1; j < N; j++) {
            x[i] -= U[i][j] * x[j];
        }
        x[i] /= U[i][i];
    }
}

// Main function to solve the system Ax = b
void solution(double A[N][N], double b[N], double x[N]) {
    double L[N][N] = {0}; // Initialize L as a zero matrix
    double U[N][N] = {0}; // Initialize U as a zero matrix
    double y[N];    // y is the intermediate solution

    // Step 1: Perform LU decomposition on A
    luDecomposition(A, L, U);

    // Step 2: Solve L * y = b using forward substitution
    forwardSubstitution(L, b, y);

    // Step 3: Solve U * x = y using backward substitution
    backwardSubstitution(U, y, x);
}

