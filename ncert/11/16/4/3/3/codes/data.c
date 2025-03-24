#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000 // Number of simulations
#define RAND_MAX_FLOAT ((float)RAND_MAX)

int main() {
    int outcomes[] = {0, 1, 2, 3};  // Possible values of the random variable
    float probs[] = {0.15, 0.35, 0.35, 0.15}; // Theoretical probabilities
    int num_outcomes = sizeof(outcomes) / sizeof(outcomes[0]);

    int count[4] = {0}; // Frequency of each outcome
    float pmf[4], cdf[4];

    srand(time(NULL));

    // Simulation
    for (int i = 0; i < N; i++) {
        float r = (float)rand() / RAND_MAX_FLOAT;
        float cumulative_prob = 0.0;

        for (int j = 0; j < num_outcomes; j++) {
            cumulative_prob += probs[j];
            if (r < cumulative_prob) {
                count[j]++;
                break;
            }
        }
    }

    float cumulative = 0.0;
    for (int j = 0; j < num_outcomes; j++) {
        pmf[j] = (float)count[j] / N;
        cumulative += pmf[j];
        cdf[j] = cumulative;
    }

    FILE *file = fopen("results.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    
    fprintf(file, "x pmf_sim cdf_sim pmf_theory cdf_theory\n");
    float cdf_theory = 0.0;
    for (int j = 0; j < num_outcomes; j++) {
        cdf_theory += probs[j];
        fprintf(file, "%d %.5f %.5f %.5f %.5f\n", outcomes[j], pmf[j], cdf[j], probs[j], cdf_theory);
    }

    fclose(file);
    printf("Simulation data written to results.txt\n");

    return 0;
}

