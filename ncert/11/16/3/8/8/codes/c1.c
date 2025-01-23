#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to calculate PMF using Z-transform
void calculate_pmf_z_transform(double *pmf) {
    int n = 3; // Number of tosses
    double p = 0.5; // Probability of tails for a fair coin

    // Compute coefficients of Z-transform: (p + (1-p)z)^n
    // Z-transform expansion: (p + qz)^n = \sum_{k=0}^{n} C(n, k) * p^(n-k) * q^k * z^k
    // PMF corresponds to the coefficients of z^k

    for (int k = 0; k <= n; k++) {
        // Calculate binomial coefficient C(n, k)
        double binomial_coefficient = 1;
        for (int i = 0; i < k; i++) {
            binomial_coefficient *= (n - i) / (double)(i + 1);
        }

        // Compute PMF value for k tails using Z-transform
        pmf[k] = binomial_coefficient * pow(p, n - k) * pow(1 - p, k);
    }
}

// Expose to Python
__attribute__((visibility("default"))) __attribute__((used))
void get_probabilities(double *pmf) {
    calculate_pmf_z_transform(pmf);
}

