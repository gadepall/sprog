#include <stdio.h>
#include <math.h>

// Function to compute the car speeds based on the given equations
void compute_car_speeds(double *v1, double *v2) {
    // Equation 1: v1 - v2 = 20
    // Equation 2: v1 + v2 = 100
    // Solving the system of equations gives v1 = 60 and v2 = 40
    
    *v1 = 60; // Speed of the car starting from A
    *v2 = 40; // Speed of the car starting from B
}

// This function is needed for the shared object
void get_car_speeds(double *v1, double *v2) {
    compute_car_speeds(v1, v2);
}

