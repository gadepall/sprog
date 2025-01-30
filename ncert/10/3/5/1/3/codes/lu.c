//code by S.Sai Akshita, importing libraries
#include <stdio.h>
#include <stdbool.h>

// Function to perform LU decomposition
void luDecomposition(double A[2][2], double L[2][2], double U[2][2]) {
    // Initialize L and U matrices
    L[0][0] = 1;
    L[0][1] = 0;
    L[1][1] = 1;
    L[1][0] = A[1][0] / A[0][0]; // l21 = A[1][0] / A[0][0]

    U[0][0] = A[0][0]; // u11
    U[0][1] = A[0][1]; // u12
    U[1][0] = 0;       // lower triangle is zero
    U[1][1] = A[1][1] - (L[1][0] * A[0][1]); // u22 = A[1][1] - l21 * A[0][1]
}

// Function to check if the system has infinitely many solutions
bool hasInfinitelyManySolutions(double U[2][2]) {
    return U[1][1] == 0; // If u22 is 0, the system is dependent
}

int main() {
    // Coefficient matrix
    double A[2][2] = {
        {3, -5},
        {6, -10}
    };

    // Matrices for L and U
    double L[2][2] = {0};
    double U[2][2] = {0};

    // Perform LU decomposition
    luDecomposition(A, L, U);

    // Output the L and U matrices
    printf("L matrix:\n");
    printf("%0.2f %0.2f\n", L[0][0], L[0][1]);
    printf("%0.2f %0.2f\n", L[1][0], L[1][1]);

    printf("\nU matrix:\n");
    printf("%0.2f %0.2f\n", U[0][0], U[0][1]);
    printf("%0.2f %0.2f\n", U[1][0], U[1][1]);

    // Check if the system has infinitely many solutions
    if (hasInfinitelyManySolutions(U)) {
        printf("\nThe system of equations has infinitely many solutions (U[1][1] = 0).\n");
    } else {
        printf("\nThe system of equations has a unique solution.\n");
    }

    return 0;
}

