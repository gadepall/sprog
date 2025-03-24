#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* In this question, random variables X_1 and X_2 for events A and B respectively, are defined as indicator random variables.
 * i.e., they take value 1 if their respective event occurs, 0 if the event doesn't occur. We have to find the probability that at least one of the event occurs.
 */

int random_number(double p) {
    double random_value = (double)rand() / RAND_MAX; // Generate a random number between 0 and 1
    return random_value <= p ? 1 : 0;
}

// Function to generate the probability of the union of the 2 events
float* union_probability(float p_a, float p_b, float p_a_p_b, int n) {
    float* arr = malloc(sizeof(float) * 4);
    int count_X_1 = 0, count_X_2 = 0, count_X_3 = 0;

    // Seeding the random number generator
    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        int X_1 = random_number(p_a);
        int X_2 = random_number(p_b);
        int X_3 = random_number(p_a_p_b);

        count_X_1 += X_1;
        count_X_2 += X_2;
        count_X_3 += X_3;
    }

    // Using Boolean algebra's property that P(A + B) = P(A) + P(B) - P(AB)
    arr[0] = (float)count_X_1 / n;
    arr[1] = (float)count_X_2 / n;
    arr[2] = (float)count_X_3 / n;
    arr[3] = (float)(count_X_1 + count_X_2 - count_X_3) / n;
    return arr;
}

void freeMemory(float* arr) {
    free(arr);
}

