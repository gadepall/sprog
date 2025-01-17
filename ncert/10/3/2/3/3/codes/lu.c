#include <stdio.h>
#include <stdlib.h>

void lu_decomposition(double A[2][2], double L[2][2], double U[2][2]) {
    // Perform LU decomposition for a 2x2 matrix
    for (int i = 0; i < 2; i++) {
        // Upper triangular matrix U
        for (int j = i; j < 2; j++) {
            U[i][j] = A[i][j];
            for (int k = 0; k < i; k++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }

        // Lower triangular matrix L
        for (int j = i + 1; j < 2; j++) {
            L[j][i] = A[j][i];
            for (int k = 0; k < i; k++) {
                L[j][i] -= L[j][k] * U[k][i];
            }
            L[j][i] /= U[i][i];
        }
    }

    // Set diagonal elements of L to 1
    L[0][0] = 1;
    L[1][1] = 1;
}

void forward_substitution(double L[2][2], double B[2], double Y[2]) {
    // Solve for Y in LY = B
    for (int i = 0; i < 2; i++) {
        Y[i] = B[i];
        for (int j = 0; j < i; j++) {
            Y[i] -= L[i][j] * Y[j];
        }
    }
}

void backward_substitution(double U[2][2], double Y[2], double X[2]) {
    // Solve for X in UX = Y
    for (int i = 1; i >= 0; i--) {
        X[i] = Y[i];
        for (int j = i + 1; j < 2; j++) {
            X[i] -= U[i][j] * X[j];
        }
        X[i] /= U[i][i];
    }
}

int main() {
    // Coefficient matrix A and constant vector B from the system of equations
    double A[2][2] = {{3.0 / 2.0, 5.0 / 3.0},
                      {9.0, 10.0}};
    double B[2] = {7.0, 14.0};
    
    // Declare matrices for L and U
    double L[2][2] = {0};
    double U[2][2] = {0};

    // Perform LU decomposition
    lu_decomposition(A, L, U);

    // Check if determinant of U is zero (indicating parallel lines)
    double det_U = U[0][0] * U[1][1] - U[0][1] * U[1][0];
    if (det_U == 0) {
        printf("No solution: Parallel lines\n");
    } else {
        // Solve the system of equations
        double Y[2], X[2];
        forward_substitution(L, B, Y);
        backward_substitution(U, Y, X);

        // Output the solution
        printf("Solution: x = %.2f, y = %.2f\n", X[0], X[1]);
    }

    return 0;
}

