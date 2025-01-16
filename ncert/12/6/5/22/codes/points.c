#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double Area(double x, double a, double b, double c)
{
	return a*x*x + b*x + c;
}

void generate_points(double *x, double* y, int n, double x_0, double x_end, double a, double b, double c)
{
	double h = (x_end - x_0)/n;
	for(int i = 0; i < n; i++)
	{
		x[i] = x_0;
		y[i] = Area(x_0, a, b, c);
		x_0 += h;
	}
}

void gradient_descent(double x_0, double h, double (*f)(double), double tolerance, double *minimum)
{
	int count = 0; 
	while(fabs((*f)(x_0)) > tolerance)
	{
		x_0 -= (*f)(x_0)*h;
		count++;
	}
	printf("Number of iterations : %d\n", count);
	*minimum = x_0;
}
