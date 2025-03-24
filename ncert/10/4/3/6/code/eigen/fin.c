#include <stdio.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <complex.h>
#include <stdlib.h>
#include "matcals.h"
#include "QR.h"

int main(int argc, char* argv[]){
    clock_t start, end;
    start = clock();
    // if(argc<=1) return -1;

    // srand(5678);
    // int m = atoi(argv[1]);
    // double complex** A = createMat(m, m);
    // for(int i=0;i<m;i++){
    //     for(int j=0;j<m;j++){
    //         A[i][j] = (double complex) ((rand()/(double)RAND_MAX)-0.5);
    //     }
    // }
    
    
    // double complex* eig;
    // // hessenberg(A, m, Hq);

    // double complex** Ak = QReig(A, m);
    // printMat(Ak, m,m);printf("\n");
    // eig = eigs(Ak, m);
    // for(int i=0;i<m;i++) printf("%lf + i%lf\n",creal(eig[i]),cimag(eig[i]));

    // freeMat(A, m);
    // freeMat(Ak,m);
    // free(eig);
    // // freeMat(Hq[0], m);
    // // freeMat(Hq[1],m);
    // // free(Hq);

    double complex** A = zeros(5, 5);
    // A[0][0] = 0; A[0][1] = 2700;
    // A[1][0] = 1; A[1][1] = 60;

    for(int i=1;i<5;i++){
        A[i][i-1] = 1;
    }

    A[0][4] = 120;
    A[1][4] = -274;
    A[2][4] = 225;
    A[3][4] = -85;
    A[4][4] = 15;

    // double complex** Hq = createMat(5 ,5);
    // double complex*** Hq = malloc(2 * sizeof(double complex**));
    // Hq[0] = createMat(5 , 5); Hq[1] = createMat(5 , 5);
    // hessenberg(A, 5, Hq);


    printMat(A, 5, 5);
    double complex** Ak = QReig(A, 5);
    double complex* eig = eigs(Ak ,5);
    // printf("%lf, %lf",creal(eig[0]), creal(eig[1]));
    for(int i =0; i<5; i++){
        printf("%lf\n",eig[i]);
    }

    end = clock();
    printf("%lf\n", (double) (end-start)/CLOCKS_PER_SEC);
    

    return 0;
}