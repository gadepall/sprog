#include<stdio.h>
double f(double x){
return (2*x*x+x)/(x*x*x+x*x+x+1);
}
void bilinear(int n,double x[],double y[],double h,double x0,double y0){
	double x1,y1;
	x[0]=x0;
	y[0]=y0;
for(int i=1;i<n;i++){
x1=x0+h;
x[i]=x1;
y1=y0+(h/2)*(((2*x1*x1+x1)/(x1*x1*x1+x1*x1+x1+1))+((2*x0*x0+x0)/(x0*x0*x0+x0*x0+x0+1)));
y[i]=y1;
x0=x1;
y0=y1;
}
}
void trapezoidal(int n,double x[],double y[],double h,double x0,double y0){
double y1,x1;
x[0]=x0;
y[0]=y0;
for(int i=0;i<n;i++){
x1=x0+h;
x[i]=x1;
y1=y0+(h/2)*(f(x1)+f(x0));
y[i]=y1;
y0=y1;
x0=x1;
}
}
