#include <stdio.h>
#include <stdlib.h>

// Define the number of outcomes and their corresponding values
#define NUM_VALUES 5
double values[NUM_VALUES] = {-6, -3.5, -1, 1.5, 4};
double probabilities[NUM_VALUES] = {0.0625, 0.25, 0.375, 0.25, 0.0625};

// Function to calculate PMF
void get_pmf(double *pmf) {
    for (int i = 0; i < NUM_VALUES; i++) {
        pmf[i] = probabilities[i];
    }
}

// Function to calculate CDF
void get_cdf(double *cdf) {
    double cumulative = 0.0;
    for (int i = 0; i < NUM_VALUES; i++) {
        cumulative += probabilities[i];
        cdf[i] = cumulative;
    }
}

// Function to get the net money values
void get_values(double *output_values) {
    for (int i = 0; i < NUM_VALUES; i++) {
        output_values[i] = values[i];
    }
}

