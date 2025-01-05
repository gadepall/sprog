#include <stdlib.h>

typedef struct {
    double *x;
    double *y;
    int size;
} Points;

Points generate_points(double y0, double h, int steps) {
    Points points;
    points.x = (double *)malloc(steps * sizeof(double));
    points.y = (double *)malloc(steps * sizeof(double));
    points.size = steps;

    points.x[0] = 1.0;  // Initial x value
    points.y[0] = y0;   // Initial y value

    for (int i = 1; i < steps; i++) {
        points.x[i] = points.x[i - 1] + h;  // Increment x
        points.y[i] = points.y[i - 1] + h * (-points.y[i - 1] * points.y[i - 1] * points.y[i - 1] * points.y[i - 1] * points.y[i - 1] /
        (points.x[i - 1] * points.x[i - 1] * points.x[i - 1] * points.x[i - 1] * points.x[i - 1]));
    }

    return points;
}

