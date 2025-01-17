#include <stdio.h>
#include <math.h>
void function(double *x,double *y,int n){
	for(int i=0;i<n;i++){
		y[i]=280+180*x[i]+720/(x[i]);
	}
}
double derivative(double x){
	return 180-720/(x*x);
}

double  gradient_descent() {
    double l = 1.0;  // Initial guess for l (should be > 0 to avoid division by zero)
    double prev_l;

    do {
        prev_l = l;
        l = l - 0.001 * derivative(prev_l);
        

    } while (fabs(l - prev_l) > 1e-6);
    return l; 
}


