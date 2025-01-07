#include <stdlib.h>
#include <math.h>

// Get 'n' points on the plot of the differential equation, 'h' being the step size, 'x', 'y1', 'y2' being the initial conditions
// y1 = y, y2 = y'
float **diffEqPoints(int n, float h, float x, float y1, float y2) {
    float **pts = (float **)malloc(sizeof(float *) * n);

    // Iteratively use Euler's method with given parameters to return 'n' points in the plot of the differential equation
    for (int i = 0; i < n; i++) {
        pts[i] = (float *)malloc(sizeof(float) * 2);

        // Compute y2' only if x != 0
        float y2_new = y2;
        if (x != 0) {
            y2_new = y2 + h * ((-2 * y2 + x * y1 - x * x + 2) / x);
        }
        y1 = y1 + h * y2;
        x = x + h;

        pts[i][0] = x;
        pts[i][1] = y1;
        y2 = y2_new;
    }

    return pts;
}

// Free a 2-dimensional array 'points' with 'n' rows in memory
void freeMultiMem(float **points, int n) {
    for (int i = 0; i < n; i++) {
        free(points[i]);
    }
    free(points);
}

