#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void simulate_dice_rolls(int trials, double *probs) {
    int counts[8] = {0}; // Track counts for 1, 2, 3, 4, 5, 6, and B ( >7 is impossible)
    int roll, i;

    // Seed the random number generator
    srand(time(NULL));

    // Simulate dice rolls
    for (i = 0; i < trials; i++) {
        roll = (rand() % 6) + 1; // Generate numbers between 1 and 6
        counts[roll - 1]++; // Increment the count for the rolled value
    }

    // Calculate probabilities for each outcome
    for (i = 0; i < 6; i++) {
        probs[i] = (double)counts[i] / trials; // Probabilities for 1, 2, 3, 4, 5, 6
    }

    // Probability for event B (greater than 7) is always 0
    probs[6] = 0.0;
}

// Exported function for Python
__attribute__((visibility("default"))) 
__attribute__((used)) 
void get_cdf(int trials, double *output_probs) {
    simulate_dice_rolls(trials, output_probs);
}

