#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>       
#include <omp.h>
#include <unistd.h>     
#include <string.h>
#include <stdint.h>
#define EPS 1e-6
#define MAX_ITER 1000

// Function to perform matrix-vector multiplication
void matvec(double A[][100], double x[], double y[], int n) {
    for (int i = 0; i < n; i++) {
        y[i] = 0;
        for (int j = 0; j < n; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
}

// Function to compute the L2 norm of a vector
double norm(double x[], int n) {
    double norm = 0;
    for (int i = 0; i < n; i++) {
        norm += x[i] * x[i];
    }
    return sqrt(norm);
}

// Function to normalize a vector
void normalize(double x[], int n) {
    double normy = norm(x, n);
    for (int i = 0; i < n; i++) {
        x[i] /= normy;
    }
}

// Function to compute the Rayleigh quotient
double rayleigh(double y[], double A[][100], int n) {
    double y0[100] = {0}, res = 0, res1 = 0;
    matvec(A, y, y0, n);
    for (int i = 0; i < n; i++) {
        res += y0[i] * y[i];
        res1 += y[i] * y[i];
    }
    return res / res1;
}

// Power iteration to compute the maximum eigenvalue
double power_iteration(double A[][100], double eigenvector[], int n) {
    double x[100], y[100], eigenvalue = 0, prev_eigenvalue = eigenvalue;
    int iterations = 0;

    for (int i = 0; i < n; i++) x[i] = 1.0; // Initial guess

    while (iterations < MAX_ITER) {
        matvec(A, x, y, n);
        normalize(y, n);
        eigenvalue = rayleigh(y, A, n);

        if (fabs(eigenvalue - prev_eigenvalue) < EPS) break;

        for (int i = 0; i < n; i++) x[i] = y[i];
        prev_eigenvalue = eigenvalue;
        iterations++;
    }

    for (int i = 0; i < n; i++) eigenvector[i] = x[i];
    return eigenvalue;
}

// Function to compute the companion matrix of a polynomial
void companion_matrix(double coeffs[], double C[][100], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == n - 1) {
                C[i][j] = -coeffs[j]; // Last row: negative coefficients of the polynomial
            } else if (j == i + 1) {
                C[i][j] = 1; // Subdiagonal: 1's
            } else {
                C[i][j] = 0; // Everything else: 0's
            }
        }
    }
}
void reverseArray(double arr[], int n) {
    int start = 0;
    int end = n - 1;
    while (start < end) {
        double temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}
// Function to perform synthetic division
void synthetic_division(double coeffs[], int n, double root, double new_coeffs[]) {
    new_coeffs[0] = coeffs[n];
    for (int i = 1; i <= n; i++) {
        new_coeffs[i] = coeffs[n-i] + root * new_coeffs[i - 1];
    }
    reverseArray(new_coeffs, n);
}
// Main function
int main() {
    srand(time(NULL));
    int degree;
    printf("Enter the degree of the polynomial (n): ");
    scanf("%d", &degree);

    double coeffs[100], new_coeffs[100], C[100][100], eigenvector[100];
    printf("Enter the coefficients of the polynomial (from c_0 to c_%d):\n", degree);
    for (int i = 0; i <= degree; i++) {
        scanf("%lf", &coeffs[i]);
    }
    clock_t start, end; 
    double start_time = omp_get_wtime();
    start = clock();
    while (degree > 1) {
        companion_matrix(coeffs, C, degree);
        double root = power_iteration(C, eigenvector, degree);

        printf("Found Root: %.6f\n", root);

        synthetic_division(coeffs, degree, root, new_coeffs);
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
    double time = (double)(end - start)/CLOCKS_PER_SEC;
    printf("roots() took %f seconds\n", time);
    return 0;
}

