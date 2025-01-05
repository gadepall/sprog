#include<stdio.h>
#include<stdlib.h>


/*
	The method used in this question is the Euler's method.
	y_new = y_old + h * (dy/dx) [at (x_old, y_old)]
	x_new = x_old + h;
	This is a method used for the approximate plotting for a differential equation.
*/

//function to generate the array of coordinates to be plotted
float** pointsGenerate(float x0, float y0, float h, int n){
	float** points = malloc(sizeof(float*) * 3 * (n/2));
	float x = x0, y = y0;
	//first for loop is to plot the points where x is negative
	for(int i=0;i<n/2;i++){
		points[i] = (float*)malloc(sizeof(float) * 2);
		points[i][0] = x;
		points[i][1] = y;
		// dy/dx = (4x + 6y + 5)/(3y + 2x + 4)
		y -= h * ((3*y + 2*x + 4)/(4*x + 6*y + 5));
		x -= h;
	}

	x = x0;
	y = y0;
	
	//second for loop is to plot the points where x is positive
	for(int i=n/2;i<3*(n/2);i++){
		points[i] = (float*)malloc(sizeof(float) * 2);
		points[i][0] = x;
		points[i][1] = y;
		// dy/dx = (4x + 6y + 5)/(3y + 2x + 4)
		y += h * ((3*y + 2*x + 4)/(4*x + 6*y + 5));
		x += h;
	}
	return points;
}

//function to free the array of coordinates after use
void freeMemory(float **points, int n){
	for(int i=0;i<3*(n/2);i++){
		free(points[i]);
	}
	free(points);
}
