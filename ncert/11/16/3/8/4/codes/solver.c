#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double* binomial_rv(double pval, int no_events) {
    if (pval < 0 || pval > 1) {
        printf("Error: Probability pval must be in the range [0, 1].\n");
        return NULL;
    }
    if (no_events <= 0) {
        printf("Error: Number of events must be greater than 0.\n");
        return NULL;
    }

    double* pmf = malloc((no_events + 1) * sizeof(double));

    // Initialize the PMF array to 0
    for (int i = 0; i <= no_events; i++) {
        pmf[i] = 0.0;
    }

    int trials = (int)(10000/fmin(pval,1-pval));

    srand(time(NULL));

    // Simulate the Binomial random variable
    for (int t = 0; t < trials; t++) {
        int successes = 0;
        for (int i = 0; i < no_events; i++) {
            if ((rand() / (double)RAND_MAX) < pval) {
                successes++;
            }
        }
        pmf[successes] += 1.0 / trials; // Update probability for the number of successes
    }

    return pmf;
}


int main() {
    double pval;
    int no_events;

    printf("Enter the probability of success (pval) for the Binomial random variable (0 <= pval <= 1): ");
    scanf("%lf", &pval);

    printf("Enter the number of trials (no_events) for the Binomial random variable: ");
    scanf("%d", &no_events);

    // Call binomial_rv to calculate PMF
    double* pmf = binomial_rv(pval, no_events);
    if (pmf != NULL) {
        // Print the PMF
        printf("PMF of Binomial random variable:\n");
        for (int k = 0; k <= no_events; k++) {
            printf("P(X=%d) = %lf\n", k, pmf[k]);
        }

        // Free the allocated memory for PMF
        free(pmf);
    }

    return 0;
}
