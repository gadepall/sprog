#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define TOL 1e-8
#define MAX_ITER 500

// Function to perform QR decomposition
void qr_decomposition(double M[2][2], double Q[2][2], double R[2][2]) {
    double norm1 = sqrt(M[0][0] * M[0][0] + M[1][0] * M[1][0]);
    if (norm1 < TOL) norm1 = 1e-10;  // Avoid division by zero
    
    // First column of Q
    Q[0][0] = M[0][0] / norm1;
    Q[1][0] = M[1][0] / norm1;

    // Orthogonalize the second column
    double dot_product = Q[0][0] * M[0][1] + Q[1][0] * M[1][1];
    double temp1 = M[0][1] - dot_product * Q[0][0];
    double temp2 = M[1][1] - dot_product * Q[1][0];
    double norm2 = sqrt(temp1 * temp1 + temp2 * temp2);

    if (norm2 < TOL) norm2 = 1e-10;

    Q[0][1] = temp1 / norm2;
    Q[1][1] = temp2 / norm2;

    // Compute R
    for (int i = 0; i < 2; i++) {
        for (int j = i; j < 2; j++) {
            R[i][j] = Q[0][i] * M[0][j] + Q[1][i] * M[1][j];
        }
    }
}

// Function to compute the eigenvalues using QR decomposition
void find_eigenvalues(double a, double b, double c, double *eigen1, double *eigen2) {
    double M[2][2] = {
        {0, -c / a},
        {1, -b / a}
    };

    double Q[2][2], R[2][2];
    int iter = 0;
    bool converged = false;

    while (iter < MAX_ITER) {
        qr_decomposition(M, Q, R);

        // Update M = R * Q
        double new_M[2][2];
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                new_M[i][j] = 0;
                for (int k = 0; k < 2; k++) {
                    new_M[i][j] += R[i][k] * Q[k][j];
                }
            }
        }

        if (fabs(new_M[1][0]) < TOL) {
            converged = true;
            break;
        }

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                M[i][j] = new_M[i][j];
            }
        }

        iter++;
    }
        *eigen1 = M[0][0];
        *eigen2 = M[1][1];
}

int main() {
    double a = 3.0, b = -2.0 * sqrt(6), c = 2.0;
    double eigen1, eigen2;

    find_eigenvalues(a, b, c, &eigen1, &eigen2);

    if (!isnan(eigen1) && !isnan(eigen2)) {
        printf("Eigenvalues (roots) of the quadratic equation: %.6f, %.6f\n", eigen1, eigen2);
    } else {
        printf("Failed to compute the eigenvalues.\n");
    }

    return 0;
}
