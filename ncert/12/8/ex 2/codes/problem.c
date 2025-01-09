#include <stdlib.h>
#include <math.h>
//function to generate points on a standard ellipse x^2/a^2 + y^/b^2 = 1
void generate_points(double a, double b, double *points, int n) {
    double h = (2*a) / n;
    double x_0 = -a;
    for (int i = 0; i < n; i++) {
        points[4*i] = x_0;
        points[4*i+2] = x_0;
        points[4*i+1] = (b/a) * sqrt(a*a - x_0*x_0);
        points[4*i+3] = -(b/a) * sqrt(a*a - x_0*x_0);
        x_0 += h;
    }  
}

void calc_area(double a, double b, int n, double *res) {
    double h = (2 * a) / n;
    double j_1 = 0;
    double x_0 = -a;
    for (int i = 0; i < n; i++) {
        j_1 += (b *h * (sqrt(a*a - x_0*x_0) + sqrt(a*a - (x_0 + h)*(x_0 + h))));
        x_0 += h;  
    }
    *res = j_1 / 2;  
}

void free_ptr(double *points) {
    free(points);  
}
