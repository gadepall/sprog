#include <stdio.h>
#include <math.h>

typedef struct {
    double x;
    double y;
    int exists;     // 1 = intersection exists, 0 = no intersection
    int coincident; // 1 = lines are coincident, 0 = otherwise
    int parallel;   // 1 = lines are parallel but not coincident, 0 = otherwise
} Point;

Point find_intersection(double a1, double b1, double c1,
                        double a2, double b2, double c2) {
    Point intersection = {0, 0, 0, 0, 0};
    double epsilon = 1e-9;

    // Matrix A and vector b
    double A[2][2] = {
        {a1, b1},
        {a2, b2}
    };
    double b[2] = {-c1, -c2};

    // Decompose A into L and U
    double L[2][2] = {0}, U[2][2] = {0};

    // Initialize L as identity
    L[0][0] = 1;
    L[1][1] = 1;

    // Fill U and L using Doolittle's method
    U[0][0] = A[0][0];
    U[0][1] = A[0][1];
    L[1][0] = A[1][0] / U[0][0];
    U[1][1] = A[1][1] - L[1][0] * U[0][1];

    // Check for singularity (determinant is zero)
    if (fabs(U[0][0]) < epsilon || fabs(U[1][1]) < epsilon) {
        if (fabs(a1 * b2 - a2 * b1) < epsilon) {
            if (fabs(c1 * b2 - c2 * b1) < epsilon) {
                intersection.coincident = 1; // Lines are coincident
            } else {
                intersection.parallel = 1;   // Lines are parallel
            }
        }
        return intersection;  // Exit early if lines are parallel or coincident
    }

    // Forward substitution to solve L * y = b
    double y[2] = {0};
    y[0] = b[0];
    y[1] = b[1] - L[1][0] * y[0];

    // Back substitution to solve U * x = y
    double x[2] = {0};
    x[1] = y[1] / U[1][1];
    x[0] = (y[0] - U[0][1] * x[1]) / U[0][0];

    // Populate intersection point
    intersection.x = x[0];
    intersection.y = x[1];
    intersection.exists = 1;

    return intersection;
}

int main() {
    double a1 = 5, b1 = -4, c1 = 8; // Line 1: 5x - 4y + 8 = 0
    double a2 = 7, b2 = 6, c2 = -9; // Line 2: 7x + 6y - 9 = 0

    Point result = find_intersection(a1, b1, c1, a2, b2, c2);

    if (result.exists) {
        printf("Intersection exists at: (%.6f, %.6f)\n", result.x, result.y);
    } else if (result.coincident) {
        printf("The lines are coincident.\n");
    } else if (result.parallel) {
        printf("The lines are parallel but not coincident.\n");
    } else {
        printf("No solution exists.\n");
    }

    return 0;
}

