#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//function to generate the array of coordinates to be plotted
float** pointsGenerateLine(float x0, float y0, float h, int n){
	float** points = malloc(sizeof(float*) * n);
	float x = x0, y = y0;
	//for loop to plot the points got through method of finite differences
	for(int i=0;i<n;i++){
		points[i] = (float*)malloc(sizeof(float) * 2);
		points[i][0] = x;
		points[i][1] = y;
		// dy/dx = 3/2
		y += h*1;
		x += h;
	}
	return points;
}
float** pointsGenerateParabola(float x0, float y0, float h, int n){
	float** points = malloc(sizeof(float*) * n);
	float x = x0, y = y0;
	//for loop to plot the points got through method of finite differences
	for(int i=0;i<n;i++){
		points[i] = (float*)malloc(sizeof(float) * 2);
		points[i][0] = x;
		points[i][1] = y;
		// dy/dx = 2x
		y += h * (2 * x);
		x += h;
	}
	return points;
}
//function to calculate value of area under the graph of a function. i.e., to calculate the integral of the given function
float area(float a, float b, int n){
	float area_parabola = 0, area_line = 0;
	float x = a;
	float h = (b - a)/(float)(n);
	for(x=a+h;x<=b;x+=h){
		area_parabola += h * 0.375 * ((x * x) + ((x-h) * (x-h)));
		area_line += h * 0.25 * ((x + 2) + ((x-h) + 2));
	}
	return area_line - area_parabola;
}
//function to free the array of coordinates after use
void freeMemory(float **points, int n){
	for(int i=0;i<n;i++){
		free(points[i]);
	}
	free(points);
}
