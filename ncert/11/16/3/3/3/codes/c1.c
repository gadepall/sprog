#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to calculate the PMF of rolling a die
double* die_roll_pmf(int trials) {
    if (trials <= 0) {
        printf("Error: Number of trials must be greater than 0.\n");
        return NULL;
    }

    double* pmf = malloc(6 * sizeof(double));
    if (pmf == NULL) {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }

    // Initialize the PMF array
    for (int i = 0; i < 6; i++) {
        pmf[i] = 0.0;
    }

    srand(time(NULL));

    // Simulate rolling the die
    for (int t = 0; t < trials; t++) {
        int outcome = rand() % 6; // Generates a number between 0 and 5
        pmf[outcome] += 1.0 / trials; // Update the probability for the outcome
    }

    return pmf;
}

void free_pmf(double* pmf) {
    free(pmf);
}

int main() {
    int trials;
    printf("Enter the number of trials for simulating the die roll: ");
    scanf("%d", &trials);

    double* pmf = die_roll_pmf(trials);
    if (pmf != NULL) {
        printf("Simulated PMF of rolling a die:\n");
        for (int i = 0; i < 6; i++) {
            printf("P(X=%d) = %lf\n", i + 1, pmf[i]);
        }
        free_pmf(pmf);
    }

    return 0;
}

