#include <stdio.h>

void luDecomposition(float A[2][2], float L[2][2], float U[2][2]) {
    L[0][0] = 1; L[0][1] = 0;
    L[1][0] = A[1][0] / A[0][0]; L[1][1] = 1;

    U[0][0] = A[0][0]; U[0][1] = A[0][1];
    U[1][0] = 0; U[1][1] = A[1][1] - L[1][0] * A[0][1];
}

void forwardSubstitution(float L[2][2], float b[2], float z[2]) {
    z[0] = b[0];
    z[1] = b[1] - L[1][0] * z[0];
}

void backwardSubstitution(float U[2][2], float z[2], float *x, float *y) {
    *y = z[1] / U[1][1];
    *x = (z[0] - U[0][1] * (*y)) / U[0][0];
}

void findIntersection(float a1, float b1, float c1, float a2, float b2, float c2, float* x, float* y) {
    float A[2][2] = {{a1, b1}, {a2, b2}};
    float L[2][2], U[2][2];
    float b[2] = {c1, c2}, z[2];

    // LU decomposition
    luDecomposition(A, L, U);

    // Forward substitution
    forwardSubstitution(L, b, z);

    // Backward substitution (correctly updating x and y)
    backwardSubstitution(U, z, x, y);

    // Print the intersection point
    printf("Intersection Point: t = %f, y = %f\n", *x, *y);
}

