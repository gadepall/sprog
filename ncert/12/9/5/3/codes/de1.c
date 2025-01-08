#include <stdio.h>
#include <stdlib.h>

// Function to compute the finite difference method and fill the x and y arrays
void finite_difference(double *x, double *y, int n) {
    double xo =1, yo =0.25;  // Initial conditions
    double h=1e-3;  // Infinitesimally small number for finite difference

    // Calculate the points using the finite difference method
    for (int i=0;i<n;i++) 
    {
        double d = xo *xo-yo/xo; 
        // The differential equation: dy/dx = x^2 - y/x
        // Next y value
        double y1 = yo+d*h;  
        // Next x value
        double x1 = xo + h;            
        // Store x value in the array
        x[i] = x1;  
        // Store y value in the array
        y[i] = y1;  
        // Update xo for next iteration
        xo = x1;    
        // Update yo for next iteration
        yo = y1;    
    }
}

