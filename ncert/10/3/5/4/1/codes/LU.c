#include <stdio.h>

#define N 2  // Size of the system

void lu_decomposition(double A[N][N], double L[N][N], double U[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = i; j < N; j++) {
            // Compute U matrix
            U[i][j] = A[i][j];
            for (int m = 0; m < i; m++)
                U[i][j] -= L[i][m] * U[m][j];
        }

        for (int j = i + 1; j < N; j++) {
            // Compute L matrix
            L[j][i] = A[j][i];
            for (int m = 0; m < i; m++)
                L[j][i] -= L[j][m] * U[m][i];
            L[j][i] /= U[i][i];  // Normalize
        }
    }
}

void forward_substitution(double L[N][N], double B[N], double Y[N]) {
    for (int i = 0; i < N; i++) {
        Y[i] = B[i];
        for (int j = 0; j < i; j++)
            Y[i] -= L[i][j] * Y[j];
    }
}

void backward_substitution(double U[N][N], double Y[N], double X[N]) {
    for (int i = N - 1; i >= 0; i--) {
        X[i] = Y[i];
        for (int j = i + 1; j < N; j++)
            X[i] -= U[i][j] * X[j];
        X[i] /= U[i][i];
    }
}

void solve(double *result) {
    double A[N][N] = {{1, 20}, {1, 26}};
    double B[N] = {1000, 1080};
    double L[N][N] = {{1, 0}, {0, 1}};
    double U[N][N] = {{0, 0}, {0, 0}};
    double Y[N], X[N];

    // Perform LU decomposition
    lu_decomposition(A, L, U);

    // Solve LY = B using forward substitution
    forward_substitution(L, B, Y);

    // Solve UX = Y using backward substitution
    backward_substitution(U, Y, X);

    // Store the result
    result[0] = X[0];
    result[1] = X[1];
}

