#include<stdio.h>

void solve_ode(double h, double y[]) {
    double x = 0.0;

    for (int i = 1; i <= 51; i++) {
        // Compute the second derivative ddy
        double ddy = -x * y[i - 1];

        // Update y using Euler's method
        y[i] = y[i - 1] + h * ddy;

        // Increment x
        x += h;
    }
}

