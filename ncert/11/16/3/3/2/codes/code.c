#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void simulate_dice_rolls(int trials, double *probs) {
    int counts[7] = {0}; // Track counts for 1, 2, 3, 4, 5, 6, and >3
    int roll, i;

    // Seed the random number generator
    srand(time(NULL));

    // Simulate dice rolls
    for (i = 0; i < trials; i++) {
        roll = (rand() % 6) + 1; // Generate numbers between 1 and 6
        counts[roll - 1]++; // Increment the count for the rolled value
    }

    // Calculate probabilities for each outcome and for >3
    for (i = 0; i < 6; i++) {
        probs[i] = (double)counts[i] / trials; // Probabilities for 1, 2, 3, 4, 5, 6
    }

    // Probability for >3 (4, 5, 6)
    probs[6] = (double)(counts[3] + counts[4] + counts[5]) / trials;
}

// Exported function for Python
__attribute__((visibility("default"))) 
__attribute__((used)) 
void get_cdf(int trials, double *output_probs) {
    simulate_dice_rolls(trials, output_probs);
}
