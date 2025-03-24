#include <stdio.h>

// Function for LU decomposition
void luDecomposition(double A[2][2], double L[2][2], double U[2][2]) {
    // Initialize L and U as identity and zero matrices
    L[0][0] = 1;
    L[0][1] = 0;
    L[1][0] = A[1][0] / A[0][0];
    L[1][1] = 1;

    U[0][0] = A[0][0];
    U[0][1] = A[0][1];
    U[1][0] = 0;
    U[1][1] = A[1][1] - L[1][0] * U[0][1];
}

// Function for forward substitution (solves L * y = b)
void forwardSubstitution(double L[2][2], double b[2], double y[2]) {
    y[0] = b[0];
    y[1] = b[1] - L[1][0] * y[0];
}

// Function for back substitution (solves U * x = y)
void backSubstitution(double U[2][2], double y[2], double x[2]) {
    x[1] = y[1] / U[1][1];
    x[0] = (y[0] - U[0][1] * x[1]) / U[0][0];
}

