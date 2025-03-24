#include <stdio.h>
#include <math.h>

double newton_raphson(double x0, int *iterations) {
    double x = x0, xprev;
    int i = 0;
    *iterations = 0;

    do {
        xprev = x;
        x = x - (4 * x * x - 35 * x - 375) / (8 * x - 35);
        i++;
    } while (fabs(x - xprev) >= 1e-6 && i < 1000);

    *iterations = i;
    return x;
}

void generate_points(double x_min, double x_max, double step, double *x_vals, double *y_vals, int *n_points) {
    int i = 0;
    for (double x = x_min; x <= x_max; x += step) {
        x_vals[i] = x;
        y_vals[i] = 4 * x * x - 35 * x - 375;
        i++;
    }
    *n_points = i;
}

