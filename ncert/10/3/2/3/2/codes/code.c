#include <stdio.h>
#include <stdlib.h>

void lu_decomposition(double* A, double* L, double* U, int n) {
    // Initialize L as identity and U as zero matrix
    for (int i = 0; i < n; i++) {
        L[i * n + i] = 1.0;  // Set diagonal of L to 1
        for (int j = 0; j < n; j++) {
            U[i * n + j] = 0.0;  // Initialize U to 0
        }
    }

    // Perform LU decomposition
    for (int k = 0; k < n; k++) {
        // Compute U[k, j] for j >= k
        for (int j = k; j < n; j++) {
            U[k * n + j] = A[k * n + j];
            for (int m = 0; m < k; m++) {
                U[k * n + j] -= L[k * n + m] * U[m * n + j];
            }
        }

        // Compute L[i, k] for i > k
        for (int i = k + 1; i < n; i++) {
            L[i * n + k] = A[i * n + k];
            for (int m = 0; m < k; m++) {
                L[i * n + k] -= L[i * n + m] * U[m * n + k];
            }
            L[i * n + k] /= U[k * n + k];  // Avoid division by zero
        }
    }
}

void print_matrix(double* mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%lf ", mat[i * n + j]);
        }
        printf("\n");
    }
}

int main() {
    int n = 3;  // Size of the matrix

    // Example matrix A (3x3 for demonstration)
    double A[] = {2, 3, 4, 6, 13, 9, 5, -2, 6};
    double L[9] = {0};  // Initialize L as a zero matrix
    double U[9] = {0};  // Initialize U as a zero matrix

    // Perform LU Decomposition
    lu_decomposition(A, L, U, n);

    // Print matrices L and U
    printf("Matrix L:\n");
    print_matrix(L, n);

    printf("Matrix U:\n");
    print_matrix(U, n);

    return 0;
}

