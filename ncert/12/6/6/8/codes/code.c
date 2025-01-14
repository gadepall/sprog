#include <stdio.h>
#include <math.h>

// Function to compute and return the last point (positive recurrence relation)
double compute_last_point(double mu, double ab, double x, int iterations) {
    for (int n = 0; n < iterations; n++) {
        double cos_x = cos(x);
        double sin_x = sin(x);

        // Update x using the given recurrence relation
        x = x + mu * (ab * cos_x * (1 - cos_x) + ab * sin_x * sin_x);
    }
    return x;
}

// Exported function for positive recurrence relation
double get_last_point() {
    double mu = 0.01;   // Set the value of mu
    double ab = 20;     // Set the value of ab
    double x = 0.5;     // Initial value of x
    int iterations = 300;

    return compute_last_point(mu, ab, x, iterations);
}

// Function to compute and return the last point (negative recurrence relation)
double compute_last_point_1(double mu, double ab, double x1, int iterations) {
    for (int n = 0; n < iterations; n++) {
        double cos_x1 = cos(x1);
        double sin_x1 = sin(x1);

        // Update x1 using the given recurrence relation
        x1 = x1 - mu * (ab * cos_x1 * (1 - cos_x1) + ab * sin_x1 * sin_x1);
    }
    return x1;
}

// Exported function for negative recurrence relation
double get_last_point_1() {
    double mu = 0.01;   // Set the value of mu
    double ab = 20;     // Set the value of ab
    double x1 = 0.5;    // Initial value of x1
    int iterations = 300;

    return compute_last_point_1(mu, ab, x1, iterations);
}

// Main function to test the code
int main() {
    double result1 = get_last_point();
    double result2 = get_last_point_1();

    printf("Result for positive recurrence relation: %f\n", result1);
    printf("Result for negative recurrence relation: %f\n", result2);

    return 0;
}

