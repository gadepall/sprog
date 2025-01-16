#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Function to perform matrix multiplication
void multiplyMatrices(double A[2][2], double B[2][2], double result[2][2]) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < 2; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to compute the QR decomposition of a 2x2 matrix
void qrDecomposition(double A[2][2], double Q[2][2], double R[2][2]) {
    // Compute the norm of the first column of A
    double norm = sqrt(A[0][0] * A[0][0] + A[1][0] * A[1][0]);

    // Create the orthonormal vector (Q)
    Q[0][0] = A[0][0] / norm;
    Q[1][0] = A[1][0] / norm;
    Q[0][1] = -A[1][0] / norm;
    Q[1][1] = A[0][0] / norm;

    // Compute R = Q^T * A
    R[0][0] = Q[0][0] * A[0][0] + Q[1][0] * A[1][0];
    R[0][1] = Q[0][0] * A[0][1] + Q[1][0] * A[1][1];
    R[1][0] = 0; // By design of Q
    R[1][1] = Q[0][1] * A[0][1] + Q[1][1] * A[1][1];
}

void findEigenvalues(double A[2][2], double tolerance, int maxIterations) {
    double Q[2][2], R[2][2], temp[2][2];
    for (int iter = 0; iter < maxIterations; iter++) {
        qrDecomposition(A, Q, R);       // Perform QR decomposition
        multiplyMatrices(R, Q, temp);  // Update A = R * Q

        // Check for convergence
        if (fabs(temp[1][0]) < tolerance) {
            break;
        }

        // Update the matrix A
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                A[i][j] = temp[i][j];
            }
        }
    }

    // Eigenvalues are the diagonal elements of A
    printf("Eigenvalues (roots of the equation):\n");
    printf("Root 1: %.6f\n", A[0][0]);
    printf("Root 2: %.6f\n", A[1][1]);
}

int main() {
    // Coefficients of the quadratic equation
    double a = 100, b = -20, c = 1;

    // Construct the companion matrix
    double A[2][2] = {
        {0, -c / a},
        {1, -b / a}
    };

    // Parameters for QR iteration
    double tolerance = 1e-9;
    int maxIterations = 100;

    // Find eigenvalues
    findEigenvalues(A, tolerance, maxIterations);

    return 0;
}

