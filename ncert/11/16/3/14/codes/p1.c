#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // Assign the probability values
    double P_a = 3.0 / 5.0;  // Probability of event A
    double P_b = 1.0 / 5.0;  // Probability of event B
    int total_trials = 100000; // Number of trials
    
    // Variables to count the occurrences of correct events
    int correct_count = 0;

    // Seed the random number generator
    srand(time(NULL));

    // Simulate trials
    for (int i = 0; i < total_trials; i++) {
        // Generate a random number between 0 and 1
        double random_number = (double)rand() / RAND_MAX;

        // Determine the event based on the random number
        if (random_number < P_a) {
            // Event A occurs
            correct_count++;
        } else if (random_number < P_a + P_b) {
            // Event B occurs
            correct_count++;
        }
        // If it's above P_a + P_b, neither event happens, so no count
    }

    // Calculate the probability of correct event prediction
    double probability_correct = (double)correct_count / total_trials;

    // Print the result
    printf("Probability of correct event prediction: %f\n", probability_correct);

    return 0;
}

