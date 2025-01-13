#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to compute sine values, store in arrays, and calculate the area using trapezoidal rule
double compute_sine(double *x, double *y, int n) {
    double h = (2*M_PI) / (n - 1); // Step size
    double area = 0.0;//initializing area to 0

    for (int i = 0; i < n; i++) {
        x[i] = i * h;         // x values from 0 to pi
        y[i] = sin(x[i]);     // y = sin(x)

        // Trapezoidal rule for area calculation
        if (i > 0&&i<(n/2)) {
            area += (y[i] + y[i - 1]) * h / 2.0;
        }
    }
    return 2*area;//we multiply the obtained area by 2 to get the area under the graph from 0 to 2pi

}

int main() {
    int n = 1000; // Number of intervals
    double *x = (double *)malloc(n * sizeof(double));
    double *y = (double *)malloc(n * sizeof(double));

    if (x == NULL || y == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    double area =2* compute_sine(x, y, n); //we multiply the obtained area by 2 to get the area under the graph from 0 to 2pi

    // Print the area for verification
    printf("Computed Area: %lf\n", area);

    

    // Free allocated memory
    free(x);
    free(y);

    return 0;
}

