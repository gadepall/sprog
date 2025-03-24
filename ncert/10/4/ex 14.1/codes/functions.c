#include <stdio.h>
#include <math.h>
#include <stdlib.h>
// Function f(x) = x^2 - 3x + 1
double f(double x) {
    return x*x - 3*x + 1;
}

// Derivative of the function f'(x) = 2x - 3
double df(double x) {
    return 2*x - 3;
}

// Newton-Raphson method for solving f(x) = 0
double newtonRaphson(double initialGuess, double tolerance, int maxIterations) {
    double x0 = initialGuess;
    double x1;
    int iteration = 0;

    while (iteration < maxIterations) {
        x1 = x0 - f(x0) / df(x0);
        
        // Check for convergence
        if (fabs(x1 - x0) < tolerance) {
            return x1;
        }

        x0 = x1;
        iteration++;
    }

    printf("Solution did not converge within the maximum iterations\n");
    return x1;
}
void qr_decomposition(double** A,double** Q,double** R,int n){
    for(int j=0;j<n;j++){
        for(int i=0;i<n;i++){
          //here we are copying jth column of A to jth column of Q.
            Q[i][j]=A[i][j];
        }
        for(int k= 0;k<j;k++){
            R[k][j]=0;
            for(int i=0;i<n;i++){
              //projection of Q on A
                R[k][j]+=Q[i][k]*A[i][j];
            }
            for(int i=0;i<n;i++){
              //subtracting vector from projection to get orthogonal
                Q[i][j]-=R[k][j]*Q[i][k];
            }
        }
        // making Q as the orthogonal vector
        R[j][j]=0;
        for(int i=0;i<n;i++){
            R[j][j]+=Q[i][j]*Q[i][j];
        }
        R[j][j] = sqrt(R[j][j]);
        for(int i=0;i<n;i++){
          //making it unit vector.
            Q[i][j]=Q[i][j]/R[j][j];
        }
    }
}

void qr_algorithm(double** A,int n) {
    double** Q=(double**)malloc(n*sizeof(double*));
    double** R =(double**)malloc(n*sizeof(double*));
    for(int i=0;i<n;i++) {
        Q[i]=(double*)malloc(n*sizeof(double));
        R[i]=(double*)malloc(n*sizeof(double));
    }

    for(int iter=0;iter<1000;iter++) {
      // taking maximum iterations
        qr_decomposition(A,Q,R,n);
        //temp for storing A_1
        double** temp =(double**)malloc(n*sizeof(double*));
        for(int i=0;i<n;i++){
            temp[i]=(double*)malloc(n*sizeof(double));
        }

        for(int i=0;i<n;i++) {
            for (int j=0;j<n;j++) {
                temp[i][j]=0;
                for (int k=0;k<n;k++) {
                  //temp=RQ
                    temp[i][j]+=R[i][k]*Q[k][j];
                }
            }
        }

        for (int i=0;i<n; i++) {
            for (int j=0;j<n;j++) {
             // A_1=temp=RQ
                A[i][j]=temp[i][j];
            }
        }

        // Free temp
        for (int i=0;i<n;i++) {
            free(temp[i]);
        }
        free(temp);
    }

    // Free Q,R
    for (int i=0;i<n;i++) {
        free(Q[i]);
        free(R[i]);
    }
    free(Q);
    free(R);
}

