#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_TRIALS 100000

// Function to simulate a fair coin toss (returns 1 or 0 for heads or tails)
int toss_coin() {
    return (rand() % 2 == 0) ? 1 : 0;  // Coin shows 1 (head) or 0 (tail)
}

// Function to calculate the exact probability of getting exactly n heads in 3 tosses
double probability_at_exact_heads(int target_heads) {
    int count = 0;
    for (int i = 0; i < NUM_TRIALS; i++) {
        int heads = 0;
        for (int j = 0; j < 3; j++) {  // Three tosses
            heads += toss_coin();  // Count heads (1 for head, 0 for tail)
        }
        if (heads == target_heads) {
            count++;
        }
    }
    return (double)count / NUM_TRIALS;
}

int main() {
    srand(time(NULL));  // Seed the random number generator

    int target_heads;
    printf("Enter the target number of heads (0 to 3): ");
    scanf("%d", &target_heads);

    // Validate input
    if (target_heads < 0 || target_heads > 3) {
        printf("Invalid number of heads. Please enter a value between 0 and 3.\n");
        return 1;
    }

    double prob = probability_at_exact_heads(target_heads);
    printf("The probability of getting exactly %d heads is: %lf\n", target_heads, prob);
    return 0;
}

