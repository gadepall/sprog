#include <stdlib.h>
void gen_line_points(double slope, double c, int num_points, double *points){
    double left_lim = 10;
    double right_lim = 60;
    double h = (right_lim - left_lim)/num_points;
    for (int i = 0; i < num_points; i++){
        points[2*i] = left_lim;
        points[2*i + 1] = (slope * left_lim) + c;
        
        left_lim += h;
    }
}



void free_ptr(double *points){
    free(points);
}
