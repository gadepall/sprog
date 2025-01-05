#include <stdio.h>
#include <math.h>

#define N 10000
#define H 0.001


// Function to calculate sec(x)
double sec(double x) {
    return 1 / cos(x);
}

// Function to calculate the derivative
double derivative(double x, double y) {
    return tan(x) - y * sec(x);
}

// Function to calculate and store x, y values in arrays
void calculate_values(double x_values[], double y_values[]) {
    double x_0 = 0.0, y_0;
    int i;

    y_0 = 1 - x_0 / (sec(x_0) + tan(x_0));
    x_values[0] = x_0;
    y_values[0] = y_0;

    for (i = 1; i < N; i++) {
        x_0 += H;
        if (x_0 >= M_PI / 2) {
            break;
        }
        y_0 += derivative(x_0, y_0) * H;
        x_values[i] = x_0;
        y_values[i] = y_0;
    }
}

