#include <stdio.h>
#include <math.h>

#define MAX 100
#define TOL 1e-6

double g_1(double x) {
    return -((2 * sqrt(10) * x + 5) / (2 * x));
}

double g_2(double x) {
    return -((-2 * sqrt(10) * x + 5) / (2 * x));
}

double g_3(double x)
{
	return -(2 * x * x + 5) / (2 * sqrt(10));
}

double g_4(double x)
{
	return (2 * x * x + 5) / (2 * sqrt(10));
}
/* Fixed-point iteration method */
int Fixed_point(double x0, double x1, double x2, double x3, double *root1, double *root2, double (*g)(double)) {
    double current = x0, next;
    int itr = 0;
    
    // Find the first root
    for (int i = 0; *root1 == 0 && i < 2; i++) 
    {
        while (itr < MAX) 
        {
            next = g(current);
            if (fabs(next - current) < TOL) {
                *root1 = next;
                printf("Root found: %lf\n", *root1);
                break;
            }
            current = next;
            itr++;
        }
        if (*root1 != 0) break;
        printf("Guess %i failed for root\n", i + 1);
        current = x1;
        itr = 0;
    }
    if (*root1 == 0) {
        printf("Failed to detect root.\n");
    }

    // Reset for second root
    current = x2;
    itr = 0;

    // Find the second root
    for (int i = 0; *root2 == 0 && i < 2; i++) {
        while (itr < MAX) {
            next = g(current);
            if (fabs(next - current) < TOL) {
                *root2 = next;
                printf("Root found: %lf\n", *root2);
                return itr;
            }
            current = next;
            itr++;
        }
        if (*root2 != 0) break;
        printf("Guess %i failed for root\n", i + 1);
        current = x3;
        itr = 0;
    }
    if (*root2 == 0) {
        printf("Failed to detect root.\n");
        return -1;
    }

    return itr;
}

int main() {
    double root1 = 0, root2 = 0;
    printf("Quad1:\n");
    // Quad1: x0 = -5, x1 = -3, x2 = 0, x3 = 2
    printf("g_1\n");
    if (Fixed_point(-5, -3, 0, 2, &root1, &root2, g_1) == -1) {
        printf("Fixed-point iteration failed to converge.\n");
    } else {
        printf("Root 1: %lf, Root 2: %lf\n", root1, root2);
    }
    printf("g_3\n");
    if (Fixed_point(-5, -3, 0, 2, &root1, &root2, g_3) == -1) {
        printf("Fixed-point iteration failed to converge.\n");
    } else {
        printf("Root 1: %lf, Root 2: %lf\n", root1, root2);
    }
    
    printf("Quad2:\n");
    // Quad2: x0 = -2, x1 = 0, x2 = 3, x3 = 5
        printf("g_2\n");
    if (Fixed_point(-2, 0, 3, 5, &root1, &root2, g_2) == -1) {
        printf("Fixed-point iteration failed to converge.\n");
    } else {
        printf("Root 1: %lf, Root 2: %lf\n", root1, root2);
    }
    printf("g_4\n");
    if (Fixed_point(-2, 0, 3, 5, &root1, &root2, g_2) == -1) {
        printf("Fixed-point iteration failed to converge.\n");
    } else {
        printf("Root 1: %lf, Root 2: %lf\n", root1, root2);
    }

    return 0;
}
