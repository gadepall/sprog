#include<stdio.h>
#include<stdlib.h>
#include<math.h>


//function to generate the array of coordinates to be plotted
float** pointsGenerate(float a, float b, float c, float x0, float h, int n){
	float** points = malloc(sizeof(float*) * n);
	float x = x0, y = -(c + (a * x))/b;
	for(int i=0;i<n;i++){
		points[i] = (float*)malloc(sizeof(float) * 2);
		points[i][0] = x;
		points[i][1] = y;
		x += h;
		y = -(c + (a * x))/b;
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
