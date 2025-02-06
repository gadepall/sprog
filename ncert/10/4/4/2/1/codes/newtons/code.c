#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TOL 1e-7
#define MAX 100

double f1(double x) {
    return 2 * x * x + 2 * sqrt(10) * x + 5;
}

double f1_prime(double x) {
    return 4 * x + 2 * sqrt(10);
}

double f2(double x) {
    return 2 * x * x - 2 * sqrt(10) * x + 5;
}

double f2_prime(double x) {
    return 4 * x - 2 * sqrt(10);
}

double newton(double x0, double (*f)(double), double (*f_prime)(double), double *iterations) {
    for (int itr = 0; itr < MAX; itr++) {
        double fx = f(x0);
        double fpx = f_prime(x0);

        if (fabs(fpx) < TOL) {
            printf("The derivative is too small.\n");
            return -1;
        }
        
        if (itr < 5) {
            iterations[itr] = x0; 
        }

        double next = x0 - fx / fpx;

        if (fabs(next - x0) < TOL) {
            printf("Number of iterations required: %d\n", itr + 1);
            return next;
        }

        x0 = next;
    }

    printf("Maximum number of iterations reached.\n");
    return -1;
}

int main() {
    double *root = malloc(2 * sizeof(double));
    double *itr = malloc(5 * sizeof(double));
    if (root == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    root[0] = newton(-3, f1, f1_prime, itr);
    root[1] = newton(0, f1, f1_prime, itr);

    if (root[0] != -1 && root[1] != -1) {
        printf("Roots of the first quadratic are %.7lf and %.7lf.\n", root[0], root[1]);
    } else {
        printf("Failed to find roots for the first quadratic.\n");
    }

    root[0] = newton(3, f2, f2_prime, itr);
    root[1] = newton(0, f2, f2_prime, itr);

    if (root[0] != -1 && root[1] != -1) {
        printf("Roots of the second quadratic are %.7lf and %.7lf.\n", root[0], root[1]);
    } else {
        printf("Failed to find roots for the second quadratic.\n");
    }

    free(root);
    return 0;
}
