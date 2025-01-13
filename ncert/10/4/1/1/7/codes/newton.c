#include <stdio.h>
#include <math.h>

int main(void) {
    double x = 1.0, xprev;
    int i = 0;

    printf("Iteration\t x_n\t\t Difference\n");
    do {
        xprev = x;
        x = x - (double)(pow(x, 3) - 2 * pow(x, 2) - 6 * x - 8) / (3 * pow(x, 2) - 4 * x - 6);
        printf("%d\t\t %.10lf\t %.10lf\n", i, x, fabs(x - xprev));
        i++;
    } while (fabs(x - xprev) >= 1e-6 && i < 1000);

    if (i >= 1000) {
        printf("Failed to converge within 100 iterations.\n");
    } else {
        printf("Converged at iteration %d: x = %.10lf\n", i, x);
    }

    return 0;
}

