#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to simulate the coin toss (1 for heads, 0 for tails)
int toss_coin() {
    return rand() % 2; // Generates either 0 or 1
}

// Function to simulate rolling a die (1 to 6)
int roll_die() {
    return (rand() % 6) + 1; // Generates a number between 1 and 6
}

// Function to run the simulation and return the PMF
void simulate(int num_trials, int* counts) {
    for (int i = 0; i < num_trials; i++) {
        int coin = toss_coin();
        int die = roll_die();
        int sum = coin + die;
        counts[sum - 1]++; // Increment the count for the corresponding sum
    }
}

