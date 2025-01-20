#include <stdio.h>
#include <math.h>
#define EPSILON 0.000001 


void function(double *x,double *y,int n){
	for(int i=0;i<n;i++){
		y[i]=x[i]*x[i]-45*x[i]+324;
	}
}
// Define the function f(x) = x^2 - 45x + 324
double f(double x) {
    return x * x - 45 * x + 324;
}

// Define the derivative of the function f'(x) = 2x - 45
double f_prime(double x) {
    return 2 * x - 45;
}

double NR(double guess1){
    double x0, root;
    x0=guess1;

    
    int iteration = 0;
    while (1) {
        double fx = f(x0);
        double fx_prime = f_prime(x0);
        root = x0 - fx / fx_prime;
        printf("Iteration %d: x = %.6f\n", iteration + 1, root);
        if (fabs(root - x0) < EPSILON) {
            break;
        }
        x0 = root;
        iteration++;
    }
    return root;
}

