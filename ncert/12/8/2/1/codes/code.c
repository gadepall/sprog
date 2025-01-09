#include <stdio.h>
#include <math.h>

// Function for the first curve: x^2 = 4y -> y = x^2 / 4
double curve1(double x) {
    return x * x / 4.0;
}

// Function for the second curve: x^2 + y^2 = 9/4 -> y = sqrt(9/4 - x^2)
double curve2(double x) {
    return sqrt(9.0 / 4.0 - x * x);
}

// Function to calculate the total area between the curves using the trapezoidal rule
double compute_area_between_curves(double a, double b, int n) {
    double h = (b - a) / n;  // Step size
    double total_area = 0.0;

    for (int i = 0; i < n; i++) {
        double x1 = a + i * h;
        double x2 = a + (i + 1) * h;
        double y1 = curve2(x1) - curve1(x1);  // Difference between curve2 and curve1
        double y2 = curve2(x2) - curve1(x2);
        total_area += (y1 + y2) * h / 2.0;  // Trapezoidal rule
    }

    return total_area;
}

int main() {
    double a = -sqrt(9.0 / 4.0);  // Leftmost intersection point
    double b = sqrt(9.0 / 4.0);   // Rightmost intersection point
    int n = 1000;  // Number of trapezoids

    double total_area = compute_area_between_curves(a, b, n);
    printf("Total area between the curves: %f\n", total_area);

    return 0;
}

