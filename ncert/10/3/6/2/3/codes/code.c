#include <stdio.h>

// Function for LU decomposition
void luDecomposition(double A[2][2], double L[2][2], double U[2][2]) {
    // Initialize L and U as identity and zero matrices
    L[0][0] = 1;
    L[0][1] = 0;
    L[1][0] = A[1][0] / A[0][0];
    L[1][1] = 1;

    U[0][0] = A[0][0];
    U[0][1] = A[0][1];
    U[1][0] = 0;
    U[1][1] = A[1][1] - L[1][0] * U[0][1];
}

// Function for forward substitution (solves L * y = B)
void forwardSubstitution(double L[2][2], double B[2], double Y[2]) {
    Y[0] = B[0];
    Y[1] = B[1] - L[1][0] * Y[0];
}

// Function for back substitution (solves U * X = Y)
void backSubstitution(double U[2][2], double Y[2], double X[2]) {
    X[1] = Y[1] / U[1][1];
    X[0] = (Y[0] - U[0][1] * X[1]) / U[0][0];
}

int main() {
    // Coefficient matrix A
    double A[2][2] = {
        {60, 240},
        {100, 200}
    };

    // Right-hand side vector B
    double B[2] = {4, 25.0 / 6.0};

    // Matrices L and U
    double L[2][2], U[2][2];

    // Solve for a and b
    double Y[2], X[2];

    // Perform LU decomposition
    luDecomposition(A, L, U);

    // Solve LY = B using forward substitution
    forwardSubstitution(L, B, Y);

    // Solve UX = Y using backward substitution
    backSubstitution(U, Y, X);

    // Compute the speeds
    double speedTrain = 1.0 / X[0];
    double speedBus = 1.0 / X[1];

    // Display results
    printf("Speed of train: %.2f km/h\n", speedTrain);
    printf("Speed of bus: %.2f km/h\n", speedBus);

    return 0;
}

