#include <math.h>


void finite_differences(double *x, double *y, int n, double h) {
    y[0] = 0; // Initial condition: y(0) = 0
    for (int i = 0; i < n - 1; i++) {
        y[i + 1] = y[i] + h * (1 - y[i]);
    }
}

//  solution: y = 1 - e^(-x)
void analytical_solution(double *x, double *y, int n) {
    for (int i = 0; i < n; i++) {
        y[i] = 1 - exp(-x[i]);
    }
}
