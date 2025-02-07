#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROLLS 100000  // Number of dice rolls for simulation

// Function to roll a fair 6-sided die
int roll_die() {
    return (rand() % 6) + 1;
}

int main() {
    int counts[7] = {0}; // To store counts for 1 to 6 (0th index unused)
    double pmf[7] = {0.0};
    double cdf[7] = {0.0};
    
    srand(time(NULL)); // Seed random number generator

    // Roll the die multiple times
    for (int i = 0; i < ROLLS; i++) {
        int outcome = roll_die();
        counts[outcome]++;
    }

    // Compute PMF
    printf("PMF of a fair 6-sided die:\n");
    for (int i = 1; i <= 6; i++) {
        pmf[i] = (double)counts[i] / ROLLS;
        printf("P(X = %d) = %.4f\n", i, pmf[i]);
    }

    // Compute CDF
    printf("\nCDF of a fair 6-sided die:\n");
    for (int i = 1; i <= 6; i++) {
        cdf[i] = (i == 1) ? pmf[i] : cdf[i-1] + pmf[i];
        printf("F(X ≤ %d) = %.4f\n", i, cdf[i]);
    }

    // Probability of rolling an even number greater than 4 (i.e., rolling a 6)
    printf("\nProbability of rolling an even number greater than 4 (P(X = 6)): %.4f\n", pmf[6]);

    return 0;
}

