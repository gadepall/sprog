#include <stdio.h>

// Function to compute x = (y^2)/4
double func(double y){
    return (y*y)/4.0;
}

// Trapezoidal Rule Function
double Area(double low_lim, double up_lim, int n){
    double h = (up_lim - low_lim)/n; // Step size
    double sum = 0.0;

    // Compute the values at the interval boundaries
    for(int i = 0; i <= n; i++){
        double y = low_lim + i*h;
        double x = func(y);
        // Add the boundary contributions
        if(i == 0 || i == n){
		sum += (h/2.0)*x; // First and last terms
	}else{
		sum += h*x; // Intermediate terms
        }
    }
    return sum;
}
