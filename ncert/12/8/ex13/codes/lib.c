#include <math.h>
void trapezoidal_1(double *x, double *y, int n, double h) {
    y[0] = 0; 
    for (int i = 0; i < n - 1; i++) {
        y[i+1]=y[i]+h*x[i]*x[i]/4+(h*h*x[i])/4;
    }
}
void trapezoidal_2(double *x, double *y, int n, double h) {
    y[1] = 0; 
    for (int i = 1; i < n - 1; i++) {
        y[i+1]=y[i]+h*2*sqrt(x[i])+(h*h)/(2*sqrt(x[i]));
    }
}


void function_1(double *x,double *y,int n){
	for(int i=0;i<n;i++){
		y[i]=x[i]*x[i]/4;
	}
}
void function_2(double *x,double *y,int n){
	for(int i=0;i<n;i++){
		y[i]=2*sqrt(x[i]);
	}
}
