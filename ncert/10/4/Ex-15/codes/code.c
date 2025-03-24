#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define EPSILON 0.0001  // Convergence criterion
#define MAX_ITER 100    // Maximum number of iterations
#define TOL 1e-10

// Function representing the equation f(x) = x^2 + 48x - 324
double f(double x) {
    return x * x + 48 * x - 324;
}

// Derivative of the function f'(x) = 2x + 48
double df(double x) {
    return 2 * x + 48;
}

// Newton-Raphson method
void newtonRaphson(double initial_guess) {
    double x = initial_guess; // Initial guess
    int iter = 0;

    while (fabs(f(x)) > EPSILON && iter < MAX_ITER) {
        if (df(x) == 0) { // Avoid division by zero
            printf("Derivative is zero. Newton-Raphson method fails.\n");
            return;
        }

        x = x - f(x) / df(x); // Newton-Raphson formula
        iter++;
    }

    if (fabs(f(x)) <= EPSILON) {
        printf("The speed of the stream is: %.4f km/h\n", x);
    } else {
        printf("Solution did not converge.\n");
    }
}

int main() {
    double initial_guess = 5.0; // Initial guess for Newton-Raphson
    newtonRaphson(initial_guess);
    return 0;
}

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
