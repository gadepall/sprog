#include <stdio.h>
#include <math.h>
// increasing the value of x by h where x_n means the new values of the x
float x_n(float x,float h){
	return x+h;
}
// we have already assumed the y_0 to find y_1 we are using finite differences method for first derivative
float y_1(float y_0,float dy,float h){
	return h*dy+y_0; // here dy means first derivative of y at x=0 i.e y'(0) 
}
// to find the values of y from y2
float y_n(float y_1,float y_0,float h,float x_0){//here y_1 and y_0 represents y_n-1 and y_n-2
	return y_1*(2-h*x_0) + y_0*(1+h*x_0-h*h*x_0) + h*h*x_0;//after substituting the y' and y" values in the given equation i got y_n+2=y_n-1(2-hx_n)+ y_n(1+hx_n-h^2x_n) + h^2x_n
}
