#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void simulate_dice_rolls(int trials, double *probs) {
    int counts[7] = {0}; // Include an extra slot for >6
    int roll, i;

    // Seed the random number generator
    srand(time(NULL));

    // Simulate dice rolls
    for (i = 0; i < trials; i++) {
        roll = (rand() % 6) + 1; // Generate numbers between 1 and 6
        if (roll > 6) {
            counts[6]++; // Impossible, but keep for simulation consistency
        } else {
            counts[roll - 1]++;
        }
    }

    // Calculate probabilities
    for (i = 0; i < 7; i++) {
        probs[i] = (double)counts[i] / trials;
    }
}

// Exported function for Python
__attribute__((visibility("default"))) 
__attribute__((used)) 
void get_cdf(int trials, double *output_probs) {
    simulate_dice_rolls(trials, output_probs);
}

