#include<stdio.h>
#include<math.h>
double h=0.0002;
float dydx(float x, float y){
	return (-3*y+exp(-2*x));
}
void vals(float *x,float *y,int n){
	for(int i=0;i<n;i++){
		*y+=dydx(*x,*y)*h;
		*x+=h;
	}
}
