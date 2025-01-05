#include <stdio.h>
#include <math.h>

float y(float x_n, float y_n, float h){
	return (y_n + (h * ((y_n/x_n)-sin(y_n/x_n))));
}

float x(float x_n, float h){
	return (x_n + h);
}

float y_val(float x){
	return (2*x*atan(1/x));
}
