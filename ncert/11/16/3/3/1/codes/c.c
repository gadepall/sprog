#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROLLS 1000000  // Number of rolls
#define SIDES 6        // Number of sides on the die

// Function to calculate the PMF of rolling a die
void calculate_pmf(double *pmf) {
    int counts[SIDES + 1] = {0};  // Counts for numbers 1 to 6

    srand(time(NULL));  // Seed random number generator

    // Simulate die rolls
    for (int i = 0; i < ROLLS; i++) {
        int roll = (rand() % SIDES) + 1;  // Roll a die (1 to 6)
        counts[roll]++;
    }

    // Calculate probabilities (PMF)
    for (int i = 1; i <= SIDES; i++) {
        pmf[i - 1] = (double)counts[i] / ROLLS;
    }
}

