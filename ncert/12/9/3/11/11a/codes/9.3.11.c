#include <stdio.h>
#include <math.h>

#define N 100         // Number of steps
#define STEP_SIZE 0.01 // Step size (h)

// Function to compute numerical solution
void compute_numerical(double h, double y[], double c1, double c2) {
    y[0] = c1;               // Initial condition y(0) = C1
    y[1] = c1 + h * c2;      // First step based on initial condition
    for (int i = 2; i <= N; i++) {
        y[i] = 2 * y[i - 1] - y[i - 2] - h * h * y[i - 1];
    }
}

// Function to compute exact solution y(x) = C1*cos(x) + C2*sin(x)
void compute_exact_cos_sin(double h, double y[], double c1, double c2) {
    for (int i = 0; i <= N; i++) {
        double x = i * h;    // x = i * h
        y[i] = c1 * cos(x) + c2 * sin(x);
    }
}

// Function to compute exact solution y(x) = C1*exp(x) + C2*exp(-x)
void compute_exact_exp(double h, double y[], double c1, double c2) {
    for (int i = 0; i <= N; i++) {
        double x = i * h;    // x = i * h
        y[i] = c1 * exp(x) + c2 * exp(-x);
    }
}

int main() {
    double y_numerical[N + 1] = {0}; // Array for numerical solution
    double y_cos_sin[N + 1] = {0};  // Array for exact solution (cos + sin)
    double y_exp[N + 1] = {0};      // Array for exact solution (exp form)
    double h = STEP_SIZE;           // Step size

    // Constants
    double C1 = 1.0;  // Example value for C1
    double C2 = 0.0;  // Example value for C2

    // Compute solutions
    compute_numerical(h, y_numerical, C1, C2);
    compute_exact_cos_sin(h, y_cos_sin, C1, C2);
    compute_exact_exp(h, y_exp, C1, C2);

    // Save results to a file
    FILE *file = fopen("output_data.txt", "w");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return 1;
    }

    fprintf(file, "x,numerical,cos_sin,exp\n");
    for (int i = 0; i <= N; i++) {
        double x = i * h; // x = i * h
        fprintf(file, "%.5f,%.5f,%.5f,%.5f\n", x, y_numerical[i], y_cos_sin[i], y_exp[i]);
    }

    fclose(file);

    printf("Data saved to 'output_data.txt'.\n");
    return 0;
}

