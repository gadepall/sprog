#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 2

// Function to perform LU decomposition
int LUdec(double A[N][N], double L[N][N], double U[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = i; j < N; j++) {
            U[i][j] = A[i][j];
            for (int k = 0; k < i; k++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }
        if (fabs(U[i][i]) < 1e-9) {
            return 0;  // Matrix is singular
        }
        for (int j = i + 1; j < N; j++) {
            L[j][i] = A[j][i];
            for (int k = 0; k < i; k++) {
                L[j][i] -= L[j][k] * U[k][i];
            }
            L[j][i] /= U[i][i];
        }
        L[i][i] = 1.0;  // Diagonal of L is 1
    }
    return 1;
}

// Function to print a matrix
void printMat(const char* name, double matrix[N][N]) {
    printf("%s:\n", name);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.4lf ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to solve the system using forward and back substitution
void systemsolver(double L[N][N], double U[N][N], double b[N], double x[N]) {
    double y[N] = {0};

    // Forward substitution
    for (int i = 0; i < N; i++) {
        y[i] = b[i];
        for (int j = 0; j < i; j++) {
            y[i] -= L[i][j] * y[j];
        }
    }

    // Back substitution
    for (int i = N - 1; i >= 0; i--) {
        x[i] = y[i];
        for (int j = i + 1; j < N; j++) {
            x[i] -= U[i][j] * x[j];
        }
        x[i] /= U[i][i];
    }
    
    // Print the solution
    printf("Solution to the given equations:\n");
    for (int i = 0; i < N; i++) {
        printf("u[%d] = %.4lf\n", i + 1, x[i]);
    }
    printf("\n");
}

int main() {
    double A[N][N] = {{3, 1}, {3, -1}};
    double b[N] = {4, -2};
    double x[N] = {0};
    double L[N][N] = {{0}};
    double U[N][N] = {{0}};

    // Perform LU decomposition
    if (!LUdec(A, L, U)) {
        printf("No solution: The system is singular or the equations are parallel.\n");
        return 1;
    }

    // Solve the system
    systemsolver(L, U, b, x);

    // Print matrices
    printMat("A (original)", A);
    printMat("L (lower triangular)", L);
    printMat("U (upper triangular)", U);

    return 0;
}

