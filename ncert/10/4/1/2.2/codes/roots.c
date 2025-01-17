#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <complex.h>

// Function definitions for the quadratic equation
double f(double x) {
    return x * x - x - 306;  // Original quadratic equation
}

double f_derivative(double x) {
    return 2 * x - 1;  // Derivative of f(x)
}

// Fixed-point iteration functions for both roots
double g_positive(double x) {
    return sqrt(x + 306);  // For the positive root
}

double g_negative(double x) {
    return -sqrt(x + 306);  // For the negative root
}

// Random number generator for initial guesses
double random_double_in_range(double min, double max) {
    if (min > max) {
        fprintf(stderr, "Invalid range: min should be less than or equal to max.\n");
        return -1;
    }
    double scale = rand() / (double)RAND_MAX;  // Random value in [0, 1]
    return min + scale * (max - min);
}

// Fixed-point iteration function
int run_fixed_point_iterations(double (*g)(double), double init_guess, int num_iter, double tolerance) {
    for (int i = 0; i < num_iter; i++) {
        double next_guess = g(init_guess);

        printf("Iteration %d: x = %lf, f(x) = %lf\n", i + 1, next_guess, f(next_guess));

        // Check for convergence
        if (fabs(next_guess - init_guess) < tolerance) {
            printf("Converged to solution: x = %lf\n", next_guess);
            return 0;
        }

        init_guess = next_guess;
    }

    printf("Failed to converge within %d iterations.\n", num_iter);
    return -1;
}

// Newton-Raphson iteration function
int run_newton_iterations(double init_guess, int num_iter, double tolerance) {
    for (int i = 0; i < num_iter; i++) {
        if (fabs(f_derivative(init_guess)) < tolerance) {
            printf("Failure: Derivative too small\n");
            return -1;
        }

        double next_guess = init_guess - f(init_guess) / f_derivative(init_guess);

        printf("Iteration %d: x = %lf, f(x) = %lf\n", i + 1, next_guess, f(next_guess));

        // Check for convergence
        if (fabs(next_guess - init_guess) < tolerance) {
            printf("Converged to solution: x = %lf\n", next_guess);
            return 0;
        }

        init_guess = next_guess;
    }

    printf("Failed to converge within %d iterations.\n", num_iter);
    return -1;
}

