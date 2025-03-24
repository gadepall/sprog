#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_TRIALS 100000  // Number of experiments
#define P 0.5               // Probability of heads

// Function to generate number of heads in 3 coin tosses
void simulate_bernoulli(int results[], int num_trials) {
    for (int i = 0; i < num_trials; i++) {
        int count = 0;
        for (int j = 0; j < 3; j++) {
            double rand_val = (double)rand() / RAND_MAX; // Normalize to [0,1)
            if (rand_val < P) {
                count++;  // Count heads
            }
        }
        results[i] = count; // Store result
    }
}

// Export function for Python
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

EXPORT void run_simulation(int* results, int num_trials) {
    srand(time(NULL));  // Seed the random number generator
    simulate_bernoulli(results, num_trials);
}
