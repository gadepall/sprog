#include <math.h>
#include <stdio.h>

// Define the function's derivative
double f_derivative(double x) {
    return 2 * cos(2 * x);//derivative of y=sin(2x) is 2cos(2x)
}

// Gradient Descent for finding minima
double gradient_descent(double x0, double learning_rate, int max_iter, double tol) {
    double x = x0;
    for (int i = 0; i < max_iter; i++) {
        double grad = f_derivative(x);//Initializing the derivative at x to grad
        if (fabs(grad) < tol) {  //checking for the condition at which the absolute value of the slope is less than the tolerance, or very close to 0
            break;
        }
        x -= learning_rate * grad;  // Gradient descent step
    }
    return x;
}

// Gradient Ascent for finding maxima
double gradient_ascent(double x0, double learning_rate, int max_iter, double tol) {
    double x = x0;
    for (int i = 0; i < max_iter; i++) {
        double grad = f_derivative(x);
        if (fabs(grad) < tol) {  // //checking for the condition at which the absolute value of the slope is less than the tolerance, or very close to 0
            break;
        }
        x += learning_rate * grad;  // Gradient ascent step
    }
    return x;
}

