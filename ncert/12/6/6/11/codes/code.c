#include <stdio.h>
#include <math.h>
double dA(double b){
  return 10 - (8*b) - (3.14*b);  
}

void fun(double b,double alpha,double *b_values,double A,double *A_values){
  b = 0.0;
  alpha = 0.01;
  A = 0;
  for(int i=0;i<100;i++){
    b_values[i] = b;
    A_values[i] = A;
    b = b+ (alpha*dA(b)); 
    A = ((3.14*b*b)/2) + (10*b) - ((4+3.14)*b*b);
  }
}
