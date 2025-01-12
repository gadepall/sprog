#include <stdio.h>
#include <math.h>

double f(double x) {
    return (2 * x )+ 1 / x;
}

void solve_trapezoidal(double x0, double y0, double h, int steps, double *results) {
    double x = x0;
    double y = y0;
    for (int i = 0; i < steps; ++i) {
        results[i] = y;
        double f_n = f(x);
        double f_n1 = f(x + h);
        y += (h / 2.0) * (f_n + f_n1);
        x += h;
    }
}

