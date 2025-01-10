#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// Trapezoidal rule for computing definite integrals:
// Divide the interval on the 'x' axis between the two target points into 'n' equidistant points.
// Draw vertical lines from each of these points on the 'x' axis to the curve along the 'y' axis.
// By connecting the tops of adjacent vertical lines, 'n-1' trapezoids are formed.
// Adding up the areas of all these trapezoids provides a close approximation of the integral, especially when 'n' is large.
 

// Generating points on a parabola of the form 4y = x^2
float **generate_parabola_points(float x, float h, float a, int num_points) {
  // Allocating memory for the points
  float **points = (float **) malloc(sizeof(float *) * 2 * num_points);
  for (int i = 0; i <= num_points; i++) {
    points[i] = (float *) malloc(sizeof(float) * 2);
    points[num_points + i] = (float *) malloc(sizeof(float) * 2);

    // Calculating points on the parabola
    points[i][0] = x;
    points[i][1] = (x * x) / (16 * a);
    points[num_points + i][0] = -x;
    points[num_points + i][1] = (x * x) / (16 * a);
    x += h;
  }
  return points;
}

// Generating evenly spaced points
float **generate_points(float h, float x, int n) {
  // Allocating memory 
  float **points = (float **) malloc(sizeof(float *) * 2* n);
  for (int i = 0; i < n; i++) {
    points[i] = (float *) malloc(sizeof(float) * 2);
    points[n + i] = (float *) malloc(sizeof(float) * 2);

    // Assigning coordinates
    points[i][0] = x;
    points[i][1] = (x+2)/4;
    points[n + i][0] = -x;
    points[n + i][1] = (x+2)/4;
  
    x += h; // Incrementing 'x' value iteratively
  }
  return points;
}


//Calculating the area using the trapezoidal rule
float calculate_area(float x, float h, int n) {
  float area = 0;
  for (int i = 0; i < n;i++){
    area += (h * 0.25*(x + 2 - x * x)) + 0.5 * (h * h * (1 - 2 * x));
    x += h;
  }
  for (int i = n; i <= 2*n; i++) {
	  x-=h;
    area += (h * 0.25*(x + 2 - x * x)) + 0.5 * (h * h * (1 - 2 * x));
  }

  return 2*area;
}
// Free memory
void free_allocated_memory(float **points, int n) {
  for (int i = 0; i < 2 * n; i++) {
    free(points[i]);
  }
  free(points);
}

