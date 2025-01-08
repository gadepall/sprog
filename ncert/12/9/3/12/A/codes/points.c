#include<stdio.h>

void value(double h,double y[],double dy[]) {
    double x=0.0;
    //y[0]=0.0;
    //dy[0]=1.0;
    
    for (int i = 1; i <=51 ; i++) {
        // Compute new y and y'
        double ddy =x*x*dy[i-1] + x - x * y[i-1];
        y[i]=y[i-1]+h*dy[i-1];
        dy[i]=dy[i-1] + h*ddy;
        x+=h;
    }
}



