#include<math.h>

//Function to find x_n
double x_n(double x_0,double h){
	return x_0+h ;
}

//Function to find y_n
double y_n(double x_0,double y_0,double h){
	return y_0+((x_0+y_0)/x_0)*h;
}
