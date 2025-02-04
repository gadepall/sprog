#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define TOL 1e-10
#define MAX_ITER 100

// Function to perform QR decomposition
void qr_decomposition(double M[2][2], double Q[2][2], double R[2][2]) {
    double norm = sqrt(M[0][0] * M[0][0] + M[1][0] * M[1][0]);

    // Compute Q
    Q[0][0] = M[0][0] / norm;
    Q[1][0] = M[1][0] / norm;
    Q[0][1] = -M[1][0] / norm;
    Q[1][1] = M[0][0] / norm;

    // Compute R
    R[0][0] = Q[0][0] * M[0][0] + Q[1][0] * M[1][0];
    R[0][1] = Q[0][0] * M[0][1] + Q[1][0] * M[1][1];
    R[1][0] = 0;
    R[1][1] = Q[0][1] * M[0][1] + Q[1][1] * M[1][1];
}

// Function to compute the eigenvalues using QR decomposition
void find_eigenvalues(double a, double b, double c, double *eigen1, double *eigen2) {
    double M[2][2] = {
        {0, -c / a},  // M[0][1] = -c/a
        {1, -b / a}   // M[1][1] = -b/a
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

        // Check for convergence
        if (fabs(new_M[1][0]) < TOL) {
            converged = true;
            break;
        }

        // Update M
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                M[i][j] = new_M[i][j];
            }
        }

        iter++;
    }

    if (converged) {
        *eigen1 = M[0][0];
        *eigen2 = M[1][1];
    } else {
        printf("QR decomposition did not converge within the iteration limit.\n");
        *eigen1 = NAN;
        *eigen2 = NAN;
    }
}

int main() {
    double a = 1, b = -3, c = -4;
    double eigen1, eigen2;

    find_eigenvalues(a, b, c, &eigen1, &eigen2);

    if (!isnan(eigen1) && !isnan(eigen2)) {
        printf("Eigenvalues (roots) of the quadratic equation are: %f, %f\n", eigen1, eigen2);
    } else {
        printf("Failed to find eigenvalues (roots).\n");
    }

    return 0;
}

