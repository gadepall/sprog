#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to simulate the random selection of a student
void simulate_selection(int trials, int *results) {
    for (int i = 0; i < trials; i++) {
        int selection = rand() % 60; // Random selection among 60 students
        if (selection < 38) {
            results[0]++; // Increment count for students in NCC or NSS
        } else {
            results[1]++; // Increment count for students not in NCC or NSS
        }
    }
}

// Main function to calculate the probability of selecting a student in NCC or NSS
int main() {
    srand(time(0)); // Seed the random number generator
    int trials = 1000000; // Number of trials
    int results[2] = {0, 0}; // Array to store counts for NCC/NSS and not in NCC/NSS

    // Simulate the random selection process
    simulate_selection(trials, results);

    // Calculate and display the probability
    double probability_ncc_or_nss = (double)results[0] / trials;
    printf("Estimated Probability of selecting a student in NCC or NSS: %f\n", probability_ncc_or_nss);

    // Optional: Display individual counts for students in NCC/NSS and not in NCC/NSS
    printf("Students in NCC or NSS selected: %d times.\n", results[0]);
    printf("Students not in NCC or NSS selected: %d times.\n", results[1]);

    return 0;
}

