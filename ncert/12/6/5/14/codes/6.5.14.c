#include <stdio.h>
#include <math.h>

#define LEARNING_RATE 0.000001  // Smaller learning rate for stability
#define MAX_ITER 1000000       // Maximum iterations
#define EPSILON 1e-6         // Convergence threshold

// Function f(x) = x * (60 - x)^3


double *gradientascent(){
static double coords[2];
    double x = 14.0;  // Initial guess for x
    double grad;      // Gradient of f(x)
    int iter = 0;     // Iteration counter

   

    while (iter < MAX_ITER) {
        grad = pow(60 - x, 3) - 3 * x * pow(60 - x, 2);  // Compute gradient

        // Update x using gradient ascent
        double new_x = x + LEARNING_RATE * grad;

        // Ensure x stays in the valid range [0, 60]
        if (new_x < 0) new_x = 0;
        if (new_x > 60) new_x = 60;

        // Check for convergence
        if (fabs(new_x - x) < EPSILON) {
            x = new_x;
            break;
        }

        x = new_x;  // Update x for next iteration
        iter++;
    }

    // Compute the final y value
    double y = x * pow(60 - x, 3);
coords[0] = x;
coords[1] = y;
    

    return coords;
    }
