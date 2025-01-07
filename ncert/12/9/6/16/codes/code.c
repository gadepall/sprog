
#include <stdio.h>
#include <math.h>

double h =0.001; //step size

double f(double x,double y){ //Function f(x,y) representing dy/dx = x + y
    return x + y;
}

void solve(double *x,double *y,int steps){ //solving using EULER'S METHOD
    for(int i=0;i<steps;i++){
        *y += h * f(*x,*y); //y using dy/dx
        *x += h; //incrementing x
    }
}

