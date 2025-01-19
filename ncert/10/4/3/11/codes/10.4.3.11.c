#include<stdio.h>
#include<math.h>
#include<stdlib.h>


double newton(){
        double x0 = 16;
        double tol = 1e-6;
	double x1 =  x0 -  ((x0*x0 - 6*x0 - 216)/(2*x0 - 6));
	while(abs(x1-x0)>tol){
	        x0 = x1;
		x1 = x0- ((x1*x1 - 6*x1 - 216)/(2*x1 - 6));
	}
	return x1;
}

double *coords(){
double h = 0.01;
 static double points[1000];
for(int i=0;i<=998;i+=2){
	if(i==0){
		points[i]=15;
		points[i+1] = points[i]*points[i] - 6*points[i] - 216;
	}
	else{
		points[i]= points[i-2]+h;
		points[i+1] = points[i]*points[i] - 6*points[i] - 216;

	}
}
return points;

}
// Function definition: f(x) = x^2 - 6x - 216
double f(double x) {
    return x * x - 6 * x - 216;
}

// Bisection method implementation
double bisection() {
	double a = 10;
	double b = 20;
	double tol = 1e-4;
	double max_iter = 1000;
       
    // Check if the initial values are valid
    if ((a*a - 6*a -216) * (b*b - 6*b - 216) > 0) {
        printf("No root found in the interval [%lf %lf].\n", a,b);
        return -1;
    }

    double c;
    for (int i = 0; i < max_iter; i++) {
        // Calculate the midpoint
        c = (a + b) / 2.0;

        // Check if the midpoint is a root or if the interval is small enough
        if (fabs(c*c - 6*c - 216) < tol || (b - a) / 2.0 < tol) {
            return c;
        }

        // Determine which subinterval contains the root
        if ((a*a - 6*a - 216) * (c*c - 6*c -216) < 0) {
            b = c;
        } else {
            a = c;
        }
    }

    return c;
}







