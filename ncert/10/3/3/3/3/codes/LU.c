#include <stdio.h>

void solveLU(double A[2][2], double B[2], double *x, double *y) {
    // Decompose A into L and U
    double L[2][2] = {0}, U[2][2] = {0};
    // Initialize L and U
    L[0][0] = 1;
    L[1][1] = 1;
    U[0][0] = A[0][0];
    U[0][1] = A[0][1];
    L[1][0] = A[1][0] / U[0][0];
    U[1][1] = A[1][1] - (L[1][0] * U[0][1]);
    //Solve L*Y=B (Forward substitution)
    double Y[2];
    Y[0] = B[0];
    Y[1] = B[1] - (L[1][0] * Y[0]);
    //Solve U*X=Y (Backward substitution)
    *y = Y[1] / U[1][1];
    *x = (Y[0] - (U[0][1] * (*y))) / U[0][0];
}
int main() {
    //Matrix A
    double A[2][2] = {
        {7, 6},
        {3, 5}
    };
    //Constant matrix B 
    double B[2] = {3800, 1750};
    double x, y;
    //Solve using LU factorization
    solveLU(A, B, &x, &y);

    printf("Cost of one bat (x): %.2lf\n", x);
    printf("Cost of one ball (y): %.2lf\n", y);

    return 0;
}

