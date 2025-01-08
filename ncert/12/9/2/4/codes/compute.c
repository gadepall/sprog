#include <math.h>

void compute(int n, double h, double *x, double *y) {
    for (int i = 0; i < n; i++) {
        x[i+1] = x[i] + h;
        y[i+1] = y[i] + h * x[i] / sqrt(1 + x[i] * x[i]);
    }
}

