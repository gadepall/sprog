#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int generate_number() {
    int rand_num = (rand() % 6) + 1; // Generate a number between 1 and 6

    if (rand_num <= 3) return 2; // 1, 2, 3 → 1
    if (rand_num <= 5) return 1; // 4, 5 → 2
    return 3; // 6 → 3
}

void generate_pmf(double * pmf) {
    srand(time(NULL)); // Seed the random number generator

    int trials = 100000; // Number of times to run the simulation
    int count1 = 0, count2 = 0, count3 = 0;

    for (int i = 0; i < trials; i++) {
        int result = generate_number();
        if (result == 1) count1++;
        else if (result == 2) count2++;
        else count3++;
    }

    // Use floating-point division to get correct probabilities
    pmf[0] = (double)count1 / (double)trials;
    pmf[1] = (double) count2 /(double) trials;
    pmf[2] = (double) count3 /(double) trials;
}


