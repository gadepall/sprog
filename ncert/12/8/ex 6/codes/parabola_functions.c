#include <stdio.h>
#include <math.h>

// Function to generate points for the parabola y = x^2
void generate_parabola_1(double start, double end, double step, double *x_points, double *y_points, int *n_points) {
    int i = 0;
    for (double x = start; x <= end; x += step) {
        x_points[i] = x;
        y_points[i] = x * x;  // y = x^2
        i++;
    }
    *n_points = i;
}

// Function to generate points for the parabola y^2 = x (i.e., y = sqrt(x))
void generate_parabola_2(double start, double end, double step, double *x_points, double *y_points, int *n_points) {
    int i = 0;
    for (double x = start; x <= end; x += step) {
        if (x >= 0) {  // Only valid for x >= 0
            x_points[i] = x;
            y_points[i] = sqrt(x);  // y = sqrt(x)
            i++;
        }
    }
    *n_points = i;
}

// Function to calculate the area between the two parabolas using the trapezoidal rule
double calculate_area(double *x_points1, double *y_points1, double *x_points2, double *y_points2, int n_points) {
    double area = 0.0;
    for (int i = 0; i < n_points - 1; i++) {
        // Trapezoidal area calculation
        double width = x_points1[i+1] - x_points1[i];
        double height = (fabs(y_points1[i] - y_points2[i]) + fabs(y_points1[i+1] - y_points2[i+1])) / 2.0;
        area += width * height;
    }
    return area;
}


