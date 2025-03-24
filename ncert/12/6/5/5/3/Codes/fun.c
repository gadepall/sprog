#include <stdio.h>
#include <math.h>

// Function to calculate f(x) = 4x - x^2 / 2
double f(double x) {
    return 4 * x - (x * x) / 2;
}

// Derivative of f(x), f'(x) = 4 - x
double f_prime(double x) {
    return 4 - x;
}

// Gradient descent to find the minimum
double gradient_descent(double start, double learning_rate, int max_iter, double tolerance) {
    double x = start;
    for (int i = 0; i < max_iter; i++) {
        double grad = f_prime(x);
        if (fabs(grad) < tolerance) {
            break;
        }
        x -= learning_rate * grad;
        if (x < -2 || x > 4.5) {
            // Constrain within the interval (-2, 4.5)
            
            if (x < -2)  return -2;
            if (x > 4.5) return 4.5;
            break;
        }
    }
    return x;
}

// Gradient ascent to find the maximum
double gradient_ascent(double start, double learning_rate, int max_iter, double tolerance) {
    double x = start;
    for (int i = 0; i < max_iter; i++) {
        double grad = f_prime(x);
        if (fabs(grad) < tolerance) {
            break;
        }
        x += learning_rate * grad;
        if (x < -2 || x > 4.5) {
            // Constrain within the interval (-2, 4.5)
            break;
        }
    }
    return x;
}
int main (){
printf("%lf",gradient_descent(0,0.01,1000,1e-6));
}
