#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generate_number() {
    int rand_num = (rand() % 6) + 1; // Generate a number between 1 and 6

    if (rand_num <= 2) return 1; // Faces with 1 (2 occurrences)
    if (rand_num <= 5) return 2; // Faces with 2 (3 occurrences)
    return 3; // Face with 3 (1 occurrence)
}

void generate_pmf(double *pmf) {
    srand(time(NULL)); // Seed random number generator

    int trials = 100000;
    int count1 = 0, count2 = 0, count3 = 0;

    for (int i = 0; i < trials; i++) {
        int result = generate_number();
        if (result == 1) count1++;
        else if (result == 2) count2++;
        else count3++;
    }

    // Compute probability
    pmf[0] = (double)count1 / trials;
    pmf[1] = (double)count2 / trials;
    pmf[2] = (double)count3 / trials;
}

