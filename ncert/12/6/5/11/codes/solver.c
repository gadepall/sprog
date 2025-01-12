#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void generate_points(double *points, int n){
    double x_0 = -10;
    double h = -(2 * x_0)/n;
    for (int i = 0; i < n; i++){
        points[2*i] = x_0; 
        points[2*i + 1] = x_0 * x_0; 
        x_0 += h;
    }   
}
void run_gradient_descent(double init_guess, double step_size,double (*f)(double), double tolerance, double *minimum){
    int count = 0;
    while(fabs((*f)(init_guess)) > tolerance){
        init_guess -= (*f)(init_guess)*step_size;
        count++;
    } 
        printf("Number of iterations : %d\n", count);
        *minimum = init_guess;
}
void free_ptr(double *points){
    free(points);
}
