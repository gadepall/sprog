#include <stdlib.h>
#include <time.h>

// Function to generate a Bernoulli random variable
int bernoulli_random(double p) {
    double rand_val = (double)rand() / RAND_MAX; // Generate random number in [0, 1)
    return rand_val < p ? 1 : 0;
}

