#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 100 // Define the maximum size of the matrix

// Function to perform Crout's LU decomposition
void LU_Crouts(double** A, double** L, double** U, int n) {
    for (int i = 0; i < n; i++) {
        // Compute L matrix
        for (int j = i; j < n; j++) {
            L[j][i] = A[j][i];
            for (int k = 0; k < i; k++) {
                L[j][i] -= L[j][k] * U[k][i];
            }
        }

        // Compute U matrix
        for (int j = i; j < n; j++) {
            if (i == j) {
                U[i][j] = 1.0; // Diagonal of U is 1
            } else {
                U[i][j] = A[i][j];
                for (int k = 0; k < i; k++) {
                    U[i][j] -= L[i][k] * U[k][j];
                }
                U[i][j] /= L[i][i];
            }
        }
    }
}

// Function to perform Doolittle's LU decomposition
void LU_Doolittle(double** A, double** L, double** U, int n) {
    for (int i = 0; i < n; i++) {
        // Compute U matrix
        for (int j = i; j < n; j++) {
            U[i][j] = A[i][j];
            for (int k = 0; k < i; k++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }

        // Compute L matrix
        for (int j = i; j < n; j++) {
            if (i == j) {
                L[i][i] = 1.0; // Diagonal of L is 1
            } else {
                L[j][i] = A[j][i];
                for (int k = 0; k < i; k++) {
                    L[j][i] -= L[j][k] * U[k][i];
                }
                L[j][i] /= U[i][i];
            }
        }
    }
}

// Solver for Crout's method
void CroutSolver(double** L, double** U, double* b, double* x, int n) {
    double y[N] = {0};

    // Forward substitution (Ly = b)
    for (int i = 0; i < n; i++) {
        y[i] = b[i];
        for (int j = 0; j < i; j++) {
            y[i] -= L[i][j] * y[j];
        }
        y[i] /= L[i][i];
    }

    // Backward substitution (Ux = y)
    for (int i = n - 1; i >= 0; i--) {
        x[i] = y[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= U[i][j] * x[j];
        }
        x[i] /= U[i][i];
    }

    // Print the solution
    printf("Solution using Crout's method:\n");
    for (int i = 0; i < n; i++) {
        printf("x[%d] = %.4lf\n", i, x[i]);
    }
}

// Solver for Doolittle's method
void DoolittleSolver(double** L, double** U, double* b, double* x, int n) {
    double y[N] = {0};

    // Forward substitution (Ly = b)
    for (int i = 0; i < n; i++) {
        y[i] = b[i];
        for (int j = 0; j < i; j++) {
            y[i] -= L[i][j] * y[j];
        }
        y[i] /= L[i][i];
    }

    // Backward substitution (Ux = y)
    for (int i = n - 1; i >= 0; i--) {
        x[i] = y[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= U[i][j] * x[j];
        }
        x[i] /= U[i][i];
    }

    // Print the solution
    printf("Solution using Doolittle's method:\n");
    for (int i = 0; i < n; i++) {
        printf("x[%d] = %.4lf\n", i, x[i]);
    }
}

int main() {
    int n;
    printf("Enter the size of the matrix: ");
    scanf("%d", &n);

    double** A = (double**)malloc(n * sizeof(double*));
    double** L = (double**)malloc(n * sizeof(double*));
    double** U = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        A[i] = (double*)malloc(n * sizeof(double));
        L[i] = (double*)malloc(n * sizeof(double));
        U[i] = (double*)malloc(n * sizeof(double));
    }
    double* b = (double*)malloc(n * sizeof(double));
    double* x = (double*)malloc(n * sizeof(double));

    // Input matrix elements
    printf("Enter the matrix elements:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%lf", &A[i][j]);
        }
    }

    // Input vector B
    printf("Enter the vector B:\n");
    for (int i = 0; i < n; i++) {
        scanf("%lf", &b[i]);
    }

    // Perform Crout's decomposition and solve
    LU_Crouts(A, L, U, n);
    CroutSolver(L, U, b, x, n);

    // Perform Doolittle's decomposition and solve
    LU_Doolittle(A, L, U, n);
    DoolittleSolver(L, U, b, x, n);

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        free(A[i]);
        free(L[i]);
        free(U[i]);
    }
    free(A);
    free(L);
    free(U);
    free(b);
    free(x);

    return 0;
}

