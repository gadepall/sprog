#include <stdio.h>
#include <stdlib.h>

// Function to perform LU decomposition and solve Ax = b
void lu_solve(double A[2][2], double b[2], double x[2]) {
    double L[2][2] = {{1, 0}, {A[1][0] / A[0][0], 1}};
    double U[2][2] = {{A[0][0], A[0][1]}, {0, A[1][1] - L[1][0] * A[0][1]}};
    
    // Forward substitution to solve Ly = b
    double y[2];
    y[0] = b[0];
    y[1] = b[1] - L[1][0] * y[0];
    
    // Backward substitution to solve Ux = y
    x[1] = y[1] / U[1][1];
    x[0] = (y[0] - U[0][1] * x[1]) / U[0][0];
}

// Function to be called from Python
void solve_system(double *x_out) {
    double A[2][2] = {{1, -1}, {2, -1}};
    double b[2] = {-2, 1};
    double x[2];
    
    lu_solve(A, b, x);
    
    x_out[0] = x[0];
    x_out[1] = x[1];
}

