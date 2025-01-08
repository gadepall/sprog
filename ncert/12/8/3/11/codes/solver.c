#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function prototypes
double given_function(double x);
double derivative_of_func(double x);
double area(double (*given_function)(double), double (*derivative_of_func)(double), double lower_limit, double upper_limit, double stepsize);
double* getpoints(double (*given_function)(double), double lower_limit, double upper_limit, double stepsize);

// Implementation of the given function and its derivative
double given_function(double x) {
    return 1 - x;
}

double derivative_of_func(double x) {
    return -1;
}

// Area calculation using the trapezoidal method
double area(double (*given_function)(double), double (*derivative_of_func)(double), double lower_limit, double upper_limit, double stepsize) {
    double total_area = 0;
    for (double i = lower_limit; i < upper_limit; i += stepsize) {
        double y1 = given_function(i);
        double y2 = given_function(i + stepsize);
        total_area += 0.5 * stepsize * (y1 + y2);
    }
    return total_area;
}

// Get points for plotting
double* getpoints(double (*given_function)(double), double lower_limit, double upper_limit, double stepsize) {
    int no_points = (int)((upper_limit - lower_limit) / stepsize) + 1;
    double* points = malloc(no_points * sizeof(double));
    for (int i = 0; i < no_points; i++) {
        double x = lower_limit + i * stepsize;
        points[i] = given_function(x);
    }
    return points;
}

int main() {
    double lower_limit = 0, upper_limit = 1, stepsize = 0.01;
    int no_points = (int)((upper_limit - lower_limit) / stepsize) + 1;

    // Calculate area
    double total_area = area(given_function, derivative_of_func, lower_limit, upper_limit, stepsize);
    printf("Approximated area under the curve: %lf\n", total_area);

    // Get points
    double* points = getpoints(given_function, lower_limit, upper_limit, stepsize);

    // Print points for visualization
    printf("Points:\n");
    for (int i = 0; i < no_points; i++) {
        printf("x = %lf, y = %lf\n", lower_limit + i * stepsize, points[i]);
    }

    free(points);
    return 0;
}

