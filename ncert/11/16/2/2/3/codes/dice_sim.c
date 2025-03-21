#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void roll_dice(int *results, int num_trials) {
    srand(time(NULL));
    for (int i = 0; i < num_trials; i++) {
        results[i] = (rand() % 6) + 1;  // Generates a number from 1 to 6
    }
}

void filter_multiples_of_3(int *input, int num_trials, int *filtered, int *filtered_size) {
    int count = 0;
    for (int i = 0; i < num_trials; i++) {
        if (input[i] % 3 == 0) {
            filtered[count++] = input[i];
        }
    }
    *filtered_size = count;
}

void count_occurrences(int *filtered, int size, int *count_3, int *count_6) {
    *count_3 = 0;
    *count_6 = 0;
    for (int i = 0; i < size; i++) {
        if (filtered[i] == 3) (*count_3)++;
        else if (filtered[i] == 6) (*count_6)++;
    }
}

