#include<math.h>

//Finite Differences function

double improvedEulers(double xo, double yo, int n, double x[n], double y[n], double h, double (*deriv)(double, double)){
	x[n/2] = xo; y[n/2] = yo;
	double k1, k2;
	for(int i = n/2+1; i < n; i++){
		y[i] = y[i-1] - h;
		k1 = h/deriv(x[i-1], y[i-1]);
		k2 = h/deriv(x[i-1]-k1, y[i]);
		x[i] = (x[i-1] - (k1+k2)/2);
	}
	for(int i = n/2-1; i > -1; i--){
		y[i] = y[i+1] + h;
		k1 = h/deriv(x[i+1], y[i+1]);
		k2 = h/deriv(x[i+1]+k1,y[i+1]);
		x[i] = x[i+1] + (k1+k2)/2;
	}
}

double finDif(double xo, double yo, int n, double x[n], double y[n], double h, double (*deriv)(double, double)){
	x[n/2] = xo; y[n/2] = yo;
	for(int i = n/2+1; i < n; i++){
		x[i] = x[i-1] - h;
		y[i] = (y[i-1] - (deriv(x[i-1], y[i-1]))*h);
	}
	for(int i = n/2-1; i > -1; i--){
		x[i] = x[i+1] + h;
		y[i] = y[i+1] + deriv(x[i+1], y[i+1])*h;
	}
}

//Functions specific to our question

//Derivative as given in the question
double dybydx(double x, double y){
	return (y*y)/(1-x*y);
}
//To plot the relation between y and x found using the theoretical method
double runOurFunc(double xo, double yo, int n, double x[n], double y[n], double h){
	x[n/2] = xo; y[n/2] = yo;
	for(int i = n/2+1; i < n; i++){
		y[i] = y[i-1] - h;
		x[i] = (log(y[i]) + 5)/y[i];
	}
	for(int i = n/2-1; i > -1; i--){
		y[i] = y[i+1] + h;
		x[i] = (log(y[i]) + 5)/y[i];
	}

}
