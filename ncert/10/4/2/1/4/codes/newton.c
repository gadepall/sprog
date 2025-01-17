#include <stdio.h>
#include <math.h>

int main(void) {
    double x = 0.5, xprev;
    int i = 1;

    printf("Iteration\t x_n\t\t Difference\n");
    do {
        xprev = x;
        x = x - (double)(16*pow(x, 2) - 8*x + 1) / (32 * x - 8);
        printf("%d\t\t %.10lf\t %.10lf\n", i, x, fabs(x - xprev));
        i++;
    } while (fabs(x - xprev) >= 1e-6 && i < 1000);

    if (i >= 1000) {
        printf("Failed to converge within 100 iterations.\n");
    } else {
        printf("Converged at iteration %d: x = %.10lf\n", i-1, x);
    }

    return 0;
}

