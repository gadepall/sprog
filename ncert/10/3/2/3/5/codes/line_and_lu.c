#include <stdio.h>
#include <stdlib.h>

void generate_points_line1(double start, double end, double step, double *x, double *y, int *n) {
    *n = 0;
    for (double i = start; i <= end; i += step) {
        x[*n] = i;
        y[*n] = (8 - (4.0 / 3.0) * i) / 2.0;
        (*n)++;
    }
}

void generate_points_line2(double start, double end, double step, double *x, double *y, int *n) {
    *n = 0;
    for (double i = start; i <= end; i += step) {
        x[*n] = i;
        y[*n] = (12 - 2.0 * i) / 3.0;
        (*n)++;
    }
}

void lu_decomposition(double matrix[2][2], double L[2][2], double U[2][2]) {
    L[0][0] = 1.0;
    L[1][0] = matrix[1][0] / matrix[0][0];
    L[1][1] = 1.0;
    L[0][1] = 0.0;

    U[0][0] = matrix[0][0];
    U[0][1] = matrix[0][1];
    U[1][0] = 0.0;
    U[1][1] = matrix[1][1] - L[1][0] * matrix[0][1];
}

