#include <stdio.h>
#include <math.h>
double integrate(double a, double b) {
    // Formula: 2 * (x^(3/2)) evaluated from a to b
    double result = 2 * (pow(b, 1.5) - pow(a, 1.5));
    return result;
}

int main() {
    // Initialization
    double x0 = 2.0;  // Starting point
    double xn = x0;   // Current x value
    double A0 = 0.0;  // Initial area
    double An = A0;   // Current area
    double h = 1.0 / 50.0; // Step size
    double xn_plus_1, An_plus_1;
    double a = 2.0; // Lower limit of integration
    double b = 4.0; // Upper limit of integration
    double exact_area = integrate(a, b);

    // Open a file to write output
    FILE *output_file = fopen("output.dat", "w");
    if (output_file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Iterative process
    while (xn <= 4.0) {
        // Calculate next values
        double term1 = h * (3.0 * sqrt(xn));
        double term2 = (h * h / 2.0) * (3.0 / (2.0 * sqrt(xn)));

        // Update area
        An_plus_1 = An + term1 + term2;

        // Write current step to file
       

        // Update x and A for next iteration
        xn_plus_1 = xn + h;
        xn = xn_plus_1;
        An = An_plus_1;
    }

    // Final result
    fprintf(output_file, "Final area at x = %.2f is %.5f\n", xn, An);
    fprintf(output_file, "The exact area under the curve is: %lf\n", exact_area);

    // Close the file
    fclose(output_file);

    return 0;
}

