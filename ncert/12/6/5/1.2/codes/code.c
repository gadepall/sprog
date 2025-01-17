#include <stdio.h>
#include <math.h>

float df(float x) {
    return (18 * x) + 12;
}

void min(float *x, float *y) {
    float lr = 0.01;  // Adjust learning rate
    double closeness = 1e-6;


    for (int i = 0; i < 1000; i++) {
        *y = 9 * pow(*x, 2) + 12 * (*x) + 2;
        *x = *x - lr * df(*x);

        if (fabs(df(*x)) <= closeness) {
            break;
        }
    }

    }


