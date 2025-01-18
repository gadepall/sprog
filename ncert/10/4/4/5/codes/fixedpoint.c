#include <stdio.h>
#include <math.h>

// Define the fixed-point iteration function
double g(double l) {
    return 400.0 / (40.0 - l);
}

// Fixed-point iteration function
double fixed_point_iteration(double initial_guess, double tolerance, int max_iter) {
    double l_n = initial_guess;
    double l_n1;
    
    for (int i = 0; i < max_iter; i++) {
        l_n1 = g(l_n);
        
        // Check if the difference is less than the tolerance
        if (fabs(l_n1 - l_n) < tolerance) {
            return l_n1;
        }
        
        l_n = l_n1;  // Update l_n to l_n+1
    }
    
    return l_n1;  // Return the result after max iterations if tolerance is not met
}

