#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#define size_limit 100 

void Output_Matrix(double A[size_limit][size_limit], int n) {
for (int i = 0; i < n; i++) {
for (int j = 0; j < n; j++) {
printf("%10.4f ", A[i][j]);
}
printf("\n");
}
}

void Matrix_Multiplication(double A[size_limit][size_limit], double B[size_limit][size_limit], double C[size_limit][size_limit], int n) {
for (int i = 0; i < n; i++) {
for (int j = 0; j < n; j++) {
C[i][j] = 0.0;
for (int k = 0; k < n; k++) {
C[i][j] += A[i][k] * B[k][j];
}
}
}
}

void QR_Decomposition(double A[size_limit][size_limit], double Q[size_limit][size_limit], double R[size_limit][size_limit], int n) {
for (int i = 0; i < n; i++) {
for (int j = 0; j < n; j++) {
Q[i][j] = A[i][j];
R[i][j] = 0.0;
}
}
for (int k = 0; k < n; k++) {
double norm = 0.0;
for (int i = 0; i < n; i++) {  
norm += Q[i][k] * Q[i][k];
}
norm = sqrt(norm);
R[k][k] = norm;
if (fabs(norm) < 1e-10) {
printf("QR decomposition can not proceed beacause of zero norm.\n");
exit(1);
}
for (int i = 0; i < n; i++) {
Q[i][k] /= norm;
}
for (int j = k + 1; j < n; j++) {
double dot = 0.0;
for (int i = 0; i < n; i++) {
dot += Q[i][k] * Q[i][j];
}
R[k][j] = dot;
for (int i = 0; i < n; i++) {
Q[i][j] -= dot * Q[i][k];
}
}
}
}

void Eigenvalues(double A[size_limit][size_limit], int n, double complex eigenvalues[size_limit]) {
double Q[size_limit][size_limit], R[size_limit][size_limit], temp[size_limit][size_limit];
for (int iter = 0; iter < 1000; iter++) { 
QR_Decomposition(A, Q, R, n);
Matrix_Multiplication(R, Q, temp, n);
for (int i = 0; i < n; i++) {
for (int j = 0; j < n; j++) {
A[i][j] = temp[i][j];
}
}
}
for (int i = 0; i < n; i++) {
if (i < n - 1 && fabs(A[i + 1][i]) > 1e-6) {
double a = A[i][i], b = A[i][i + 1];
double c = A[i + 1][i], d = A[i + 1][i + 1];
double trace = a + d, det = a * d - b * c;
double discriminant = trace * trace - 4 * det;
if (discriminant >= 0) {
eigenvalues[i] = (trace + sqrt(discriminant)) / 2;
eigenvalues[i + 1] = (trace - sqrt(discriminant)) / 2;
} 
else {
eigenvalues[i] = (trace / 2) + I * (sqrt(-discriminant) / 2);
eigenvalues[i + 1] = (trace / 2) - I * (sqrt(-discriminant) / 2);
}
i++;
} 
else {
eigenvalues[i] = A[i][i];
}
}
}
void Input_Matrix(double A[size_limit][size_limit], int *n) {
printf("Enter the size of the square matrix (n x n): ");
scanf("%d", n);
if (*n <= 0 || *n > size_limit) {
printf("Invalid matrix size! Please enter a size between 1 and %d.\n", size_limit);
        exit(1);
    }

    printf("Enter the elements of the matrix row by row:\n");
    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n; j++) {
            scanf("%lf", &A[i][j]);
        }
    }
}

int main() {
    double A[size_limit][size_limit];
    double complex eigenvalues[size_limit];
    int n;

    Input_Matrix(A, &n);
    printf("Input Matrix:\n");
    Output_Matrix(A, n);
    Eigenvalues(A, n, eigenvalues);
    printf("Eigenvalues(if a+ib is the eigenvalue then the answer is (a,b) ):\n");
    for (int i = 0; i < n; i++) {
        printf("(%f, %f)\n", creal(eigenvalues[i]), cimag(eigenvalues[i]));
    }
    return 0;
}

