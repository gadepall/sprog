#include <stdlib.h>
#include <math.h>

// get 'n' points on the plot of the differential equation, 'h' being the step size, 'x', 'y1', 'y2', 'y3', 'y4' being the initial conditions
// y1 = y, y2 = y', y3 = y'', y4 = y'''
float **diffEqPoints(int n, float h, float x, float y1, float y2, float y3, float y4){
    float **pts = (float **) malloc(sizeof(float *) * n);

    // iteratively use euler's method with given parameters to return 'n' points in the plot of the differential equation
    for (int i = 0; i < n; i++){
        pts[i] = (float *) malloc(sizeof(float) * 2);

        // Calculate the new values of y5 (the fourth derivative), update y4 (third derivative), and so on
        float y5_new = sin(y4)/* Calculate the fourth derivative here (e.g., based on the specific ODE) */;
        float y4_new = y4 + h * y5_new;
        float y3_new = y3 + h * y4;
        float y2_new = y2 + h * y3;
        y1 = y1 + h * y2;

        // Update values
        y4 = y4_new;
        y3 = y3_new;
        y2 = y2_new;
        y1 = y1;  // We already updated y1 in the previous line
        x = x + h;

        // Store the point (x, y1) for the plot
        pts[i][0] = x;
        pts[i][1] = y1;
    }

    return pts;
}

// free a 2-dimensional array 'points' with 'n' rows in memory
void freeMultiMem(float **points, int n){
    for(int i = 0; i < n; i++){
        free(points[i]);
    }

    free(points);
}

