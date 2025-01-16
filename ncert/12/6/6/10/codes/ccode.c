#include<stdio.h>
#include<stdlib.h>
#include<math.h>


/*
	The method used in this question is the Gradient Descent.
	x_new = x_old - h * (dy/dx) [at (x_old, y_old)]
	This is a method used for finding the points of local minima or maxima for a function.
*/

//function to generate the array of coordinates to be plotted
float** pointsGenerate(float x0, float y0, float h, float k, int n){
	float** points = malloc(sizeof(float*) * n);
	float x = x0, y = y0;
	//for loop to plot the points
	for(int i=0;i<n;i++){
		points[i] = (float*)malloc(sizeof(float) * 2);
		points[i][0] = x;
		points[i][1] = (((M_PI * M_PI)/4 + M_PI) * x * x) + (k*k/16) - ((M_PI * k * x)/4);
		x += h;
	}
	return points;
}

//function to find the point of minima using gradient descent
float minima(float x0, float h, float tol, float k, int n){
	float x = x0;
	for(int i=0;i<n;i++){
		float temp = h * ((((0.5 * M_PI * M_PI) + (2 * M_PI)) * x) - (M_PI * k * 0.25));
		x -= temp;
		if(fabs(temp) < tol){
			return x;
		}
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
