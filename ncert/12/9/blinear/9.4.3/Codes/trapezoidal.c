#include <math.h>
void trapezoidal(double *x, double *y,double *y_trapezoid, int n, double h) {
    y_trapezoid[0] = 0; 
    for (int i = 0; i < n - 1; i++) {
        y_trapezoid[i+1]=y_trapezoid[i]+(h/2)*(2-y[i]-y[i+1]);
    }
}
void function(double *x,double *y,int n){
	y[0]=0;
	for(int i=0;i<n;i++){
		y[i]=1-exp(-1*x[i]);
	}

}
