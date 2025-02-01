#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_TRIALS 100000

// Function to simulate a fair coin toss (returns 1 or 6)
int toss_coin() {
    return (rand() % 2 == 0) ? 1 : 6;  // Coin shows either 1 or 6
}

// Function to simulate a fair die roll (returns a number from 1 to 6)
int toss_die() {
    return (rand() % 6) + 1;  // Die shows a number from 1 to 6
}

// Function to calculate the probability of any sum (coin + die)
double probability_for_sum(int target_sum) {
    int count = 0;
    for (int i = 0; i < NUM_TRIALS; i++) {
        int coin = toss_coin();  // Toss the coin
        int die = toss_die();    // Toss the die
        if (coin + die == target_sum) {
            count++;
        }
    }
    return (double)count / NUM_TRIALS;
}

int main() {
    srand(time(NULL));  // Seed the random number generator

    int target_sum;
    printf("Enter the target sum (between 2 and 12): ");
    scanf("%d", &target_sum);

    // Validate input
    if (target_sum < 2 || target_sum > 12) {
        printf("Invalid sum. Please enter a value between 2 and 12.\n");
        return 1;
    }

    double prob = probability_for_sum(target_sum);
    printf("The probability that the sum is %d is: %lf\n", target_sum, prob);
    return 0;
}
