#include <stdio.h>

#define N 2  // Size of the system

void lu_decomposition(double A[N][N], double L[N][N], double U[N][N]) {
    for (int i = 0; i < N; i++) {
        // Initialize L matrix diagonal to 1
        L[i][i] = 1.0;
        
        for (int j = i; j < N; j++) {
            // Compute U matrix
            U[i][j] = A[i][j];
            for (int k = 0; k < i; k++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }
        
        for (int j = i + 1; j < N; j++) {
            // Compute L matrix
            L[j][i] = A[j][i];
            for (int k = 0; k < i; k++) {
                L[j][i] -= L[j][k] * U[k][i];
            }
            L[j][i] /= U[i][i];
        }
    }
}

void forward_substitution(double L[N][N], double B[N], double Y[N]) {
    for (int i = 0; i < N; i++) {
        Y[i] = B[i];
        for (int j = 0; j < i; j++) {
            Y[i] -= L[i][j] * Y[j];
        }
    }
}

void backward_substitution(double U[N][N], double Y[N], double X[N]) {
    for (int i = N - 1; i >= 0; i--) {
        X[i] = Y[i];
        for (int j = i + 1; j < N; j++) {
            X[i] -= U[i][j] * X[j];
        }
        X[i] /= U[i][i];
    }
}

void solve(double *result) {
    // Matrix A represents coefficients of v and c
    double A[N][N] = {
        {1,  1},  // v + c = 10 (downstream)
        {1, -1}   // v - c = 2  (upstream)
    };
    
    // Vector B contains the right-hand side values
    double B[N] = {10, 2};
    
    // Initialize matrices
    double L[N][N] = {{0}};
    double U[N][N] = {{0}};
    double Y[N], X[N];

    // Perform LU decomposition
    lu_decomposition(A, L, U);

    // Solve LY = B using forward substitution
    forward_substitution(L, B, Y);

    // Solve UX = Y using backward substitution
    backward_substitution(U, Y, X);

    // Store results
    result[0] = X[0];  // Speed in still water (v)
    result[1] = X[1];  // Speed of current (c)
}

int main() {
    double result[N];
    solve(result);
    
    printf("Speed in still water = %.2f km/h\n", result[0]);
    printf("Speed of current = %.2f km/h\n", result[1]);
    
    return 0;
}