// Matrix multiplication for complex matrices
void matmul(int n, double complex A[n][n], double complex B[n][n], double complex C[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0 + 0 * I;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Gram-Schmidt QR decomposition for complex matrices
void gramschmidt(int n, double complex A[n][n], double complex Q[n][n], double complex R[n][n]) {
    for (int j = 0; j < n; j++) {
        double complex v[n];
        for (int i = 0; i < n; i++) {
            v[i] = A[i][j];
        }

        for (int k = 0; k < j; k++) {
            R[k][j] = 0 + 0 * I;
            for (int i = 0; i < n; i++) {
                R[k][j] += conj(Q[i][k]) * v[i];
            }
            for (int i = 0; i < n; i++) {
                v[i] -= R[k][j] * Q[i][k];
            }
        }

        double norm = 0;
        for (int i = 0; i < n; i++) {
            norm += creal(v[i]) * creal(v[i]) + cimag(v[i]) * cimag(v[i]);
        }

        if (fabs(norm) < 1e-10) {
            R[j][j] = 0 + 0 * I;
            for (int i = 0; i < n; i++) {
                Q[i][j] = 0 + 0 * I;
            }
        } else {
            R[j][j] = csqrt(norm);
            for (int i = 0; i < n; i++) {
                Q[i][j] = v[i] / R[j][j];
            }
        }
    }
}

// Compute eigenvalues from a Hessenberg matrix
void compute_eigenvalues(int n, double complex matrix[n][n], double complex eigenvalues[n]) {
    int k = 0;
    while (k < n) {
        if (k < n - 1 && cabs(matrix[k + 1][k]) > 1e-10) {
            double complex a11 = matrix[k][k];
            double complex a12 = matrix[k][k + 1];
            double complex a21 = matrix[k + 1][k];
            double complex a22 = matrix[k + 1][k + 1];

            double complex b = -(a11 + a22);
            double complex c = a11 * a22 - a21 * a12;
            double complex D = b * b - 4.0 * c;

            eigenvalues[k] = (-b + csqrt(D)) / 2.0;
            eigenvalues[k + 1] = (-b - csqrt(D)) / 2.0;
            matrix[k + 1][k] = 0 + 0 * I;
            k += 2;
        } else {
            eigenvalues[k] = matrix[k][k];
            k++;
        }
    }
}

// QR algorithm with shifts for complex matrices
void qr_algorithm_with_shift(int n, double complex matrix[n][n], double complex eigenvalues[n]) {
    double complex Q[n][n], R[n][n];
    for (int iter = 0; iter < 1000; iter++) {
        double complex shift;
        if (n > 1) {
            double complex d = (matrix[n - 2][n - 2] - matrix[n - 1][n - 1]) / 2.0;
            double complex sign = (creal(d) >= 0) ? 1.0 : -1.0;
            shift = matrix[n - 1][n - 1] - sign * csqrt(d * d + matrix[n - 1][n - 2] * matrix[n - 2][n - 1]);
        } else {
            shift = matrix[n - 1][n - 1];
        }

        for (int i = 0; i < n; i++) {
            matrix[i][i] -= shift;
        }

        gramschmidt(n, matrix, Q, R);
        matmul(n, R, Q, matrix);

        for (int i = 0; i < n; i++) {
            matrix[i][i] += shift;
        }

        int converged = 1;
        for (int i = 0; i < n - 1; i++) {
            if (cabs(matrix[i + 1][i]) > 1e-10) {
                converged = 0;
                break;
            }
        }
        if (converged) {
            break;
        }
    }
    compute_eigenvalues(n, matrix, eigenvalues);
}

// Create companion matrix for a polynomial
void eigvalmat(int order, double complex coefficients[], double complex matrix[order][order]) {
    // Initialize all elements of the matrix to 0
    for (int i = 0; i < order; i++) {
        for (int j = 0; j < order; j++) {
            matrix[i][j] = 0 + 0 * I;
        }
    }

    // Set 1s on the superdiagonal
    for (int i = 0; i < order - 1; i++) {
        matrix[i][i + 1] = 1 + 0 * I;
    }

    // Set the last row with -coefficients
    for (int j = 0; j < order; j++) {
        matrix[order - 1][j] = -coefficients[j];
    }
}

int main() {
    srand(time(NULL));  // Seed the random number generator

    // Parameters for iterations
    int max_iterations = 1000;
    double tolerance = 1e-6;

    // Fixed-point iteration for both roots
    printf("Fixed-Point Iteration for Positive Root:\n");
    double init_guess_positive = random_double_in_range(0, 50);
    run_fixed_point_iterations(g_positive, init_guess_positive, max_iterations, tolerance);

    printf("\nFixed-Point Iteration for Negative Root:\n");
    double init_guess_negative = random_double_in_range(-306, 0);  // Ensure domain validity
    run_fixed_point_iterations(g_negative, init_guess_negative, max_iterations, tolerance);

    // Newton-Raphson iteration for both roots
    printf("\nNewton-Raphson Iteration for Positive Root:\n");
    double newton_guess_positive = random_double_in_range(0, 50);
    run_newton_iterations(newton_guess_positive, max_iterations, tolerance);

    printf("\nNewton-Raphson Iteration for Negative Root:\n");
    double newton_guess_negative = random_double_in_range(-50, 0);
    run_newton_iterations(newton_guess_negative, max_iterations, tolerance);

    // Eigenvalue computation
    int order = 2;
    double complex matrix[order][order];
    double complex coefficients[] = {-306 + 0 * I, -1 + 0 * I};
    eigvalmat(order, coefficients, matrix);
    double complex eigenvalues[2];
    qr_algorithm_with_shift(order, matrix, eigenvalues);

    // Print eigenvalues (roots)
    printf("\nEigenvalues (Roots of the equation):\n");
    for (int i = 0; i < order; i++) {
        printf("Eigenvalue %d: %lf + %lfi\n", i + 1, creal(eigenvalues[i]), cimag(eigenvalues[i]));
    }

    return 0;
}

