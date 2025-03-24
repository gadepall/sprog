#include <stdio.h>

void lu_decomposition(double A[2][2], double L[2][2], double U[2][2]) {
    int i, j, k;

    // Initialize L as identity matrix and U as zero matrix
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if (i == j)
                L[i][j] = 1;
            else
                L[i][j] = 0;
            U[i][j] = 0;
        }
    }

    // Perform LU Decomposition
    for (i = 0; i < 2; i++) {
        // Upper triangular matrix U
        for (j = i; j < 2; j++) {
            U[i][j] = A[i][j];
            for (k = 0; k < i; k++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }
        // Lower triangular matrix L
        for (j = i + 1; j < 2; j++) {
            L[j][i] = A[j][i];
            for (k = 0; k < i; k++) {
                L[j][i] -= L[j][k] * U[k][i];
            }
            L[j][i] /= U[i][i];
        }
    }
}

void print_matrix(double matrix[2][2]) {
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // Input matrix A (2x2)
    double A[2][2] = {
        {3, 4},
        {2, -2}
    };

    // Matrices L and U
    double L[2][2], U[2][2];

    // Perform LU decomposition
    lu_decomposition(A, L, U);

    // Print the result
    printf("Lower triangular matrix L:\n");
    print_matrix(L);

    printf("\nUpper triangular matrix U:\n");
    print_matrix(U);

    return 0;
}

