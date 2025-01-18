#include<stdio.h>
#include<stdlib.h>
#include<math.h>


/*
	The method used in this question is the Euler's method.
	y_new = y_old + h * (dy/dx) [at (x_old, y_old)]
	x_new = x_old + h;
	This is a method used for the approximate plotting for a differential equation.
*/

//function to generate the array of coordinates to be plotted
float** pointsGenerate(float x0, float y0, float h, int n){
	float** points = malloc(sizeof(float*) * n);
	float x = x0, y = y0;
	//for loop to plot the points got through method of finite differences
	for(int i=0;i<n;i++){
		points[i] = (float*)malloc(sizeof(float) * 2);
		points[i][0] = x;
		points[i][1] = y;
		// dy/dx = sin2x + 3y(cotx)
		y = x*x + 5*x - 1800;
		x += h;
	}
	return points;
}

float newtonRalphson(float x, int n){
	for (int i=0; i<(n);i++){
    	x = x - ((x*x + 5*x - 1800) / (2*x + 5));
  	}
  	return x;
}

//function to free the array of coordinates after use
void freeMemory(float **points, int n){
	for(int i=0;i<n;i++){
		free(points[i]);
	}
	free(points);
}
