// gradient_ascent.c
#include <stdio.h>

double gradient(double x) {
    return 24 - 2 * x; // Derivative of 24x - x^2
}

double gradient_ascent(double start_x, double learning_rate, int max_iterations) {
    double x = start_x;
    for (int i = 0; i < max_iterations; i++) {
        double grad = gradient(x);
        x += learning_rate * grad;
        if (grad < 1e-6 && grad > -1e-6) // Convergence condition
            break;
    }
    return x;
}

int main() {
    double start_x = 0.0;
    double learning_rate = 0.1;
    int max_iterations = 1000;
    double maxima = gradient_ascent(start_x, learning_rate, max_iterations);

    printf("Maxima is at x = %f\n", maxima);
    return 0;
}

