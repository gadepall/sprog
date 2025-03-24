#include<stdio.h>
#include<stdlib.h>
#include<math.h>

/*
    The method used in this question is the Euler's method.
    y_new = y_old + h * (dy/dx) [at (x_old, y_old)]
    x_new = x_old + h;
    This is a method used for the approximate plotting for a differential equation.
*/

// Function to generate points on the line
float** pointsGenerateLine(float a, float b, float x0, float y0, float h, int n){
    float** points = malloc(sizeof(float*) * n);
    float x = x0, y = y0;
    // For loop to calculate points using the equation of the line: y = b - (b/a)*x
    for (int i = 0; i < n; i++) {
        points[i] = (float*)malloc(sizeof(float) * 2);
        points[i][0] = x;
        points[i][1] = b - (b/a) * x;
        x += h;
    }
    return points;
}

// Function to generate points on the ellipse
float** pointsGenerateEllipse(float a, float b, float x0, float h, int n){
    float** points = malloc(sizeof(float*) * n);
    float x = x0, y;
    // For loop to calculate points using the ellipse equation: y = sqrt(b^2 * (1 - x^2/a^2))
    for (int i = 0; i < n; i++) {
        points[i] = (float*)malloc(sizeof(float) * 2);
        points[i][0] = x;
        points[i][1] = b * sqrt(1 - (x * x) / (a * a));
        x += h;
    }
    return points;
}

// Function to calculate the area of the bounded region
float area(float a, float b, float x1, float x2, int n){
    float area_ellipse = 0, area_line = 0;
    float h = (x2 - x1) / (float)n;
    float x;
    for (x = x1 + h; x <= x2; x += h) {
        // Using trapezoidal rule for area under the curve
        area_ellipse += h * 0.5 * (b * sqrt(1 - (x * x) / (a * a)) + b * sqrt(1 - ((x - h) * (x - h)) / (a * a)));
        area_line += h * 0.5 * ((b - (b/a) * x) + (b - (b/a) * (x - h)));
    }
    return fabs(area_ellipse - area_line); // Returning the absolute difference
}

// Function to free the array of coordinates after use
void freeMemory(float **points, int n) {
    for (int i = 0; i < n; i++) {
        free(points[i]);
    }
    free(points);
}

