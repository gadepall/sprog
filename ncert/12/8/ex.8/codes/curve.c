#include <stdio.h>
#include <math.h>

double conic(double x, double a, double b) 
{
	return (b / a) * sqrt(a * a - x * x);
}

double line(double x, double a, double b) 
{
    	return (b / a) * (a - x);
}

void generate_conic(int size, double a, double b, double *x_coord, double *y_coord_pos, double *y_coord_neg) 
{
    	double h = (2 * a) / size;
    	double x = -a;
    	for (int i = 0; i < size + 1; i++) 
	{ 
        	x_coord[i] = x;
        	double temp = conic(x_coord[i], a, b); 
        	y_coord_pos[i] = temp;
        	y_coord_neg[i] = -temp;
        	x += h;
    	}
}

void generate_line(int size, double a, double b, double *x_coord, double *y_coord) 
{
    	double h = (2 * a) / size;
    	double x = -a;
    	for (int i = 0; i < size + 1; i++) 
	{ 
        	x_coord[i] = x;
        	y_coord[i] = line(x_coord[i], a, b);
        	x += h;
    	}
}

double area(int trapezoids, double a, double b, double x_0, double x_n)
{
	double A = 0.0;
	double h = (x_n - x_0)/trapezoids;
	while(x_0<x_n-h)
	{
		//double x = x_0 + h;
		double y1 = conic(x_0, a, b) - line(x_0, a, b);
		x_0 += h;
		double y2 = conic(x_0, a, b) - line(x_0, a, b);
		A += (h/2) * (y1 + y2);
	}
	return A;
}
