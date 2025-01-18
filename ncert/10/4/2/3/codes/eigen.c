#include <math.h>
#include <stddef.h>

void find_real_eigenvalues(double matrix[2][2], double eigenvalues[2]) {
    // Extract coefficients from the matrix
    double b = -matrix[1][1]; // -trace
    double c = -matrix[0][1]; // -determinant

    // Compute discriminant
    double discriminant = b * b - 4 * c;

    // Check if the discriminant is non-negative
    if (discriminant >= 0) {
        eigenvalues[0] = (-b + sqrt(discriminant)) / 2.0;
        eigenvalues[1] = (-b - sqrt(discriminant)) / 2.0;
    } else {
        // If discriminant is negative, return NaN (not handling complex roots here)
        eigenvalues[0] = NAN;
        eigenvalues[1] = NAN;
    }
}

