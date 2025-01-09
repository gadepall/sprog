#include <stdio.h>
#include<math.h>
void fun(double n,double t,double h,double *x,double *y){
  n=100000;
  t=0;
  h=0.1;
   double k = log(11.0 / 10.0) * 0.5;
  for(int i=0;i<100;i++){
    x[i]=t;
    y[i]=n;
    n = n * (1 + h * k); 
    t=t+h;
  }
}
