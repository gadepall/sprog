#include<stdio.h>
#include<stdlib.h>
#include<math.h>


/*
	The method used in this question is the Bilinear Transform.
	This is a method used for the approximate plotting for a differential equation.
*/

//function to generate the array of coordinates to be plotted
float** pointsGenerate(float x0, float y0, float h, int n){
	float** points = malloc(sizeof(float*) * n);
	float x = 0;
	//for loop to plot the points got through method of finite differences
	points[0] = (float*)malloc(sizeof(float) * 2);
	points[1] = (float*)malloc(sizeof(float) * 2);
	points[2] = (float*)malloc(sizeof(float) * 2);
	points[0][0] = x, points[0][1] = 0;
	x += h;
	points[1][0] = x, points[1][1] = h;
	x += h;
	points[2][0] = x;
	points[2][1] = (h*h + 8*h - 2*h*h*h)/(4 + h*h);
	y0 = h;
	float y1 = points[2][1];
	for(int i=3;i<n;i++){
		points[i] = (float*)malloc(sizeof(float ) * 2);
		float y2 = y1*2*((4 - h*h)/(4 + h*h)) - y0;
		x += h;
		points[i][0] = x;
		points[i][1] = y2;
		y0 = y1;
		y1 = y2;
	}
	return points;
}



//function to free the array of coordinates after use
void freeMemory(float **points, int n){
	for(int i=0;i<n;i++){
		free(points[i]);
	}
	free(points);
}
