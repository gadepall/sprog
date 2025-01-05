#include<stdio.h>
#include<math.h>
double h=0.0002;
float dydx(float x, float y){
	return (1+pow(x,2))*(1+pow(y,2));
}
void vals(float *x,float *y,int n){
	for(int i=0;i<n;i++){
		*y+=dydx(*x,*y)*h;
		*x+=h;
	}
}
