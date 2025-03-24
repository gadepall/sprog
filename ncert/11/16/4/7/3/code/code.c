#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <openssl/rand.h>

/* Function to generate a random number based on probability */
int random_number(double p) {
    unsigned char random_byte;
    if (RAND_bytes(&random_byte, 1) != 1) {
        fprintf(stderr, "Error generating random byte\n");
        exit(1);
    }
    double random_value = random_byte / 255.0;
    return random_value < p ? 1 : 0;
}

/* Function to compute P(A ∩ B') */
float compute_probability(float p_a, float p_b, float p_a_p_b, int n) {
    int count_a = 0, count_a_and_b = 0;

    for (int i = 0; i < n; i++) {
        int A = random_number(p_a);
        int A_and_B = random_number(p_a_p_b);

        count_a += A;
        count_a_and_b += A_and_B;
    }

    // P(A ∩ B') = P(A) - P(A ∩ B)
    return ((float)count_a / n) - ((float)count_a_and_b / n);
}

