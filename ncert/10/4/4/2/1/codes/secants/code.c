#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TOL 1e-7
#define MAX 1000

double f1(double x) {
    return 2 * x * x + 2 * sqrt(10) * x + 5;
}

double f2(double x) {
    return 2 * x * x - 2 * sqrt(10) * x + 5;
}

double secants(double x0, double x1, double (*f)(double))
{
	for(int i = 0; i < MAX; i++)
	{
		double current = f(x1);
		double prev = f(x0);
		if(fabs(current - prev) < TOL)
		{
			printf("The difference is too small due to horizontal tangent.\n");
			return -1;
		}

		double next = x1 - current * (x1 - x0) / (current - prev);

		if(fabs(next - x1) < TOL)
		{
			printf("Number of iterations required to converge; %d\n", i + 1);
			return next;
		}

		x0 = x1;
		x1 = next;
	}
	printf("Maximum number of iterations reached.\n");
	return -1;
}

int main()
{
	double *root = malloc(2 * sizeof(double));
    	if (root == NULL) {
        	printf("Memory allocation failed.\n");
        	return 1;
    	}

    	root[0] = secants(-3, -2.9 ,f1);
    	root[1] = secants(0, 0.1, f1);

    	if (root[0] != -1 && root[1] != -1) {
        	printf("Roots of the first quadratic are %.7lf and %.7lf.\n", root[0], root[1]);
    	} 
    	else 
    	{
        	printf("Failed to find roots for the first quadratic.\n");
    	}

    	root[0] = secants(3, 2.9, f2);
    	root[1] = secants(0, 0.1, f2);

    	if (root[0] != -1 && root[1] != -1) {
        	printf("Roots of the second quadratic are %.7lf and %.7lf.\n", root[0], root[1]);
    	} 
    	else 
	{
        	printf("Failed to find roots for the second quadratic.\n");
    	}

    	free(root);
	return 0;
}
