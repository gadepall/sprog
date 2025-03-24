#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int a = 2, b = 3;
double eta = 0.0001;  // Step size

double nextstep(double i) {
    double sin_theta = sin(i);
    double cos_theta = cos(i);

    // Gradient Descent step
    double result = i - eta * ((a * sin_theta / (cos_theta * cos_theta)) + (-b * cos_theta / (sin_theta * sin_theta)));

    // If the result is close enough to the previous value, return 0 (for stopping)
    if (fabs(i - result) > 0.000001) {
        return result;  // Continue the iteration
    } else {
        return 0;  // Stop the iteration
    }
}

