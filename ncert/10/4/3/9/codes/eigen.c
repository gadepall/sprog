#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 2  // Define the size of the matrix as a constant

typedef struct {
    double Re;
    double Im;
} Complex;

Complex CreateCom(double Re, double Im) {
    Complex c;
    c.Re = Re;
    c.Im = Im;
    return c;
}

Complex PlusCom(Complex a, Complex b) {
    return CreateCom(a.Re + b.Re, a.Im + b.Im);
}

Complex MinusCom(Complex a, Complex b) {
    return CreateCom(a.Re - b.Re, a.Im - b.Im);
}

Complex MultCom(Complex a, Complex b) {
    return CreateCom(a.Re * b.Re - a.Im * b.Im, a.Re * b.Im + a.Im * b.Re);
}

Complex DivideCom(Complex a, Complex b) {
    double denom = b.Re * b.Re + b.Im * b.Im;
    return CreateCom((a.Re * b.Re + a.Im * b.Im) / denom,
                     (a.Im * b.Re - a.Re * b.Im) / denom);
}

Complex ConjugateCom(Complex c) {
    return CreateCom(c.Re, -c.Im);
}

double MagCom(Complex c) {
    return sqrt(c.Re * c.Re + c.Im * c.Im);
}

double MagSquareCom(Complex c) {
    return c.Re * c.Re + c.Im * c.Im;
}

double NormComColumn(Complex matrix[N][N], int col) {
    double normSq = 0.0;
    for (int i = 0; i < N; i++) {
        normSq += MagSquareCom(matrix[i][col]);
    }
    return sqrt(normSq);
}

void copyMatrixComplex(Complex A[N][N], Complex B[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            B[i][j] = A[i][j];
        }
    }
}

void multiplyMatricesComplex(Complex A[N][N], Complex B[N][N], Complex result[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = CreateCom(0.0, 0.0);
            for (int k = 0; k < N; k++) {
                result[i][j] = PlusCom(result[i][j], MultCom(A[i][k], B[k][j]));
            }
        }
    }
}

void QRDecom(Complex A[N][N], Complex Q[N][N], Complex R[N][N]) {
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            Q[i][k] = A[i][k];
        }

        for (int j = 0; j < k; j++) {
            R[j][k] = CreateCom(0.0, 0.0);
            for (int i = 0; i < N; i++) {
                R[j][k] = PlusCom(R[j][k], MultCom(ConjugateCom(Q[i][j]), Q[i][k]));
            }
            for (int i = 0; i < N; i++) {
                Q[i][k] = MinusCom(Q[i][k], MultCom(R[j][k], Q[i][j]));
            }
        }

        double norm = NormComColumn(Q, k);
        R[k][k] = CreateCom(norm, 0.0);
        for (int i = 0; i < N; i++) {
            Q[i][k] = DivideCom(Q[i][k], R[k][k]);
        }
    }
}

void QRIterations(Complex A[N][N], int maxIterations, double tolerance, double eigenvalues[2][2]) {
    Complex Q[N][N], R[N][N], temp[N][N];

    for (int it = 0; it < maxIterations; it++) {
        QRDecom(A, Q, R);
        multiplyMatricesComplex(R, Q, temp);
        copyMatrixComplex(temp, A);

        int converged = 1;
        for (int i = 0; i < N - 1; i++) {
            if (MagCom(A[i + 1][i]) > tolerance) {
                converged = 0;
                break;
            }
        }

        if (converged) break;
    }

    // Store eigenvalues in the result matrix
    for (int i = 0; i < N; i++) {
        eigenvalues[i][0] = A[i][i].Re;
        eigenvalues[i][1] = A[i][i].Im;
    }
}

