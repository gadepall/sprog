#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <immintrin.h>
#include <complex.h>
#include "matcals.h"

double complex** grMat(double** A, int m, int i, int j){
    double complex** G = eye(m);

    double complex r = 1/csqrt(cabs(A[i][j]*cabs(A[i][j]) + cabs(A[i-1][j])*cabs(A[i][j-1])));
    double complex s = r*A[i][j];
    double complex c = r*A[i-1][j];

    G[i][i] = c;
    G[i-1][i-1] = conj(c);
    G[i][i-1] = -conj(s);
    G[i-1][i] = s;

    return G;
}

#pragma omp parallel num_threads(4)
double complex*** QRdec(double complex** A, int m){
    double complex** Q = eye(m);
    double complex** R = A;
    double complex*** QR; QR = malloc(2 * sizeof(double complex**));
    double complex** G;
    #pragma omp for
    for(int i=0;i<m-1;i++){
        for(int j=m-1;j>i;j--){
            if(cabs(R[i][j])<1.0e-10) continue;
            G = grMat(R, m, i, j);
            Q = matMul(Q, transpose(G, m, m), m, m, m);
            R = matMul(G, R, m, m, m);
        }
    }

    QR[0] = Q; QR[1] = R;

    return QR;
}

double complex** QReig(double complex** A, int m, int inter){
    double complex** Ak = A;
    double complex*** QR;
    for(int i=0;i<inter;i++){
        QR = QRdec(A, m);
        Ak = matMul(QR[1], QR[0], m, m, m);
    }

    return Ak;
}


double complex* eigs(double complex** A, int m){
    double complex* eigs = malloc(m * sizeof(double complex));
    double complex* eigs2;
    double complex** Asl = malloc(2 * sizeof(double complex*));
    for(int i=0;i<2;i++) Asl[i] = malloc(2 * sizeof(double complex));

    int i=0;
    while(i<m){
        if(i<m-1 && cabs(A[i][i+1]+(A[i+1][i]))<10.0e-10) eigs[i] = A[i][i];
        else{
            Asl[0][0] = A[i][i]; Asl[0][1] = A[i][i+1];
            Asl[1][0] = A[i+1][i]; Asl[1][1] = A[i+1][i+1];

            eigs2 = eigs2x(Asl);
            eigs[i] = eigs2[0]; eigs[i+1] = eigs2[1];
            i++;
        }
        i++;
    }

    return eigs;
}
