#include<math.h>

//Function to find x_n
double x_n(double x_0,double y_0,double h){
	double p=(x_0*y_0)/(1-pow(y_0,2));
	return x_0 - (h*p) ;
}

//Function to find y_n
double y_n(double y_0,double h){
	return y_0 + h;
}
