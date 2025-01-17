#include <stdio.h>
#include <stdlib.h>
#include <lapacke.h>
#include <time.h>

// Function to compute the companion matrix of a polynomial
void companion_matrix(double coeffs[], double C[], int degree) {
    for (int i = 0; i < degree; i++) {
        for (int j = 0; j < degree; j++) {
            if (i == degree - 1) {
                C[i * degree + j] = -coeffs[j] / coeffs[degree]; // Last row
            } else if (j == i + 1) {
                C[i * degree + j] = 1.0; // Subdiagonal
            } else {
                C[i * degree + j] = 0.0; // Other elements
            }
        }
    }
}

int main() {
    int degree;
    printf("Enter the degree of the polynomial (n): ");
    scanf("%d", &degree);

    double coeffs[100];
    printf("Enter the coefficients of the polynomial (from c_0 to c_%d):\n", degree);
    for (int i = 0; i <= degree; i++) {
        scanf("%lf", &coeffs[i]);
    }

    // Allocate memory for the companion matrix
    double *C = (double *)malloc(degree * degree * sizeof(double));

    // Create the companion matrix
    companion_matrix(coeffs, C, degree);

    // Allocate arrays for eigenvalues and workspace
    double *wr = (double *)malloc(degree * sizeof(double)); // Real part of eigenvalues
    double *wi = (double *)malloc(degree * sizeof(double)); // Imaginary part of eigenvalues
    double *vl = NULL; // Left eigenvectors (not needed)
    double *vr = NULL; // Right eigenvectors (not needed)
    int lwork = 4 * degree; // Recommended size for work array
    double *work = (double *)malloc(lwork * sizeof(double));

    // Start timing
    clock_t start_time = clock();

    // Call LAPACK's dgeev to compute eigenvalues
    int info = LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'N', degree, C, degree, wr, wi, vl, degree, vr, degree);

    // End timing
    clock_t end_time = clock();

    if (info == 0) {
        printf("Eigenvalues of the companion matrix:\n");
        for (int i = 0; i < degree; i++) {
            if (wi[i] == 0) {
                printf("%.6f\n", wr[i]); // Real eigenvalue
            } else {
                printf("%.6f + %.6fi\n", wr[i], wi[i]); // Complex eigenvalue
            }
        }
    } else {
        printf("LAPACK dgeev failed with info = %d\n", info);
    }

    // Calculate and display the elapsed time
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken for eigenvalue computation: %.6f seconds\n", elapsed_time);

    // Free allocated memory
    free(C);
    free(wr);
    free(wi);
    free(work);

    return 0;
}

