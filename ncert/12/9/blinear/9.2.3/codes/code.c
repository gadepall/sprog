#include <stdio.h>
#include <math.h>

double dydx(double x){
  return -sin(x);
}

void fun(double x, double y, double h, double *x_values,double *y_values){
  x=0;
  y=1;
  h=0.1;
  for(int i=0;i<100;i++){
    x_values[i]=x;
    y_values[i]=y;
    y = y- (1.0 /2)*h*(sin(x)+sin(x+h));
    x = x+h;
  }
}
