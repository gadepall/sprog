#include<stdio.h>
#include<math.h>
#include<stdlib.h>

//Functions specific to our question
//f(x)
double func(double x){
  return 3*x*x-5*x+2;
}
//manually found derivative of f(x)
double deriv(double x){
  return 6*x-5;
}
//This function generates points on the known function to plot
void makePlotArray(double xo, double yo, int n, double x[n], double y[n], double h, double(*func)(double)){
  x[n/2] = xo; y[n/2] = yo;
  //Plotting backwards
  for(int i = n/2+1; i < n; i++){
    x[i] = x[i-1]-h;
    y[i] = func(x[i]);
  }
  //Plotting forwards
  for(int i = n/2-1; i > -1; i--){
    x[i] = x[i+1] + h;
    y[i] = func(x[i]);
  }
}
//The Newton-Raphson algorithm
void NewtonRootFind(double xo, int iters, double (*func)(double), double (*deriv)(double), double root[1]){
  double xn = xo;
  for(int i = 0; i<iters; i++){
    double old = xn;
    //Update equation
    xn = xn - func(xn)/deriv(xn);
    //Checking for convergence and breaking once achieved
    if(xn - old < 1e-6 && xn - old > - 1e-6){
      printf("iterations = %d, found root = %lf\n", i, xn);
      break;
    }
  }
  root[0] = xn;
}

//Functions for the Matrix approach
//Basic print matrix, norm of vector, and matrix multiplication functions 
void printMat(double **matrix, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%lf ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
double norm(double *vec, int size) {
  double normVal = 0.0;
  for (int i = 0; i < size; i++) {
    normVal += vec[i] * vec[i];
  }
  return sqrt(normVal);
}
void matMult(double **mat1, double **mat2, double **res, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      res[i][j] = 0.0; 
    }
  }

  for (int i = 0; i < n; i++) {
    for (int k = 0; k < n; k++) {
      for (int j = 0; j < n; j++) {
        res[i][j] += mat1[i][k] * mat2[k][j];
      }
    }
  }
}
//This function performs the Householder reflections, and can be reused in other situations
void householderReflection(double *vec, double **hMatrix, int n, int startRow, int startCol) {
  int size = n - startRow;
  double normU = norm(vec, size);
  if (normU < 1e-10) return;
  double alpha = (vec[0] <= 0 ? 1 : -1) * norm(vec, size);
  vec[0] -= alpha;
  normU = norm(vec, size);
  for (int i = 0; i < size; i++) {
    vec[i] /= normU;
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i < startRow || j < startCol) {
        hMatrix[i][j] = (i == j) ? 1.0 : 0.0;
      } else {
        hMatrix[i][j] = (i == j ? 1.0 : 0.0) - 2.0 * vec[i - startRow] * vec[j - startCol];
      }
    }
  }
}
//This function performs the QR decomposition aMatrix = qMatrix * rMatrix
void qrDecomp(double **aMatrix, int n, double **qMatrix, double **rMatrix) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      qMatrix[i][j] = (i == j ? 1.0 : 0.0);
      rMatrix[i][j] = aMatrix[i][j];
    }
  }
  for (int k = 0; k < n - 1; k++) {
    int size = n - k;

    double *vec = (double *)malloc(size * sizeof(double));
    for (int i = 0; i < size; i++) {
      vec[i] = rMatrix[k + i][k];
    }

    double **hMatrix = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
      hMatrix[i] = (double *)malloc(n * sizeof(double));
    }
    householderReflection(vec, hMatrix, n, k, k);

    double **temp = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
      temp[i] = (double *)malloc(n * sizeof(double));
    }
    matMult(hMatrix, rMatrix, temp, n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        rMatrix[i][j] = temp[i][j];
      }
    }
    matMult(qMatrix, hMatrix, temp, n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        qMatrix[i][j] = temp[i][j];
      }
    }
    free(vec);
    for (int i = 0; i < n; i++) {
      free(hMatrix[i]);
      free(temp[i]);
    }
    free(hMatrix);
    free(temp);
  }
}
//This function extracts the eigenvalues from our converged Matrix, also takes care of Jordan blocks to give complex eigenvalues
void extractEigenvalues(double **aMatrix, int n, double *realPart, double *imagPart) {
  for (int i = 0; i < n; i++) {
    if (i < n - 1 && fabs(aMatrix[i + 1][i]) > 1e-7) {
      double a = aMatrix[i][i];
      double b = aMatrix[i][i + 1];
      double c = aMatrix[i + 1][i];
      double d = aMatrix[i + 1][i + 1];

      double trace = a + d;
      double det = a * d - b * c;
      realPart[i] = realPart[i + 1] = trace / 2.0;
      imagPart[i] = sqrt(fabs(det - trace * trace / 4.0));
      imagPart[i + 1] = -imagPart[i];
      i++;  
    } 
    else {
      realPart[i] = aMatrix[i][i];
      imagPart[i] = 0.0;
    }
  }
}


//The main function, we're only running the matrix approach here, the Newton-Raphson approach is run from python
void main(){
  //Variables being used. aMatrix will be the companion matrix, qMatrix and rMatrix will be used to store the QR decomposition, temp is used in matrix multiplication, realCurr and imagCurr will store the eigenvalues
  //n is the size of the matrix, maxiters is the maximum number of QR algorithm iterations we want to run
  int n = 2, maxiters = 100;
  double **aMatrix = (double **)malloc(n * sizeof(double *));
  double **qMatrix = (double **)malloc(n * sizeof(double *));
	double **rMatrix = (double **)malloc(n * sizeof(double *));
	double **temp = (double **)malloc(n * sizeof(double *));
	double *realCurr = (double *)malloc(n * sizeof(double));
	double *imagCurr = (double *)malloc(n * sizeof(double));
	for (int i = 0; i < n; i++) {
		temp[i] = (double *)malloc(n * sizeof(double));
		qMatrix[i] = (double *)malloc(n * sizeof(double));
		rMatrix[i] = (double *)malloc(n * sizeof(double));
    aMatrix[i] = (double *)malloc(n * sizeof(double));
	}
  //The companion matrix
  aMatrix[0][0] = 0.0; aMatrix[0][1] = -(2.0/3.0);
  aMatrix[1][0] = 1.0; aMatrix[1][1] = (5.0/3.0);
  //The QR algorithm, with a convergence check in each loop
  for(int iters = 0; iters<maxiters; iters++){
    double maxChange = 0.0;
    //A=QR
    qrDecomp(aMatrix, n, qMatrix, rMatrix);
    //"temp = RQ, A = temp" update, here maxChange stores the difference between previous value and current value to check for convergence
    matMult(rMatrix, qMatrix, temp, n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if(aMatrix[i][j] - temp[i][j] > maxChange){
          maxChange = aMatrix[i][j]-temp[i][j];
        } 
        aMatrix[i][j] = temp[i][j];
      }
    }
    //Convergence check, tolerance is 1e-7. If the maximum change in entries of the matrix meets this check, the QR algorithm ends
    if(maxChange < 1e-7){
        printf("converged after %d iterations\n", iters);
        break;
      }
  }
  //Extract the eigenvalues and print them. Remember that the eigenvalues of the companion matrix were the roots of the polynomial.
  extractEigenvalues(aMatrix, n, realCurr, imagCurr);
  printf("roots of the polynomial are: %lf+j%lf, %lf+j%lf", realCurr[0], imagCurr[0], realCurr[1], imagCurr[1]);
}
