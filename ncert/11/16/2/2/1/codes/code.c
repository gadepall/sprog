#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_REPETITIONS 100000  // Number of trials for simulation
#define SIDES 6  // A fair six-sided die

// Function to simulate rolling a fair die using OpenSSL's RAND_bytes

int roll_die() {
    return (rand() % 6) + 1;  // Generate a number between 1 and 6
}

// Simulate rolling a die multiple times and compute PMF
void sim_die_pmf(double *probability_arr) {
    int counts[SIDES] = {0};

    for (int i = 0; i < NUM_REPETITIONS; i++) {
        int roll = roll_die();
        counts[roll - 1]++;
    }

    for (int i = 0; i < SIDES; i++) {
        probability_arr[i] = (double)counts[i] / NUM_REPETITIONS;
    }
}

// Theoretical PMF of a fair six-sided die
void theoretical_die_pmf(double *probability_arr) {
    for (int i = 0; i < SIDES; i++) {
        probability_arr[i] = 1.0 / SIDES;
    }
}

// Compute cumulative probabilities (CDF) from PMF
void compute_cdf(double *pmf_arr, double *cdf_arr) {
    cdf_arr[0] = pmf_arr[0];
    for (int i = 1; i < SIDES; i++) {
        cdf_arr[i] = cdf_arr[i - 1] + pmf_arr[i];
    }
}

// Main function
int main() {
    double sim_pmf[SIDES], theory_pmf[SIDES];
    double sim_cdf[SIDES], theory_cdf[SIDES];

    sim_die_pmf(sim_pmf);
    compute_cdf(sim_pmf, sim_cdf);
    theoretical_die_pmf(theory_pmf);
    compute_cdf(theory_pmf, theory_cdf);

    return 0;
}
