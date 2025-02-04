#include <stdio.h>
#include <stdlib.h>

// Function to perform LU decomposition using Doolittle's Algorithm
void doolittle_lu(double A[2][2], double L[2][2], double U[2][2]) {
    int i, j, k;
    
    // Initialize L and U to zero
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            L[i][j] = 0;
            U[i][j] = 0;
        }
    }

    // Compute LU decomposition
    for (i = 0; i < 2; i++) {
        for (j = i; j < 2; j++) {
            // Compute U matrix
            U[i][j] = A[i][j];
            for (k = 0; k < i; k++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }

        for (j = i; j < 2; j++) {
            if (i == j)
                L[i][i] = 1;  // Set diagonal of L to 1
            else {
                // Compute L matrix
                L[j][i] = A[j][i];
                for (k = 0; k < i; k++) {
                    L[j][i] -= L[j][k] * U[k][i];
                }
                L[j][i] /= U[i][i];
            }
        }
    }
}

// Function to solve Ax = B using LU decomposition
void solve_lu(double L[2][2], double U[2][2], double B[2], double *F, double *C) {
    double Y[2];

    // Forward substitution (Ly = B)
    Y[0] = B[0];
    Y[1] = B[1] - L[1][0] * Y[0];

    // Backward substitution (Ux = Y)
    *C = Y[1] / U[1][1];
    *F = (Y[0] - U[0][1] * (*C)) / U[0][0];
}

// Generate points for plotting
void generate_points(double points1[10][2], double points2[10][2]) {
    for (int x = 0; x < 10; x++) {
        points1[x][0] = x;
        points1[x][1] = (27 - x) / 4.0;  // Equation: F + 4C = 27
        
        points2[x][0] = x;
        points2[x][1] = (21 - x) / 2.0;  // Equation: F + 2C = 21
    }
}

// Exposing function to Python
#include <stdint.h>
#include <stddef.h>

void solve_and_generate(double *F, double *C, double points1[10][2], double points2[10][2]) {
    double A[2][2] = {{1, 4}, {1, 2}};
    double B[2] = {27, 21};

    double L[2][2], U[2][2];
    doolittle_lu(A, L, U);
    solve_lu(L, U, B, F, C);
    generate_points(points1, points2);
}

