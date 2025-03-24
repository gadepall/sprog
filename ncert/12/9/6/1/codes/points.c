// function to generate approximate graph for the differential equation
// y' + ay = k 
#include <stdlib.h>
void generate_points(double x_0, double y_0, double h, double *points, double (*f)(double), int n){
    for (int i = 0; i < n; i++){
        points[2*i] = x_0; 
        points[2*i + 1] = y_0; 
        y_0 = y_0 * (1 - 2*h) + h*(*f)(x_0);
        x_0 += h;
    }   
}

void free_ptr(double *points){
    free(points);
}
