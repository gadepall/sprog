#include <stdio.h>
// code to verify the axiom RS + RS^1 = R
int main() {
    // Iterate over all possible values of R and S (0 or 1)
    for (int R = 0; R <= 1; R++) {
        for (int S = 0; S <= 1; S++) {
            // Compute left-hand side of equation using bitwise operations
            int left_side = (R & S) | (R & ~S);
            // Right-hand side is simply R
            int right_side = R;

            // Print results
            printf("R: %d, S: %d -> (R & S) | (R & ~S) = %d, R = %d -> %s\n",
                   R, S, left_side, right_side, (left_side == right_side) ? "Verified" : "Failed");
        }
    }
    return 0;
}
