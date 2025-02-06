#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define EPS 1e-6
#define MAX_ITER 1000

// Function to perform matrix-vector multiplication
void matvec(double mat[][100], double vec[], double res[], int n) {
    for (int i = 0; i < n; i++) {
        res[i] = 0;
        for (int j = 0; j < n; j++) {
            res[i] += mat[i][j] * vec[j];
        }
    }
}

// Function to compute the L2 norm of a vector
double vec_norm(double vec[], int n) {
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += vec[i] * vec[i];
    }
    return sqrt(sum);
}

// Function to normalize a vector
void normalize(double vec[], int n) {
    double norm = vec_norm(vec, n);
    for (int i = 0; i < n; i++) {
        vec[i] /= norm;
    }
}

// Function to compute the Rayleigh quotient
double rayleigh(double vec[], double mat[][100], int n) {
    double temp[100] = {0};
    double num = 0, den = 0;

    matvec(mat, vec, temp, n);
    for (int i = 0; i < n; i++) {
        num += temp[i] * vec[i];
        den += vec[i] * vec[i];
    }
    return num / den;
}

// Power iteration to compute the maximum eigenvalue
double power_iter(double mat[][100], double eigvec[], int n) {
    double x[100], y[100];
    double eigval = 0, prev_eigval = eigval;
    int iter = 0;

    for (int i = 0; i < n; i++) x[i] = 1.0; // Initial guess

    while (iter < MAX_ITER) {
        matvec(mat, x, y, n);
        normalize(y, n);
        eigval = rayleigh(y, mat, n);

        if (fabs(eigval - prev_eigval) < EPS) break;

        for (int i = 0; i < n; i++) x[i] = y[i];
        prev_eigval = eigval;
        iter++;
    }

    for (int i = 0; i < n; i++) eigvec[i] = x[i];
    return eigval;
}

// Function to compute the companion matrix of a polynomial
void companion(double coeffs[], double mat[][100], int n) {
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

void reverse(double arr[], int n) {
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
void synthetic_div(double coeffs[], int n, double root, double new_coeffs[]) {
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

    double coeffs[100], new_coeffs[100], mat[100][100], eigvec[100];
    printf("Enter the coefficients of the polynomial (from c_0 to c_%d):\n", degree);
    for (int i = 0; i <= degree; i++) {
        scanf("%lf", &coeffs[i]);
    }

    clock_t start, end; 
    double start_time = omp_get_wtime();
    start = clock();

    while (degree > 1) {
        companion(coeffs, mat, degree);
        double root = power_iter(mat, eigvec, degree);

        printf("Found Root: %.6f\n", root);

        synthetic_div(coeffs, degree, root, new_coeffs);
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
    printf("roots() took %f seconds\n", cpu_time);
    return 0;
}

