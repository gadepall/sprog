#include <stdio.h>
#include <math.h>

double dy_dx(double x,double y){
  return -tan(x)/tan(y);


}

void fun(double x,double y,double h,double *x_points,double *y_points){
  y=3.414/4;
  x=0;
  h=0.1;
  for(int i=0;i<100;i++){
    x_points[i]=x;
    y_points[i]=y;
    y=y+(h*(dy_dx(x,y)));
    x=x+h;
  }
}
