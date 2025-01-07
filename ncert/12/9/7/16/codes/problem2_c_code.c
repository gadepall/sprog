#include <math.h>

void compute_points(double *x_values, double *y_values, int num_points, double h) {
    double x = 1;
    double y = 1;  // Initial value of y = 0
    
    for (int i = 0; i < num_points; i++) {
        x_values[i] = x;
        y_values[i] = y;
        
        // Compute dy/dx = y/x
        double dy_dx = y/x;
        y += h * dy_dx;  // Update y
        x += h;          // Update x
    }
}
