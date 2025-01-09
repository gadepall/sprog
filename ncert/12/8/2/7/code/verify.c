#include <stdio.h>

// Function to calculate the summation
double Area(int n) {
    double sum = 0.0;

    // Loop through i from 1 to n-1
    for (int i = 1; i < n; i++) {
        // Add the term (i^2 / n^2 - i / n) to the sum
        double term = -((double)(i * i) / (n * n)) + (double)i / n;
        sum += term;
    }

    return 2*sum/n;
}


