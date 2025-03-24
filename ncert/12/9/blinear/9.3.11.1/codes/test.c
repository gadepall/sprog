#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"

// Let the differential equation be d2y/dx2 = -y, with initial conditions y(0) = 1, y'(0) = 0

double* recorddata_trapezoid(double lowerbound, double upperbound, double stepsize, double initialy0, double initialyprime0) {
    int no_datapoints = (int)((upperbound - lowerbound) / stepsize) + 1;
    double* y_values = malloc(no_datapoints * sizeof(double));
    double y_prev = initialy0;  // y(n)
    double y_curr = initialy0 + stepsize * initialyprime0;  // y(n+1), derived using Taylor expansion

    y_values[0] = y_prev;
    y_values[1] = y_curr;

    for (int i = 1; i < no_datapoints - 1; i++) {
        double y_next = (2 * y_curr - y_prev - stepsize * stepsize * y_curr) / (1 + stepsize * stepsize);
        y_values[i + 1] = y_next;
        y_prev = y_curr;
        y_curr = y_next;
    }
    return y_values;
}

double* recorddata_ztransform(double lowerbound, double upperbound, double stepsize, double initialy0, double initialyprime0) {
    int no_datapoints = (int)((upperbound - lowerbound) / stepsize) + 1;
    double* y_values = malloc(no_datapoints * sizeof(double));

    y_values[0] = initialy0;
    y_values[1] = initialy0 + stepsize * initialyprime0;  // Using Taylor expansion for y'(0)

    double alpha = (stepsize * stepsize) / (2 + stepsize * stepsize);  // Derived from Z-transform equations
    for (int i = 1; i < no_datapoints - 1; i++) {
        y_values[i + 1] = (2 - 2 * alpha) * y_values[i] - y_values[i - 1];
    }
    return y_values;
}

