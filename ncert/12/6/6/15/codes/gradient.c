#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_POINTS 100000

// Function to compute the volume of the cone
float computeVolume(float h, float r) {
    float x = sqrtf(2.0f*r*h - h*h);
    return (1.0f/3.0f) * M_PI * x * x * h;
}

// Function to compute the derivative of the volume with respect to h
float dV_dh(float h, float r) {
    return (1.0f/3.0f) * M_PI * (4.0f*h*r - 3.0f*h*h);
}

// Gradient ascent function with point tracking
float gradientAscent(float* h, float alpha, float* gradient, int max_iter, float r, 
                     float* height_points, float* volume_points, int* point_count) {
    *point_count = 0;
    
    for (int i = 0; i < max_iter; i++) {
        gradient[0] = dV_dh(h[0], r);
        
        // Store current point
        height_points[*point_count] = h[0];
        volume_points[*point_count] = computeVolume(h[0], r);
        (*point_count)++;
        
        h[0] = h[0] + alpha * gradient[0];
        
        // Convergence and boundary checks
        if (fabs(gradient[0] / (dV_dh(1.0f,r)+1e-10)) < 1e-6) {
            return h[0];
        }
        if (h[0] > 2*r || h[0] < 0)
            return h[0];
    }
    return h[0];
}
