#include <stdio.h>
#include <math.h>

double Area(double x1, double x2) {
    double n = 300000;
    double h = fabs(x1 - x2) / n;
    double A = 0;
    double x = fmin(x1, x2);

    for (int i = 0; i < n; i++) {
        double temp = (3 * (x + h) + 2 + 3 * x + 2) / 2; 
        A += fabs(temp) * h;
        x += h; 
    }
    return A;
}