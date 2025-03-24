#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h> // Include OpenMP for parallelization

#define EPS 1e-6
#define MAX_ITER 1000
#define MAX_SIZE 100 // Maximum degree of the polynomial

// Function to perform matrix-vector multiplication (parallelized with OpenMP)
void matvec(double mat[MAX_SIZE][MAX_SIZE], double *vec, double *res, int n) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        res[i] = 0;
        for (int j = 0; j < n; j++) {
            res[i] += mat[i][j] * vec[j];
        }
    }
}

// Function to compute the L2 norm of a vector
double vec_norm(double *vec, int n) {
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += vec[i] * vec[i];
    }
    return sqrt(sum);
}

// Function to normalize a vector
void normalize(double *vec, int n) {
    double norm = vec_norm(vec, n);
    if (norm < EPS) {
        // Handle the case where the vector is zero
        for (int i = 0; i < n; i++) vec[i] = 1.0 / sqrt(n);
    } else {
        for (int i = 0; i < n; i++) vec[i] /= norm;
    }
}

// Function to perform Gaussian elimination with partial pivoting
void GaussianElimination(double mat[MAX_SIZE][MAX_SIZE], double *x, int n) {
    double b[MAX_SIZE][MAX_SIZE + 1];

    // Copy the matrix and vector into b
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            b[i][j] = mat[i][j];
        }
        b[i][n] = x[i];
    }

    // Forward elimination with partial pivoting
    for (int i = 0; i < n; i++) {
        // Find the pivot row
        int max_row = i;
        for (int k = i + 1; k < n; k++) {
            if (fabs(b[k][i]) > fabs(b[max_row][i])) {
                max_row = k;
            }
        }

        // Swap rows
        if (max_row != i) {
            for (int j = 0; j <= n; j++) {
                double temp = b[i][j];
                b[i][j] = b[max_row][j];
                b[max_row][j] = temp;
            }
        }

        // Check for singular matrix
        if (fabs(b[i][i]) < EPS) {
            printf("Matrix is singular or nearly singular.\n");
            exit(1);
        }

        // Normalize the pivot row
        double pivot = b[i][i];
        for (int j = i; j <= n; j++) {
            b[i][j] /= pivot;
        }

        // Eliminate the current column in the rows below
        for (int k = i + 1; k < n; k++) {
            double factor = b[k][i];
            for (int j = i; j <= n; j++) {
                b[k][j] -= factor * b[i][j];
            }
        }
    }

    // Backward substitution
    for (int i = n - 1; i >= 0; i--) {
        x[i] = b[i][n];
        for (int j = i + 1; j < n; j++) {
            x[i] -= b[i][j] * x[j];
        }
    }
}

// Function to compute the Rayleigh quotient
double rayleigh(double *vec, double mat[MAX_SIZE][MAX_SIZE], int n) {
    double temp[MAX_SIZE];
    matvec(mat, vec, temp, n);
    double num = 0, den = 0;
    for (int i = 0; i < n; i++) {
        num += temp[i] * vec[i];
        den += vec[i] * vec[i];
    }
    return num / den;
}

// Inverse power iteration to compute the minimum eigenvalue
double inv_power_iter(double mat[MAX_SIZE][MAX_SIZE], double *eigvec, int n) {
    double x[MAX_SIZE];
    double eigval = 0, prev_eigval = eigval;
    int iter = 0;

    // Initial guess for the eigenvector
    for (int i = 0; i < n; i++) x[i] = 1.0;

    while (iter < MAX_ITER) {
        GaussianElimination(mat, x, n);
        normalize(x, n);
        eigval = rayleigh(x, mat, n);

        if (fabs(eigval - prev_eigval) < EPS) break;

        prev_eigval = eigval;
        iter++;
    }

    for (int i = 0; i < n; i++) eigvec[i] = x[i];
    return eigval;
}

// Function to compute the companion matrix of a polynomial
void companion(double *coeffs, double mat[MAX_SIZE][MAX_SIZE], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == n - 1) {
                mat[i][j] = -coeffs[j];
            } else if (j == i + 1) {
                mat[i][j] = 1;
            } else {
                mat[i][j] = 0;
            }
        }
    }
}

// Function to perform synthetic division
void reverse(double *arr, int n) {
    int start = 0, end = n - 1;
    while (start < end) {
        double temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

// Function to perform synthetic division
void synthetic_div(double *coeffs, int n, double root, double *new_coeffs) {
    new_coeffs[0] = coeffs[n];
    for (int i = 1; i <= n; i++) {
        new_coeffs[i] = coeffs[n - i] + root * new_coeffs[i - 1];
    }
    reverse(new_coeffs, n);
}

// Main function
int main() {
    srand(time(NULL));
    int degree;
    printf("Enter the degree of the polynomial (n): ");
    scanf("%d", &degree);

    if (degree <= 0 || degree > MAX_SIZE) {
        printf("Degree must be a positive integer and less than or equal to %d.\n", MAX_SIZE);
        return 1;
    }

    double coeffs[MAX_SIZE + 1];
    double new_coeffs[MAX_SIZE + 1];
    double mat[MAX_SIZE][MAX_SIZE];
    double eigvec[MAX_SIZE];

    printf("Enter the coefficients of the polynomial (from c_0 to c_%d):\n", degree);
    for (int i = 0; i <= degree; i++) {
        scanf("%lf", &coeffs[i]);
    }

    int n = degree;
    clock_t start, end;
    double start_time = omp_get_wtime();
    start = clock();

    while (degree > 1) {
        printf("Debug: Computing companion matrix for degree %d\n", degree);
        companion(coeffs, mat, degree);

        printf("Debug: Starting inverse power iteration\n");
        double root = inv_power_iter(mat, eigvec, degree);

        printf("Found Root: %.6f\n", root);

        printf("Debug: Performing synthetic division\n");
        synthetic_div(coeffs, degree + 1, root, new_coeffs);
        degree--;

        printf("Reduced Polynomial Coefficients:\n");
        for (int i = 0; i <= degree; i++) {
            coeffs[i] = new_coeffs[i];
            printf("%.6f ", coeffs[i]);
        }
        printf("\n");
    }

    printf("Final Root: %.6f\n", -coeffs[0] / coeffs[1]);
    end = clock();
    double end_time = omp_get_wtime();
    double time_taken = end_time - start_time;
    double cpu_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("roots() took %f seconds (wall clock time: %f seconds)\n", cpu_time, time_taken);

    return 0;
}
