#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <openssl/rand.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function to generate a random indicator (0 or 1) based on probability p */
int random_number(double p) {
    unsigned char random_byte;
    if (RAND_bytes(&random_byte, 1) != 1) {
        fprintf(stderr, "Error generating random byte\n");
        exit(1);
    }
    double random_value = (double)random_byte / 255.0;
    return random_value < p ? 1 : 0;
}

/* Function to compute probabilities using Monte Carlo simulation */
float* compute_probabilities(float p_A, float p_B, int n) {
    static float arr[2];  // Array to store computed probabilities
    int count_A = 0, count_B = 0;

    for (int i = 0; i < n; i++) {
        int A = random_number(p_A);
        int B = random_number(p_B);

        count_A += A;
        count_B += B;
    }

    // Storing computed probabilities
    arr[0] = (float)count_A / n;    // P(A)
    arr[1] = (float)count_B / n;    // P(B)

    return arr;
}

#ifdef __cplusplus
}
#endif

