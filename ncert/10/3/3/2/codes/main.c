#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 2

void LUdec(double A[N][N], double L[N][N], double U[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = i; j < N; j++) {
            U[i][j] = A[i][j];
            for (int k = 0; k < i; k++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }
        for (int j = i; j < N; j++) {
            if (i == j) {
                L[i][i] = 1.0;
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

void systemsolver(double L[N][N], double U[N][N], double b[N], double x[N]) {
    double y[N] = {0};
    // Forward substitution
    for (int i = 0; i < N; i++) {
        y[i] = b[i];
        for (int j = 0; j < i; j++) {
            y[i] -= L[i][j] * y[j];
        }
        y[i] /= L[i][i];
    }
    // Back substitution to find x
    for (int i = N - 1; i >= 0; i--) {
        x[i] = y[i];
        for (int j = i + 1; j < N; j++) {
            x[i] -= U[i][j] * x[j];
        }
        x[i] /= U[i][i];
    }
}

// Function to be called from Python
void solve_system(double result[N]) {
    double A[N][N] = {
        {2, 3},
        {2, -4}
    };
    double b[N] = {11, -24};
    double x[N] = {0};
    double L[N][N] = {0};
    double U[N][N] = {0};

    LUdec(A, L, U);
    systemsolver(L, U, b, x);

    result[0] = x[0];
    result[1] = x[1];
}

int main() {
    // Optional: For testing purposes
    double result[N] = {0};
    solve_system(result);
    printf("Solution: x = %.4lf, y = %.4lf\n", result[0], result[1]);
    return 0;
}

