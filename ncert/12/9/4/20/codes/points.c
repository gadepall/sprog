#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    double* t;
    double* P;
} points;

void generate_points(points* point, double P_0, double t_0, double t_end, double h)
{
    int size = (int)((t_end - t_0) / h) + 1;
    point->t = malloc(size * sizeof(double));
    point->P = malloc(size * sizeof(double));
    if (point->t == NULL || point->P == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    point->t[0] = t_0;
    point->P[0] = P_0;

    for (int i = 1; i < size; i++)
    {
        point->t[i] = point->t[i - 1] + h;
        double slope = point->P[i - 1] * 6.931 / 100;
        point->P[i] = point->P[i - 1] + h * slope;
    }
}

void free_points(points* point)
{
    free(point->t);
    free(point->P);
}

