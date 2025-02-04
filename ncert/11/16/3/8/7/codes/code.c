#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void simulate_coin_tosses(int num_trials, int *results) {
    srand(time(NULL)); // Initialize random seed

    for (int i = 0; i < num_trials; i++) {
        int tails = 0;

        // Toss three coins
        for (int j = 0; j < 3; j++) {
            if (rand() % 2 == 0) {
                tails++; // Increment for each tail
            }
        }

        // Increment the result for this outcome
        results[tails]++;
    }
}

