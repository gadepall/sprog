#include <stdio.h>
#include <math.h>

#define MAX_ITER 10000
#define TOL 1e-6

void copy(int n, double A[n][n], double B[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            B[i][j] = A[i][j];
        }
    }
}

void multiply(int n, double A[n][n], double B[n][n], double C[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void gs(int n, double a[n][n], double q[n][n], double r[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            r[i][j] = 0;
        }
    }
    for (int j = 0; j < n; j++) {
        double norm = 0;
        for (int i = 0; i < n; i++) {
            q[i][j] = a[i][j];
        }
        for (int k = 0; k < j; k++) {
            for (int i = 0; i < n; i++) {
                r[k][j] += q[i][j] * q[i][k];
            }
            for (int i = 0; i < n; i++) {
                q[i][j] -= r[k][j] * q[i][k];
            }
        }
        for (int i = 0; i < n; i++) {
            norm += q[i][j] * q[i][j];
        }
        norm = sqrt(norm);
        r[j][j] = norm;
        if (norm > 1e-8) {
            for (int i = 0; i < n; i++) {
                q[i][j] /= norm;
            }
        }
    }
}

void find_eigenvalues(int n, double a[n][n], double eigenvalues[n]) {
    double q[n][n], r[n][n], an[n][n];
    copy(n, a, an);
    for (int iter = 0; iter < MAX_ITER; iter++) {
        gs(n, an, q, r);
        multiply(n, r, q, an);
        int converged = 1;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j && fabs(an[i][j]) > TOL) {
                    converged = 0;
                    break;
                }
            }
            if (!converged) break;
        }
        if (converged) break;
    }
    for (int i = 0; i < n; i++) {
        eigenvalues[i] = an[i][i];
    }
}


