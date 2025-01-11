#include<stdio.h>
#include<math.h>
#include<stdlib.h>
//assumed initial conditions
double a = 4.0;
double e = sqrt(7)/4;
double h = 0.001;
int n = 2646;
double b = 3.0;

// function to calculate area  by using the  difference equation obtained from the trapezoidal rule
double calc_area() {
    
    double A = 0;
    double x = 0;
    for (int i = 0; i < n; i++) {
        A += h*(0.75)*sqrt(a*a - x*x) + (0.5)*h*h*((-b*x)/(a*sqrt(a*a - x*x)));//difference equation of area A_n = A_n-1 + h*y + (1/2)*h*h*dy/dx
        x += h;  
    }
    return 2*A; // as total area is twice the area calculated
}




