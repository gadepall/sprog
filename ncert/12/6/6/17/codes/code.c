#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Function to compute the volume of a cylinder given its height h and radius R of the sphere
float computeVolume(float h, float R) {
    float r = sqrt(R * R - (h * h) / 4);
    return M_PI * r * r * h;
}

// Function to compute the derivative of the volume with respect to h
float dV_dh(float h, float R) {
    return M_PI * (R * R - (3 * h * h) / 4);
}

// Function to compute the gradient ascent using matrices (here, the matrix is just a simple array to hold values)
float gradientAscent(float* h, float alpha, float* gradient, int max_iter, float R) {
    for (int i = 0; i < max_iter; i++) {
        gradient[0] = dV_dh(h[0], R); // Gradient calculation at h[0]
        h[0] = h[0] + alpha * gradient[0];  // Update height based on gradient ascent
        if (fabs(gradient[0]) < 1e-6) {
            return h[0];  // If the change is below the tolerance, break out
        }
    }
    return h[0];  // Return the final value of h after max iterations
}

int main() {
    float R = 1.0;         // Radius of the sphere
    float alpha = 0.001;   // Learning rate (alpha)
    float h0 = 1.0;        // Initial guess for height
    int max_iter = 100000; // Maximum number of iterations
    float gradient[1];     // Matrix for storing gradient (1x1)
    float h[1] = {h0};     // Matrix for height (1x1)
    
    // Run gradient ascent
    float optimalHeight = gradientAscent(h, alpha, gradient, max_iter, R);

    // Compute the corresponding volume at optimal height
    float optimalVolume = computeVolume(optimalHeight, R);

    printf("Optimal height (h) = %.6f\n", optimalHeight);
    printf("Maximum volume = %.6f\n", optimalVolume);

    return 0;
}

