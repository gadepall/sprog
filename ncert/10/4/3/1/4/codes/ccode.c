#include <stdio.h>
#include <math.h>

#define TOLERANCE 1e-6
#define MAX_ITER 1000

// Function to print the matrix
void printMatrix(double matrix[2][2]) {
    printf("\nMatrix:\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%lf\t", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function to perform the QR decomposition using Gram-Schmidt
void qrDecomposition(double A[2][2], double Q[2][2], double R[2][2]) {
    double norm = sqrt(A[0][0]*A[0][0] + A[1][0]*A[1][0]);
    
    // Q Column 1
    Q[0][0] = A[0][0] / norm;
    Q[1][0] = A[1][0] / norm;
    
    // R(1,1)
    R[0][0] = norm;

    // Compute the second column of Q
    double dot = Q[0][0]*A[0][1] + Q[1][0]*A[1][1];
    Q[0][1] = A[0][1] - dot * Q[0][0];
    Q[1][1] = A[1][1] - dot * Q[1][0];

    norm = sqrt(Q[0][1]*Q[0][1] + Q[1][1]*Q[1][1]);
    Q[0][1] /= norm;
    Q[1][1] /= norm;

    // R(2,2)
    R[1][1] = norm;

    // R(1,2)
    R[0][1] = dot;

    // R(2,1) = 0
    R[1][0] = 0;
}

// Function to multiply matrices A = B * C
void matrixMultiply(double A[2][2], double B[2][2], double C[2][2]) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            A[i][j] = 0;
            for (int k = 0; k < 2; k++) {
                A[i][j] += B[i][k] * C[k][j];
            }
        }
    }
}

// Function to check for convergence (off-diagonal near zero)
int hasConverged(double A[2][2]) {
    return (fabs(A[1][0]) < TOLERANCE);
}

// QR Algorithm to find eigenvalues of a 2x2 matrix
void qrAlgorithm(double matrix[2][2]) {
    double Q[2][2], R[2][2], temp[2][2];
    int iter = 0;

    while (!hasConverged(matrix) && iter < MAX_ITER) {
        iter++;
        
        // Perform QR decomposition
        qrDecomposition(matrix, Q, R);

        // Compute A_k+1 = RQ
        matrixMultiply(temp, R, Q);

        // Copy result back to matrix
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                matrix[i][j] = temp[i][j];
    }

    printf("\nConverged Matrix (Quasi Upper Triangular Form):\n");
    printMatrix(matrix);

    // Eigenvalues are diagonal elements in the quasi upper triangular form
    double a = matrix[0][0];
    double b = matrix[1][1];
    double c = matrix[1][0] * matrix[0][1];

    // Calculate the discriminant for quadratic equation
    double discriminant = (a + b) * (a + b) - 4 * (a * b - c);

    if (discriminant < 0) {
        double realPart = (a + b) / 2;
        double imaginaryPart = sqrt(-discriminant) / 2;
        printf("\nEigenvalues (Complex):\n");
        printf("λ1 = %.5lf + %.5lfi\n", realPart, imaginaryPart);
        printf("λ2 = %.5lf - %.5lfi\n", realPart, imaginaryPart);
    } else {
        double lambda1 = (a + b + sqrt(discriminant)) / 2;
        double lambda2 = (a + b - sqrt(discriminant)) / 2;
        printf("\nEigenvalues (Real):\n");
        printf("λ1 = %.5lf\n", lambda1);
        printf("λ2 = %.5lf\n", lambda2);
    }
}

int main() {
    // Companion matrix for the equation 2x^2 + x + 4
    double C[2][2] = {
        {0, -2}, 
        {1, -0.5}
    };

    printf("Initial Companion Matrix:\n");
    printMatrix(C);

    // Apply QR Algorithm
    qrAlgorithm(C);

    return 0;
}

