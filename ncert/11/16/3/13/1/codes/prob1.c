#include <stdio.h>
#include <stdlib.h>

#define NUM_TRIALS 100000  // Number of simulations

// Function to calculate union probabilities
float* union_probability(float p_A, float p_B, float p_AB, int trials) {
    int count_A = 0, count_B = 0, count_AB = 0, count_A_or_B = 0;

    // Allocate memory for the result
    float *result = (float *)malloc(4 * sizeof(float));  // 4 values: p(A), p(B), p(AB), p(A + B)

    for (int i = 0; i < trials; i++) {
        int X = (rand() / (double)RAND_MAX) < p_A ? 1 : 0;
        int Y = (rand() / (double)RAND_MAX) < p_B ? 1 : 0;
        int Z = (rand() / (double)RAND_MAX) < p_AB ? 1 : 0;

        count_A += X;
        count_B += Y;
        count_AB += Z;
        count_A_or_B += (X + Y - Z);
    }

    // Computing empirical probabilities
    result[0] = (float)count_A / trials;          // Pr(A)
    result[1] = (float)count_B / trials;          // Pr(B)
    result[2] = (float)count_AB / trials;         // Pr(AB)
    result[3] = (float)count_A_or_B / trials;     // Pr(A + B)

    return result;
}

// freeing the allocated memory
void freeMemory(float* ptr) {
    free(ptr);
}

