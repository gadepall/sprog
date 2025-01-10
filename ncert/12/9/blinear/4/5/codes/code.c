#include <stdio.h>
#include <math.h>

void Trapezoid(double x_0, double y_0, double x_final, double step_size, double* x_vals, double* y_vals) {
    int data_points = ((x_final - x_0) / step_size) + 1;
    double x = x_0;
    double y = y_0;

    for (int i = 0; i < data_points; i++) {
        x_vals[i] = x;
        y_vals[i] = y;
	x = x + step_size;
	y = y + step_size * ((exp(2*x)-1) / (exp(2*x)+1)) + ((step_size*step_size)*(4*exp(2*x)))/(2*(exp(2*x)+1)*(exp(2*x)+1)); //euler's method with dy/dx`
   }
}

