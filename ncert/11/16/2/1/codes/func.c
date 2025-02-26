#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROLLS 1000000  // Number of rolls
#define SIDES 6        // Number of sides on the die

// Function to calculate PMF and event probabilities
void calculate_probabilities(double *pmf, double *p_e, double *p_f, double *p_e_and_f) {
    int counts[SIDES + 1] = {0}; // Store count for numbers 1-6
    int count_E = 0, count_F = 0, count_E_and_F = 0;

    srand(time(NULL)); // Seed random number generator

    // Simulate die rolls
    for (int i = 0; i < ROLLS; i++) {
        int roll = (rand() % SIDES) + 1; // Roll a die (1 to 6)
        counts[roll]++;

        if (roll == 4) count_E++;      // Event E (die shows 4)
        if (roll % 2 == 0) count_F++;  // Event F (die shows even number)
        if (roll == 4) count_E_and_F++; // Event E ∩ F (die is 4 and even)
    }

    // Calculate PMF
    for (int i = 1; i <= SIDES; i++) {
        pmf[i - 1] = (double)counts[i] / ROLLS;
    }

    // Calculate probabilities
    *p_e = (double)count_E / ROLLS;
    *p_f = (double)count_F / ROLLS;
    *p_e_and_f = (double)count_E_and_F / ROLLS;
}

