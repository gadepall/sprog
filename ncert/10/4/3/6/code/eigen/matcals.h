void printMat(double complex** A, int m, int n){
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++) printf("%.2lf + j%.2lf ",creal(A[i][j]), cimag(A[i][j]));
        printf("\n");
    }

}

#pragma omp parallel num_threads(4)
double complex** createMat(int m, int n){
    double complex** A;
    A = malloc(m * sizeof(double complex*));
    #pragma omp for
    for(int i=0;i<m;i++){
        A[i] = malloc(n * sizeof(double complex));
    }
    // printf("allocated memory\n");
    return A;
}

void copyMat(double complex** A, double complex** B, int m, int n){
    for(int i=0;i<m;i++){
        memcpy(B[i], A[i], n * sizeof(double complex));
    }
    // memcpy(B, A, m * sizeof(double complex*));
}

void freeMat(double complex** A, int m){
    for(int i=0;i<m;i++){
        free(A[i]);
    }
    free(A);
    // printf("freed memory\n");
}

#pragma omp parallel num_threads(4)
void matAdd(double complex** A, double complex** B, int m, int n, double complex** D){
    double complex** C = createMat(m, n);
    #pragma omp for
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    copyMat(C, D, m ,n);
    freeMat(C, m);
    // free(C);
}

#pragma omp parallel num_thread(4)
void matMul(double complex** A, double complex** B, int m, int p, int n, double complex** D){
    double complex ** C = createMat(m, n);
    #pragma omp for
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++) C[i][j] = 0;
    }

    #pragma omp for
    for(int i=0;i<m;i++){
        for(int j=0;j<p;j++){
            if(cabs(A[i][j])<1.e-10) continue;
            for(int k=0;k<n;k++) C[i][k] += A[i][j] * B[j][k];
        }
    }
    copyMat(C, D, m , n);
    // printMat(D, m, n);
    freeMat(C,m);
    // free(C);
}

// #pragma omp parallel num_thread(4)
// void matMulTran(double complex** A, double complex** B, int m, int p, int n, double complex** D){
//     double complex ** C = createMat(m, n);
//     #pragma omp for
//     for(int i=0;i<m;i++){
//         for(int j=0;j<n;j++) C[i][j] = 0;
//     }

//     #pragma omp for
//     for(int i=0;i<m;i++){
//         for(int j=0;j<p;j++){
//             for(int k=0;k<n;k++) C[i][k] += A[i][j] * B[j][k];
//         }
//     }
//     memcpy(D, C, m * sizeof(double complex*));
//     free(C);
// }

// #pragma omp parallel num_threads(4)
double complex** eye(int m){
    double complex ** e = createMat(m, m);
    #pragma omp for
    for(int i=0;i<m;i++){
        for(int j=0;j<m;j++){
            if(i==j) e[i][j] = 1;
            else e[i][j] = 0;
        }
    }

    return e;
}

// #pragma omp parallel num_threads(4)
double complex** zeros(int m, int n){
    double complex** C = createMat(m,n);

    #pragma omp for
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++) C[i][j] = 0;
    }

    return C;
}

// #pragma omp parallel num_threads(4)
double complex** transpose(double complex** A, int m, int n){
    double complex** C = createMat(n, m);
    #pragma omp for
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            C[j][i] = A[i][j];
        }
    }

    return C;
}

// #pragma omp parallel num_threads(4)
double complex** getCol(double complex** A, int m, int k){
    double complex** C = createMat(m,1);

    #pragma omp for
    for(int i=0;i<m;i++){
        C[i][0] = A[i][k];
    }

    return C;
}

// #pragma omp parallel num_threads(4)
double complex norm(double complex** A, int m){
    double complex n = 0;
    #pragma omp for
    for(int i=0;i<m;i++){
        n+=A[i][0]*conj(A[i][0]);
    }
    return csqrt(n);
}

// #pragma omp parallel num_threads(4)
double complex** inner(double complex** A, double complex** B, int m){
    double complex C = 0;
    
    #pragma omp for
    for(int i=0;i<m;i++){
        C += A[i][0] * B[i][0];
    }
}

// #pragma omp parallel num_threads(4)
void scale(double complex** A, double complex x, int m, int n, double complex** C){
    // double complex** C = createMat(m ,n);

    #pragma omp for
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            C[i][j] = x * A[i][j];
        }
    }
    return;
    // return C;
}

double complex trace(double complex** A, int m){
     double complex T;

     for(int i=0;i<m;i++) T+=A[i][i];

     return T;
}

double complex det2x(double complex** A){
    return A[0][0]*A[1][1] - A[0][1]*A[1][0];
}

double complex* eigs2x(double complex** A){
    double complex b = -trace(A, 2);
    double complex c = det2x(A);

    double complex*eigs = malloc(2 * sizeof(double complex));

    eigs[0] = (-b + csqrt(b*b - 4*c))/2;
    eigs[1] = (-b - csqrt(b*b - 4*c))/2;

    return eigs;
}


