#include<math.h>
#include<stdio.h>

//Functions specific to our question
double func(double x){
	return pow((x+1),3)*pow((x-2),4);
}

double deriv(double x){
	return 3*pow((x+1),2)*pow((x-2),4)+4*pow((x+1)*(x-2),3);
}

void makePlotArray(double xo, double yo, int n, double x[n], double y[n], double h){
	x[n/2] = xo; y[n/2] = yo;
	for(int i = n/2+1; i < n; i++){
		x[i] = x[i-1]-h;
		y[i] = func(x[i]);
	}
	for(int i = n/2-1; i > -1; i--){
		x[i] = x[i+1] + h;
		y[i] = func(x[i]);
	}
}

void gradDesc(double xo, double e, double tol, double coords[2], double (*func)(double), double (*deriv)(double)){
	double xi = xo+e*deriv(xo);
	while(fabs(xi - xo) >= tol){
		xo = xi;
		xi = xo + e * deriv(xo);
		//printf("%lf,%lf|%lf\n", xi, func(xi),deriv(xi));
	}
	coords[0] = xi; coords[1] = func(xi);
}
