#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_NUMBERS 20 // Total numbers in the lottery
#define CHOICES 6        // Numbers chosen by the player

// Function to generate a unique set of numbers
void generate_unique_numbers(int* numbers, int num_choices) {
    int i, j;
    for (i = 0; i < num_choices; i++) {
        numbers[i] = 1 + (int)(rand() % TOTAL_NUMBERS);
        // Ensure the generated number is unique
        for (j = 0; j < i; j++) {
            if (numbers[i] == numbers[j]) {
                i--; // Regenerate this number
                break;
            }
        }
    }
}

// Function to simulate the lottery and track relative frequency
void simulate_lottery(int simulations, int interval, double* results) {
    int win_count = 0;

    // Seed the random number generator
    srand(time(NULL));

    for (int i = 0; i < simulations; i++) {
        int fixed_numbers[CHOICES], chosen_numbers[CHOICES];
        int is_match = 1;

        // Generate fixed lottery numbers
        generate_unique_numbers(fixed_numbers, CHOICES);

        // Sort the fixed numbers
        for (int j = 0; j < CHOICES - 1; j++) {
            for (int k = j + 1; k < CHOICES; k++) {
                if (fixed_numbers[j] > fixed_numbers[k]) {
                    int temp = fixed_numbers[j];
                    fixed_numbers[j] = fixed_numbers[k];
                    fixed_numbers[k] = temp;
                }
            }
        }

        // Generate player's chosen numbers
        generate_unique_numbers(chosen_numbers, CHOICES);

        // Sort the chosen numbers
        for (int j = 0; j < CHOICES - 1; j++) {
            for (int k = j + 1; k < CHOICES; k++) {
                if (chosen_numbers[j] > chosen_numbers[k]) {
                    int temp = chosen_numbers[j];
                    chosen_numbers[j] = chosen_numbers[k];
                    chosen_numbers[k] = temp;
                }
            }
        }

        // Check for match
        for (int j = 0; j < CHOICES; j++) {
            if (fixed_numbers[j] != chosen_numbers[j]) {
                is_match = 0;
                break;
            }
        }

        if (is_match) {
            win_count++;
        }

        // Record relative frequency at intervals
        if ((i + 1) % interval == 0) {
            results[i / interval] = (double)win_count / (i + 1);
        }
    }
}

int main() {
    int simulations = 100000; // Example number of simulations
    int interval = 1000;      // Interval for frequency tracking
    int result_size = simulations / interval;
    double* results = (double*)malloc(result_size * sizeof(double));

    simulate_lottery(simulations, interval, results);

    // Output the results
    for (int i = 0; i < result_size; i++) {
        printf("Interval %d: Relative Frequency = %.6f\n", i + 1, results[i]);
    }

    free(results);
    return 0;
}

