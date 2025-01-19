#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to simulate the random selection of a council member
void simulate_selection(int trials, int *results) {
    for (int i = 0; i < trials; i++) {
        int selection = rand() % 10; // Random selection among 10 members
        if (selection < 6) {
            results[0]++; // Increment count for women (indices 0-5)
        } else {
            results[1]++; // Increment count for men (indices 6-9)
        }
    }
}

// Main function to calculate the probability of selecting a woman
int main() {
    srand(time(0)); // Seed the random number generator
    int trials = 1000000; // Number of trials
    int results[2] = {0, 0}; // Array to store counts for women and men

    // Simulate the random selection process
    simulate_selection(trials, results);

    // Calculate and display the probability
    double probability_woman = (double)results[0] / trials;
    printf("Estimated Probability of selecting a woman: %f\n", probability_woman);

    // Optional: Display individual counts for women and men
    printf("Women selected: %d times.\n", results[0]);
    printf("Men selected: %d times.\n", results[1]);

    return 0;
}

