#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>

// Function to generate a random Bernoulli variable with probability p
int random_number(double p) {
    unsigned char random_byte;
    if (RAND_bytes(&random_byte, 1) != 1) {
        fprintf(stderr, "Error generating random byte\n");
        exit(1);
    }
    double random_value = random_byte / 255.0;
    return random_value > p ? 0 : 1;
}

// Function to estimate P(B') using Monte Carlo simulation
float estimate_probability_complement(float p_b, int n) {
    int count_B = 0;

    for (int i = 0; i < n; i++) {
        int X_B = random_number(p_b);  // Indicator for event B
        count_B += X_B;  // Count occurrences of event B
    }

    return 1.0 - ((float)count_B / n);  // P(B') = 1 - P(B)
}

