#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_TRIALS 100000  // Number of experiments

// Function to generate the sum of two die rolls
void simulate_dice_rolls(int results[], int num_trials) {
    srand(time(NULL)); // Seed the random number generator

    for (int i = 0; i < num_trials; i++) {
        int die1 = (rand() % 6) + 1; // Generate number between 1 and 6
        int die2 = (rand() % 6) + 1;
        results[i] = die1 + die2; // Store the sum of two dice
    }
}

// Export function for Python
#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

EXPORT void run_simulation(int* results, int num_trials) {
    if (num_trials <= 0) {
        printf("Error: Number of trials must be positive.\n");
        return;
    }
    simulate_dice_rolls(results, num_trials);
}

