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

    // Augmented matrix representation of the system
    double matrix[2][3] = {
        {a1, b1, -c1},
        {a2, b2, -c2}
    };

    // Row reduction to row-echelon form
    // Make the first element of the first row 1 (leading 1)
    if (fabs(matrix[0][0]) > epsilon) {
        double factor = matrix[0][0];
        for (int i = 0; i < 3; i++) {
            matrix[0][i] /= factor;
        }
    }

    // Make the first element of the second row 0
    if (fabs(matrix[1][0]) > epsilon) {
        double factor = matrix[1][0];
        for (int i = 0; i < 3; i++) {
            matrix[1][i] -= factor * matrix[0][i];
        }
    }

    // Make the second element of the second row 1 (leading 1)
    if (fabs(matrix[1][1]) > epsilon) {
        double factor = matrix[1][1];
        for (int i = 1; i < 3; i++) {
            matrix[1][i] /= factor;
        }
    }

    // Make the second element of the first row 0
    if (fabs(matrix[1][1]) > epsilon) {
        double factor = matrix[0][1];
        for (int i = 1; i < 3; i++) {
            matrix[0][i] -= factor * matrix[1][i];
        }
    }

    // Check if the system has a unique solution
    if (fabs(matrix[1][1]) < epsilon) {
        // The lines are either parallel or coincident
        if (fabs(matrix[1][2]) < epsilon) {
            intersection.coincident = 1;  // Lines are coincident
        } else {
            intersection.parallel = 1;    // Lines are parallel
        }
    } else {
        // The system has a unique solution
        intersection.x = matrix[0][2];
        intersection.y = matrix[1][2];
        intersection.exists = 1;
    }

    return intersection;
}
