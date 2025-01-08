#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double h = 0.1; //step size which I am considering

double f(double x, double y, double v) {
    return cos(3*x)+sin(3*x);           /* d2y/dx2 = f(x,y,dy/dx) */
}

void solve(double *x,double *y,double *v,int steps){
    for(int i=0;i<steps;i++){
        *y += h * (*v); //y value using dy/dx
        
        *v += h * f(*x,*y,*v); //value v updating using d2y/dx2
        
        *x += h; //x value
    }
}

