#include <stdlib.h>
#include <math.h>

typedef struct {
    double *x;
    double *y;
    int size;
} Points;

Points generate_points(double x0, double y0, double h, int steps) {
    Points points;
    points.x = (double *)malloc(steps * sizeof(double));
    points.y = (double *)malloc(steps * sizeof(double));
    points.size = steps;
    points.x[0] = x0;
    points.y[0] = y0;
    for (int i = 1; i < steps; i++) {
        points.x[i] = points.x[i - 1] + h;
        if (points.x[i] >= log(2)) {
            points.size = i;
            break;
        }
        points.y[i] = points.y[i - 1] + h * exp(points.x[i - 1] + points.y[i - 1]);
    }
    return points;
}

void free_points(Points points) {
    free(points.x);
    free(points.y);
}
