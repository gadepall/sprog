#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void monte_carlo(int num_trials, double *pmf, double *cdf) {
    srand(time(NULL)); // Seed the random number generator

    int counts[18] = {0};

    for (int i = 0; i < num_trials; i++) {
        int color = rand() % 3; // 0: Red, 1: White, 2: Blue
        int number = (rand() % 6) + 1; // 1-6
        int index = color * 6 + (number - 1);
        counts[index]++;
    }

    // Compute PMF
    for (int i = 0; i < 18; i++) {
        pmf[i] = (double)counts[i] / num_trials;
    }

    // Compute CDF
    cdf[0] = pmf[0];
    for (int i = 1; i < 18; i++) {
        cdf[i] = cdf[i-1] + pmf[i];
    }
}

int main() {
    // Define the number of trials
    int num_trials = 1000000;

    // Arrays to store PMF and CDF
    double pmf[18];
    double cdf[18];

    // Perform Monte Carlo simulation
    monte_carlo(num_trials, pmf, cdf);

    // Print results
    printf("Outcome\tPMF\t\tCDF\n");
    const char *colors[] = {"R", "W", "B"};
    for (int i = 0; i < 18; i++) {
        int color_idx = i / 6;       // Determine the color index (0: R, 1: W, 2: B)
        int number = (i % 6) + 1;    // Determine the number (1 to 6)
        printf("%s%d\t%.6f\t%.6f\n", colors[color_idx], number, pmf[i], cdf[i]);
    }

    return 0;
}
