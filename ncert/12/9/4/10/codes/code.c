#include<stdio.h>
#include<math.h>
double h=0.1;
double e=2.7182;
float dydx(float x, float y){
	return ((pow(e,x)-1)*(1/pow(cos(y),2)))/((pow(e,x)*tan(y)));
}
void vals(float *x,float *y,int n){
	for(int i=0;i<n;i++){
		*x+= dydx(*x,*y)*h;
		*y+=h;
	}
}
