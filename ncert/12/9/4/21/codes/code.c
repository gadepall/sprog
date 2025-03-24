#include <stdio.h>

void fun(double p,double t,double h,double *x,double *y){
  p=1000;
  t=0;
  h=0.1;
  for(int i=0;i<100;i++){
    x[i]=t;
    y[i]=p;
    p=p+(h*(p/20));
    t=t+h;
  }
}
