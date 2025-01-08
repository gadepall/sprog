#include <stdio.h>
#include <math.h>

#define N 10000
#define H 0.0001

//function to calculate dy/dx
double derivative(double x,double y){
return pow(x,2)-y/x;                 
}

// Function to calculate and store x, y values in arrays
void calculate_values(double x_values[], double y_values[]) {
    double x_0 = 1.0, y_0;  //initialising x 
    int i;

    y_0 = pow(x_0,3)/4+1/x_0;
    x_values[0] = x_0;
    y_values[0] = y_0;

    for (i = 1; i < N; i++) {
        x_0 += H;
        y_0 += derivative(x_0, y_0) * H; //iteration to calculate different values of x and y
        x_values[i] = x_0;               //storing values of x and y in arrays
        y_values[i] = y_0;
    }
}

