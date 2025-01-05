// function to generate approximate graph for the differential equation
// y' + ay = k 
#include <stdlib.h>
void generate_points(double a, double k, double x_0, double y_0, double slope_0, double h, double *points, int n){
    for (int i = 0; i < n; i++){
        points[2*i] = x_0; 
        points[2*i + 1] = y_0; 
        y_0 = y_0 + slope_0 * h;
        x_0 += h;
        slope_0 = k - a * y_0;
    }   
}

void free_ptr(double *points){
    free(points);
}
