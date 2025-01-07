#include <math.h>

void compute_points(double *x_values, double *y_values, int num_points, double h) {
    double x = 0.0;
    double y = log(2.0);  // Initial value of y = ln(2)
    
    for (int i = 0; i < num_points; i++) {
        x_values[i] = x;
        y_values[i] = y;
        
        // Compute dy/dx = (e^x - e^-x) / (e^x + e^-x)
        double dy_dx = (exp(x) - exp(-x)) / (exp(x) + exp(-x));
        y += h * dy_dx;  // Update y
        x += h;          // Update x
    }
}
