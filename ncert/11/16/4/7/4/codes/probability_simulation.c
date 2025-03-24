#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRIALS 1000000  // Number of Monte Carlo simulations

int main() {
    // Given probabilities
    double P_A = 0.54;
    double P_B = 0.69;
    double P_A_inter_B = 0.35;
    
    // Compute conditional probability P(B | A)
    double P_B_given_A = P_A_inter_B / P_A;

    // Compute P(A^c)
    double P_Ac = 1 - P_A;

    // Compute P(B | A^c) using Law of Total Probability
    double P_B_given_Ac = (P_B - P_A_inter_B) / P_Ac;

    // Monte Carlo Simulation Variables
    int count_B_inter_Ac = 0;

    // Seed the random number generator
    srand(time(NULL));

    for (int i = 0; i < TRIALS; i++) {
        // Simulate event A
        double rand_A = (double)rand() / RAND_MAX;
        int A_occurs = (rand_A < P_A) ? 1 : 0;

        // Simulate event B based on whether A occurred or not
        double rand_B = (double)rand() / RAND_MAX;
        int B_occurs;

        if (A_occurs) {
            B_occurs = (rand_B < P_B_given_A) ? 1 : 0;
        } else {
            B_occurs = (rand_B < P_B_given_Ac) ? 1 : 0;
        }

        // Count cases where B occurs but A does not (B ∩ A^c)
        if (B_occurs && !A_occurs) {
            count_B_inter_Ac++;
        }
    }

    // Compute probability estimate
    double P_B_inter_Ac = (double)count_B_inter_Ac / TRIALS;

    // Print the result
    printf("Estimated P(B ∩ A^c) using Monte Carlo: %.4f\n", P_B_inter_Ac);

    return 0;
}
